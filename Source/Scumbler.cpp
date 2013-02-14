// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"


#include "Commands.h"
#include "Processors/Passthrough.h"
#include "Track.h"


#ifdef qUnitTests
// If we are compiling unit tests in, we do a fake singleton that's only
// visible in file scope so the unit test code can have access to the 
// one and only Scumbler object.
namespace
{
   Scumbler* instance = nullptr;
};

#endif

Scumbler::Scumbler(AudioDeviceManager& deviceManager, 
                  AudioPluginFormatManager& pluginManager)
: fDeviceManager(deviceManager)
, fPluginManager(pluginManager)
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
   this->removeAllChangeListeners();
   fDeviceManager.removeAudioCallback(&fPlayer);
   fPlayer.setProcessor(nullptr);
   fGraph.clear();
}

#ifdef qUnitTests 
Scumbler* Scumbler::GetInstance()
{
   return instance;
}
#endif

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
   this->sendChangeMessage();

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

   AudioProcessorGraph::AudioGraphIOProcessor* out = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
   fOutputNode = this->AddProcessor(out);

   this->Connect(fInputNode, fOutputNode);

   // Delete any tracks that we have, returning to zero tracks.
   fTracks.clear();
   // ... and then add a single track to start out.
   this->AddTrack();
   // let anyone listening tk::know that we've changed.
   this->sendChangeMessage();

}


tk::Result Scumbler::Connect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, true);
}

tk::Result Scumbler::Disconnect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, false);
}


tk::Result Scumbler::InsertBetween(NodeId before, NodeId newNode, NodeId after)
{
   tk::Result retval = tk::kFailure;

   before = this->HandleSpecialNode(before);
   after = this->HandleSpecialNode(after);

   // 1: we can't succeed of before and after aren't connected.
   if (!fGraph.isConnected(before, after))
   {
      return tk::kNotConnected;
   }
   // the new connections both need to be legal before we start messing with things. 
   if (!fGraph.canConnect(before, 0, newNode, 0) || 
       !fGraph.canConnect(newNode, 0, after, 0))
   {
      return tk::kIllegalConnection;
   }
   //  first, disconnect the two nodes that are already being connected.
   retval = this->Disconnect(before, after);
   if (tk::kSuccess == retval)
   {
      retval = this->Connect(before, newNode);
      if (tk::kSuccess == retval)
      {
         retval = this->Connect(newNode, after);
      }
   }

   return retval;
}



