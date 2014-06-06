// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"

#include <math.h>

#include "Commands.h"
#include "Processors/Gain.h"
#include "Processors/Passthrough.h"
#include "Processors/SampleCounter.h"
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




float DbToFloat(float db)
{
   return pow(10.0, db/20.0);
}

float GainToDb(float gain)
{
   return 20.0 * log10(gain);
}


Scumbler::Scumbler(AudioDeviceManager& deviceManager, 
                  AudioPluginFormatManager& pluginManager)
: fDeviceManager(deviceManager)
, fPluginManager(pluginManager)
, fProcessing(false)
, fPlaying(false)
, fPluginSort(KnownPluginList::defaultOrder)
, fInputNode(tk::kInvalidNode)
, fOutputNode(tk::kInvalidNode)
, fSampleCount(nullptr)
, fSoloTrack(nullptr)
, fActiveTrackIndex(-1)
, fOutputVolume(0.0f) 
, fGainNode(tk::kInvalidNode)
{
#ifdef qUnitTests
   jassert(nullptr == instance);
   // save the pointer to this scumbler instance as 'the' scumbler instance.
   instance = this;
#endif
   this->StartProcessing();
   this->Reset();
   this->SetOutputVolume(fOutputVolume);
}

Scumbler::~Scumbler()
{
   this->removeAllChangeListeners();
   if (fProcessing)
   {
      this->StopProcessing();
   }
   fGraph.clear();
}


tk::Result Scumbler::StartProcessing()
{
   tk::Result retval = tk::kAlreadyStarted;
   if (!fProcessing)
   {
      fPlayer.setProcessor(&fGraph);
      fDeviceManager.addAudioCallback(&fPlayer);
      fProcessing = true;
   }
   return retval;

}


tk::Result Scumbler::StopProcessing()
{
   tk::Result retval = tk::kAlreadyStopped;
   if (fProcessing)
   {
      fDeviceManager.removeAudioCallback(&fPlayer);
      fPlayer.setProcessor(nullptr);
      fProcessing = false;
      retval = tk::kSuccess;
   }
   return retval;
}



void Scumbler::LoadXml(XmlElement* e, StringArray& errors, int formatVersion)
{
   if (e->hasTagName("scumbler"))
   {
      this->Reset(false);
      #if 0
      // retrieve the scumbler values, but don't do anything with them yet.
      int formatVersion = e->getIntAttribute("fileFormat");
      int activeTrackIndex = e->getIntAttribute("activeTrackIndex");
      float outputvolume = e->getDoubleAttribute("outputVolume");

      // get the 'tracks' tag that contains all of the track data.
      XmlElement* tracks = e->getChildByName("tracks");
      int trackIndex = 0;
      forEachXmlChildElement(*tracks, track)
      {
         this->AddTrack();
         Track* t = this->GetTrack(trackIndex++);
         t->LoadXml(track, errors, formatVersion);
      }
      #endif
   }
   else
   {
      // !!! add an error string to the error message list
      // 
   }

}


XmlElement* Scumbler::DumpXml(int formatVersion) const
{
   XmlElement* node = new XmlElement("scumbler");
   node->setAttribute("fileFormat", formatVersion);
   node->setAttribute("activeTrackIndex", fActiveTrackIndex);
   node->setAttribute("outputVolume", fOutputVolume);
   XmlElement* trackContainer = node->createNewChildElement("tracks");
   for (int i = 0; i < this->GetNumTracks(); ++i)
   {
      Track* t = this->GetTrack(i);
      XmlElement* trackData = t->DumpXml(formatVersion);
       trackContainer->addChildElement(trackData);
   }

   return node;
}

void Scumbler::changeListenerCallback(ChangeBroadcaster* source)
{
   std::cout << "Scumbler::changeListenerCallback()" << std::endl;
   if (source == fSampleCount)
   {
      // just notify that we've changed so the time readout can change.
      this->sendChangeMessage();
   }
   else
   {
      // for now, assume that we're being notified that a track wants us to delete it.
      // look through the list & delete any tracks that want to be deleted.
      // We go through the list in reverse so that we don't skip items if there are 
      // more than one that want to be deleted.
      
      for (int i = (fTracks.size() - 1); i >= 0; --i)
      {
         Track* t = fTracks[i];
         if (t->WantsToBeDeleted())
         {
            if (i == this->GetActiveTrackIndex())
            {
               this->ActivatePreviousTrack();
            }
            this->DeleteTrack(i);
         }
      }
   }
}


