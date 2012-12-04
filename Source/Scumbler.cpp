// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"
#include "Commands.h"

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
   AudioProcessorGraph::Node* node;

   AudioProcessorGraph::AudioGraphIOProcessor* in = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
   node = fGraph.addNode(in);
   fInputNode = node->nodeId;

   AudioProcessorGraph::AudioGraphIOProcessor* out = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
   node = fGraph.addNode(out);
   fOutputNode = node->nodeId;

   this->Connect(fInputNode, fOutputNode);


}


Scumbler::Result Scumbler::Connect(uint32 source, uint32 dest)
{
   return this->HandleConnection(source, dest, true);
}

Scumbler::Result Scumbler::Disconnect(uint32 source, uint32 dest)
{
   return this->HandleConnection(source, dest, false);

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

Scumbler::Result Scumbler::HandleConnection(uint32 source, uint32 dest, bool connecting)
{
   Scumbler::Result retval = Scumbler::kFailure;
   fnPtr op = nullptr;

   if (connecting)
   {
      op = &AudioProcessorGraph::addConnection;
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

   AudioProcessorGraph::Node* srcNode  = fGraph.getNodeForId(source);
   AudioProcessorGraph::Node* destNode = fGraph.getNodeForId(dest);
   // only proceed if those nodes exist.
   if (srcNode && destNode)
   {
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


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Scumbler.cpp"
#endif