tk::Result Scumbler::RemoveBetween(NodeId before, NodeId nodeToRemove, 
   NodeId after, bool deleteNode)
{
   tk::Result retval = tk::kFailure;

   // if needed, look up the node ids for the input/output nodes.
   before = this->HandleSpecialNode(before);
   after = this->HandleSpecialNode(after);

   // 1. First pre-condition: before must already be connected to nTR, and 
   // nTR must be connected to after.
   if (! (fGraph.isConnected(before, nodeToRemove) && 
          fGraph.isConnected(nodeToRemove, after)))
   {
      return tk::kNotConnected;
   }

   // 2. Next precondition: The connection between `before` and `after` needs 
   // to be a legal connection. I don't think this one is possible if we assume
   // that all graphs start out connecting input->output and are after that only 
   // added to with the InsertBetween() method. 
   if (! fGraph.canConnect(before, 0, after, 0))
   {
      return tk::kIllegalConnection;
   }

   // 3. Disconnect `nodeToRemove on either side.
   retval = this->Disconnect(before, nodeToRemove);
   if (tk::kSuccess == retval)
   {
      retval = this->Disconnect(nodeToRemove, after);
      if (tk::kSuccess == retval)
      {
         // 4. Re-connect the before and after nodes, as if the nodeToRemove had 
         // never been there.
         retval = this->Connect(before, after);
         if (deleteNode)
         {
            fGraph.removeNode(nodeToRemove);
         }
      }
   }

   return retval;
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


int Scumbler::GetNumTracks() const
{
   return fTracks.size();
}

tk::Result Scumbler::AddTrack(const String& name)
{
   fTracks.add(new Track(this, kPreEffects, kPostEffects, name));
   this->sendChangeMessage();
   return tk::kSuccess;
}


tk::Result Scumbler::DeleteTrack(int index)
{
   tk::Result retval = tk::kFailure;
   if (fTracks[index])
   {
      fTracks.remove(index);
      retval = tk::kSuccess;
      this->sendChangeMessage();
   }
   return retval;
}


tk::Result Scumbler::MoveTrack(int fromIndex, int toIndex)
{
   tk::Result retval = tk::kFailure;
   // make sure the track really exists
   if (fTracks[fromIndex])
   {
      if (toIndex >= fTracks.size())
      {
         // rely on the behavior of the OwnedArray class -- a toIndex < 0 
         // means 'move this to the end.' We modify that slightly so that anything
         // out of bounds results in move to the end.
         toIndex = -1;
      }
      fTracks.move(fromIndex, toIndex);
      retval = tk::kSuccess;
      this->sendChangeMessage();
   }
   return retval;
}



Track* Scumbler::GetTrack(int index) const
{
   // if index is out of range (or if the array is legitimately holding a 
   // nullptr), this will return nullptr;
   return fTracks[index];
}


tk::Result Scumbler::HandleConnection(NodeId source, NodeId dest, bool connecting)
{
   tk::Result retval = tk::kFailure;
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
            return tk::kAlreadyConnected;
         }      
         // verify that we can at least connect the lower channels of these nodes.
         if (!fGraph.canConnect(source, 0, dest, 0))
         {
            return tk::kIllegalConnection;
         }

      }
      else
      {
         op = &AudioProcessorGraph::removeConnection;
         // bail out early if the two nodes aren't connected.
         if (!fGraph.isConnected(source, dest))
         {
            return tk::kNotConnected;
         }
      }

      retval = tk::kSuccess;
      AudioProcessor* srcFilter  = srcNode->getProcessor();
      int numSrcChannels         = srcFilter->getNumOutputChannels();
      AudioProcessor* destFilter = destNode->getProcessor();
      int numDestChannels        = destFilter->getNumInputChannels();
      // in our immediate situation, we're only interested in 2 channels at most. 
      // Future versions might be interested in more.
      // !!! NOTE that a better way to do this is to instead check whether 
      // isInputChannelStereoPair/isOutputChannelStereoPair() is true and to 
      // hook things up accordingly.
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
         return tk::kNoSourceNode;
      }
      if (nullptr == destNode)
      {
         return tk::kNoDestNode;
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

tk::Result Scumbler::DeleteNode(NodeId node)
{
   tk::Result retval = tk::kFailure;
   if (fGraph.removeNode(node))
   {
      retval = tk::kSuccess;
   }
   return retval;
}


NodeId Scumbler::LoadPlugin(const PluginDescription& description, String& errorMessage)
{
   NodeId retval = tk::kInvalidNode;
   AudioPluginInstance* loaded = fPluginManager.createPluginInstance(description, errorMessage);
   if (loaded)
   {
      retval = this->AddProcessor(loaded);
   }
   return retval;
}


AudioProcessorEditor* Scumbler::GetEditorForNode(NodeId node, bool useGeneric)
{
   AudioProcessorEditor* retval = nullptr;
   AudioProcessorGraph::Node* plugin = fGraph.getNodeForId(node); 
   if (nullptr != plugin)
   {
      if (!useGeneric)
      {
         retval = plugin->getProcessor()->createEditorIfNeeded();
         if (nullptr == retval)
         {
            // we failed to create a native editor, so fall back to the generic.
            useGeneric = true;
         }
      }
      if (useGeneric)
      {
         retval = new GenericAudioProcessorEditor(plugin->getProcessor());
      }

   }
   return retval;
}

tk::Result Scumbler::GetPluginDescriptionForNode(NodeId nodeId, PluginDescription& desc)
{
   tk::Result retval = tk::kFailure;
   AudioProcessorGraph::Node* node = fGraph.getNodeForId(nodeId);
   if (nullptr != node)
   {
      AudioPluginInstance* plugin = dynamic_cast<AudioPluginInstance*>(node->getProcessor());
      if (nullptr != plugin)
      {
         plugin->fillInPluginDescription(desc);
         retval = tk::kSuccess;
      }
   }
   return retval;
}


NodeId Scumbler::HandleSpecialNode(NodeId node)
{
   NodeId retval = node;
   if (tk::kInput == node)
   {
      retval = fInputNode;
   }
   else if (tk::kOutput == node)
   {
      retval = fOutputNode;
   }

   return retval;
}



/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Scumbler.cpp"
#endif
