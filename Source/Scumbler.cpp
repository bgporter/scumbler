// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"
#include "Commands.h"

#include "Processors/Passthrough.h"

#ifdef qUnitTests
namespace
{
   Scumbler* instance = nullptr;
};

#endif

Scumbler::Scumbler(AudioDeviceManager& deviceManager)
: fDeviceManager(deviceManager)
, fPlaying(false)
, fInputNode(-1)
, fOutputNode(-1)
{
#ifdef qUnitTests
   jassert(nullptr == instance);
   // save the pointer to this scumbler instance as 'the' scumbler instance.
   instance = this;
#endif
   fPlayer.setProcessor(&fGraph);
   fDeviceManager.addAudioCallback(&fPlayer);
   this->Reset();
}

Scumbler::~Scumbler()
{
   fDeviceManager.removeAudioCallback(&fPlayer);
   fPlayer.setProcessor(nullptr);
   fGraph.clear();
}


void Scumbler::TogglePlay()
{
   if (fPlaying)
   {
      this->Pause();
   }
   else
   {
      this->Play();
   }
}

bool Scumbler::IsPlaying() const
{
   return fPlaying;
}

void Scumbler::Reset()
{
   this->Pause();

   // clear out the processor graph
   fGraph.clear();
   
   // Create and add new input/output processor nodes. 
   AudioProcessorGraph::AudioGraphIOProcessor* in = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
   fInputNode = this->AddProcessor(in);
   //fInputNode = node->nodeId;

   AudioProcessorGraph::AudioGraphIOProcessor* out = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
   fOutputNode = this->AddProcessor(out);

   this->Connect(fInputNode, fOutputNode);

}


Scumbler::Result Scumbler::Connect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, true);
}

Scumbler::Result Scumbler::Disconnect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, false);

}


Scumbler::Result Scumbler::InsertBetween(NodeId before, NodeId newNode, NodeId after)
{
   Scumbler::Result retval = kFailure;

   before = this->HandleSpecialNode(before);
   after = this->HandleSpecialNode(after);

   // 1: we can't succeed of before and after aren't connected.
   if (!fGraph.isConnected(before, after))
   {
      return kNotConnected;
   }
   // the new connections both need to be legal before we start messing with things. 
   if (!fGraph.canConnect(before, 0, newNode, 0) || 
       !fGraph.canConnect(newNode, 0, after, 0))
   {
      return kIllegalConnection;
   }
   //  first, disconnect the two nodes that are already being connected.
   retval = this->Disconnect(before, after);
   if (kSuccess == retval)
   {
      retval = this->Connect(before, newNode);
      if (kSuccess == retval)
      {
         retval = this->Connect(newNode, after);
      }
   }

   return retval;
}



Scumbler::Result Scumbler::RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after)
{
   Scumbler::Result retval = kFailure;

   // if needed, look up the node ids for the input/output nodes.
   before = this->HandleSpecialNode(before);
   after = this->HandleSpecialNode(after);

   // 1. First pre-condition: before must already be connected to nTR, and 
   // nTR must be connected to after.
   if (! (fGraph.isConnected(before, nodeToRemove) && 
          fGraph.isConnected(nodeToRemove, after)))
   {
      return kNotConnected;
   }

   // 2. Next precondition: The connection between `before` and `after` needs 
   // to be a legal connection. I don't think this one is possible if we assume
   // that all graphs start out connecting input->output and are after that only 
   // added to with the InsertBetween() method. 
   if (! fGraph.canConnect(before, 0, after, 0))
   {
      return kIllegalConnection;
   }

   // 3. Disconnect `nodeToRemove on either side.
   retval = this->Disconnect(before, nodeToRemove);
   if (kSuccess == retval)
   {
      retval = this->Disconnect(nodeToRemove, after);
      if (kSuccess == retval)
      {
         // 4. Re-connect the before and after nodes, as if the nodeToRemove had 
         // never been there.
         retval = this->Connect(before, after);
      }
   }

   return retval;
}


Scumbler* Scumbler::GetInstance()
{
   return instance;
}

void Scumbler::Play()
{
   if (!fPlaying)
   {
      fPlaying = true;
      // !!! TODO
   }
}

void Scumbler::Pause()
{
   if (fPlaying)
   {
      fPlaying = false;
      // !!! TODO
   }
}

Scumbler::Result Scumbler::HandleConnection(NodeId source, NodeId dest, bool connecting)
{
   Scumbler::Result retval = Scumbler::kFailure;
   fnPtr op = nullptr;

   AudioProcessorGraph::Node* srcNode  = fGraph.getNodeForId(source);
   AudioProcessorGraph::Node* destNode = fGraph.getNodeForId(dest);
   // only proceed if those nodes exist.
   if (srcNode && destNode)
   {
      if (connecting)
      {
         op = &AudioProcessorGraph::addConnection;
         // if they're already connected, there's nothing to do.
         if (fGraph.isConnected(source, dest))
         {
            return kAlreadyConnected;
         }      
         // verify that we can at least connect the lower channels of these nodes.
         if (!fGraph.canConnect(source, 0, dest, 0))
         {
            return kIllegalConnection;
         }

      }
      else
      {
         op = &AudioProcessorGraph::removeConnection;
         // bail out early if the two nodes aren't connected.
         if (!fGraph.isConnected(source, dest))
         {
            return kNotConnected;
         }
      }

      retval = kSuccess;
      AudioProcessor* srcFilter  = srcNode->getProcessor();
      int numSrcChannels         = srcFilter->getNumOutputChannels();
      AudioProcessor* destFilter = destNode->getProcessor();
      int numDestChannels        = destFilter->getNumInputChannels();
      // in our immediate situation, we're only interested in 2 channels at most. 
      // Future versions might be interested in more.
      if (numSrcChannels == numDestChannels)
      {
         for (int index = 0; index < numSrcChannels; ++index)
         {
            mCallMemberFn(fGraph, op)(source, index, dest, index);
         }

      }
      else if (numSrcChannels < numDestChannels)
      {
         // connect the single input to both pins of the destination filter
         mCallMemberFn(fGraph, op)(source, 0, dest, 0);
         mCallMemberFn(fGraph, op)(source, 0, dest, 1);
      }
      else // numSrcChannels > numDestChannels
      {
         // connect both source pins to the single pin of the dest filter.
         mCallMemberFn(fGraph, op)(source, 0, dest, 0);
         mCallMemberFn(fGraph, op)(source, 1, dest, 0);
      }
   }
   else
   {
      // one or other of the requested nodes aren't present in the graph.
      if (nullptr == srcNode)
      {
         return kNoSourceNode;
      }
      if (nullptr == destNode)
      {
         return kNoDestNode;
      }
   }
   return retval;
}

NodeId Scumbler::AddProcessor(AudioProcessor* p)
{
   AudioProcessorGraph::Node* node;
   node = fGraph.addNode(p);
   return node->nodeId;
}

NodeId Scumbler::HandleSpecialNode(NodeId node)
{
   NodeId retval = node;
   if (kInput == node)
   {
      retval = fInputNode;
   }
   else if (kOutput == node)
   {
      retval = fOutputNode;
   }

   return retval;
}
/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Scumbler.cpp"
#endif