void Scumbler::SetPluginSortOrder(KnownPluginList::SortMethod sort)
{
   fPluginSort = sort;
}

KnownPluginList::SortMethod Scumbler::GetPluginSortOrder() const
{
   return fPluginSort;
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
   std::cout << "Scumbler::TogglePlay->sendChangeMessage" << std::endl;
   this->sendChangeMessage();

}

bool Scumbler::IsPlaying() const
{
   return fPlaying;
}

void Scumbler::Reset(bool addFirstTrack)
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

   // connect a gain processor in the middle:
   GainProcessor* gain = new GainProcessor(nullptr, 2);
   NodeId gainNode = this->AddProcessor(gain);
   if (tk::kSuccess == this->InsertBetween(fInputNode, gainNode, fOutputNode))
   {
      fOutputGain = gain;
      fGainNode = gainNode;

   }
   else
   {
      fOutputGain = nullptr;
      fGainNode = tk::kInvalidNode;
   }

   // Add the processor that counts samples for us
   fSampleCount = new SampleCounterProcessor(this, 5000);
   fSampleCount->addChangeListener(this);
   fSampleCountNode = this->AddProcessor(fSampleCount);
   this->Connect(fInputNode, fSampleCountNode);


   // Delete any tracks that we have, returning to zero tracks.
   fTracks.clear();

   if (addFirstTrack)
   {
      // ... and then add a single track to start out.
      this->AddTrack();
      // (and make sure it's active so it receives input!)
      this->ActivateTrack(0);
   }
   // let anyone listening tk::know that we've changed.
   std::cout << "Scumbler::Reset->sendChangeMessage" << std::endl;
   this->sendChangeMessage();

}

void Scumbler::SetOutputVolume(float volumeInDb)
{
   if (volumeInDb != fOutputVolume)
   {
      fOutputVolume = volumeInDb;

      //!!! Send the new gain to the audio processor that actually controls
      // the output.
      float gain = DbToFloat(fOutputVolume);
      fOutputGain->SetGain(gain);

      // update our observers.
      std::cout << "Scumbler::SetOutputVolume->sendChangeMessage" << std::endl;
      this->sendChangeMessage();
   }
}

float Scumbler::GetOutputVolume() const
{
   return fOutputVolume;
}

uint64 Scumbler::GetSampleCount() const
{
   return fSampleCount->GetSampleCount();
}

tk::Result Scumbler::Connect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, true);
}

tk::Result Scumbler::Disconnect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, false);
}


tk::Result Scumbler::InsertBetween(NodeId before, NodeId newNode, NodeId after, bool disconnect)
{
   tk::Result retval = tk::kFailure;

   before = this->HandleSpecialNode(before);
   after = this->HandleSpecialNode(after);

   if (disconnect)
   {
      // 1: we can't succeed of before and after aren't connected.
      if (!fGraph.isConnected(before, after))
      {
         return tk::kNotConnected;
      }
   }
   // the new connections both need to be legal before we start messing with things. 
   if (!fGraph.canConnect(before, 0, newNode, 0) || 
       !fGraph.canConnect(newNode, 0, after, 0))
   {
      return tk::kIllegalConnection;
   }
   //  first, disconnect the two nodes that are already being connected.
   retval = disconnect ? this->Disconnect(before, after) : tk::kSuccess;
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
   NodeId after, bool deleteNode, bool reconnect)
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
         if (reconnect)
         {
            // 4. Re-connect the before and after nodes, as if the nodeToRemove had 
            // never been there.
            retval = this->Connect(before, after);
         }
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
   std::cout << "Scumbler::AddTrack->sendChangeMessage" << std::endl;
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
   std::cout << "Scumbler::DeleteTrack->sendChangeMessage" << std::endl;
      this->sendChangeMessage();
   }
   return retval;
}


tk::Result Scumbler::ActivateTrack(int index)
{
   tk::Result retval = tk::kFailure;
   if ((index >= 0) && (index < this->GetNumTracks()) )
   {
      retval = tk::kSuccess;
      if (index != fActiveTrackIndex)
      {
         Track* newActive = this->GetTrack(index);
         newActive->SetActive(true);
   std::cout << "Scumbler::ActivateTrack->sendChangeMessage" << std::endl;
         this->sendChangeMessage();
      }
   }
   return retval;
}

tk::Result Scumbler::ActivateNextTrack()
{
   tk::Result retval = tk::kFailure;
   int trackCount = this->GetNumTracks();
   if (trackCount > 1)
   {
      int currentActiveTrack = this->GetActiveTrackIndex();
      if (currentActiveTrack >= 0)
      {
         if (++currentActiveTrack >= trackCount)
         {
            currentActiveTrack = 0;
         }
         retval = this->ActivateTrack(currentActiveTrack);
      }
   }
   return retval;
}


tk::Result Scumbler::ActivatePreviousTrack()
{
   tk::Result retval = tk::kFailure;
   int trackCount = this->GetNumTracks();
   if (trackCount > 1)
   {
      int currentActiveTrack = this->GetActiveTrackIndex();
      if (currentActiveTrack >= 0)
      {
         if (--currentActiveTrack < 0)
         {
            currentActiveTrack = trackCount - 1;
         }
         retval = this->ActivateTrack(currentActiveTrack);
      }
   }
   return retval;   
}

tk::Result Scumbler::TrackIsActivating(Track* trackBeingActivated)
{
   tk::Result retval = tk::kSuccess;

   // get a pointer to the currently active track (this may return nullptr)
   Track* currentActiveTrack = this->GetTrack(fActiveTrackIndex);
   if (nullptr != currentActiveTrack && currentActiveTrack != trackBeingActivated)
   {
      currentActiveTrack->SetActive(false);
   }
   fActiveTrackIndex = fTracks.indexOf(trackBeingActivated);

   return retval;
}

int Scumbler::GetActiveTrackIndex() const
{
   return fActiveTrackIndex;
}


tk::Result Scumbler::SoloTrack(Track* trackToSolo)
{
   fSoloTrack = trackToSolo;
   return tk::kSuccess;
}

Track* Scumbler::GetSoloTrack() const
{
   return fSoloTrack;
}

tk::Result Scumbler::ResetAllTracks()
{
   for (int i = 0; i < this->GetNumTracks(); ++i)
   {
      Track* t = this->GetTrack(i);
      t->ResetLoop();
   }
   // set the sample count back to zero.
   fSampleCount->Reset();
   return tk::kSuccess;

}

tk::Result Scumbler::SeekAllTracksAbsolute(int loopPos)
{
   for (int i = 0; i < this->GetNumTracks(); ++i)
   {
      Track* t = this->GetTrack(i);
      t->SeekAbsolute(loopPos);
   }

   return tk::kSuccess;

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
   std::cout << "Scumbler::MoveTrack->sendChangeMessage" << std::endl;
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
      // Add this plugin to the AudioProcessorGraph. It gets connected elsewhere
      // (in PluginBlockComponent.cpp)
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


tk::Result Scumbler::GetStateInformationForNode(NodeId nodeId, MemoryBlock& m)
{
   tk::Result retval = tk::kFailure;
   AudioProcessorGraph::Node* node = fGraph.getNodeForId(nodeId);
   if (nullptr != node)
   {
      // get the actual processor object behind this node, and have 
      // it stuff its state data into the memory block that we've been passed.
      AudioProcessor* processor = node->getProcessor();
      processor->getStateInformation(m);
      retval = tk::kSuccess;
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
      // if we have inserted a gain processor before the output, that should
      // be treated as the output; everything goes through it.
      retval = (tk::kInvalidNode == fGainNode)  ? fOutputNode : fGainNode;
   }

   return retval;
}



/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Scumbler.cpp"
#endif
