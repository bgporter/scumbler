// Copyright (c) 2012 Bg Porter

#include "Track.h"

int Track::sTrackCount = 0;




Track::Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name)
:  fScumbler(owner)
,  fDeleteMe(false)
,  fName(name)
,  fPlaying(true)
,  fMuted(false)
,  fInputActive(false)
,  fInputGain(0.f)
,  fPan(0.5)
,  fPreEffectCount(preFxCount)
,  fPreEffects(nullptr)
,  fPostEffectCount(postFxCount)
,  fPostEffects(nullptr)
,  fLoop(nullptr)
,  fLoopId(tk::kInvalidNode)
,  fOutputGain(nullptr)
,  fVolumeId(tk::kInvalidNode)
,  fOutputVolume(0.0)
{
   // we need the input and output nodes that the Scumbler controls.
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);

   // create an insert the input processor 
   fInputProcessor = new InputProcessor(this, 2);
   this->SetInputGain(fInputGain);
   this->SetInputPan(fPan);
   fInputId = fScumbler->AddProcessor(fInputProcessor);

   bool disconnectInputAndOutput = (0 == Track::sTrackCount++);

   fScumbler->InsertBetween(input, fInputId, output, disconnectInputAndOutput);


   // create and insert the gain processor.
   fOutputGain = new GainProcessor(this, 2);
   fVolumeId = fScumbler->AddProcessor(fOutputGain);
   fScumbler->InsertBetween(fInputId, fVolumeId, output);

   // create & insert the loop processor
   fLoop = new LoopProcessor(this, 2);
   fLoopId = fScumbler->AddProcessor(fLoop);
   fScumbler->InsertBetween(fInputId, fLoopId, fVolumeId);


   // create the plugin blocks and hook them in.
   fPreEffects = new PluginBlock(fScumbler, fInputId, fLoopId, fPreEffectCount);
   fPostEffects = new PluginBlock(fScumbler, fLoopId, fVolumeId, fPostEffectCount);

   this->addChangeListener(fScumbler);


}

Track::~Track()
{
   // delete the plugin blocks, and then remove the loop processor from the graph, deleting
   // the loop processor as we exit.
   fPreEffects = nullptr;
   fPostEffects = nullptr;
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);

   

   // remove the loop & delete it
   fScumbler->RemoveBetween(fInputId, fLoopId, fVolumeId, true);
   // remove the output gain node & delete it.
   fScumbler->RemoveBetween(fInputId, fVolumeId, output, true);
   // remove the input processor & delete it.
    
   bool reconnectInputAndOutput = (0 == --Track::sTrackCount);
    
   fScumbler->RemoveBetween(input, fInputId, output, true, reconnectInputAndOutput);

   this->removeChangeListener(fScumbler);
}

void Track::LoadXml(XmlElement* e, StringArray& errors, int formatVersion)
{
   if (e->hasTagName(tag::kTrack))
   {
      this->SetName(e->getStringAttribute(tag::kName, ""));
      this->Mute(e->getBoolAttribute(tag::kMuted));
      this->SetActive(e->getBoolAttribute(tag::kActive, false));
      this->SetInputGain(e->getDoubleAttribute(tag::kInputGain, 0));
      this->SetInputPan(e->getDoubleAttribute(tag::kPan, 0.5));
      this->SetOutputVolume(e->getDoubleAttribute("outputVolume", 0));
      bool isSoloed = e->getBoolAttribute(tag::kSoloed, false);
      if (isSoloed)
      {
         fScumbler->SoloTrack(this);
      }
      int channels = e->getIntAttribute(tag::kChannels, tk::kStereo);


      // handle the pre-effect block.
      XmlElement* pre = e->getChildByName(tag::kPreBlock);
      if (pre)
      {
         fPreEffects->LoadXml(pre, errors, formatVersion);
      }
      else
      {
         errors.add("File format error -- missing <pre> effects block.");
      }

      // restore the loop parameters
      XmlElement* loop = e->getChildByName(tag::kLoop);
      if (loop)
      {
         fLoop->SetLoopDuration(loop->getIntAttribute(tag::kLoopDuration, 4000));
         fLoop->SetFeedback(loop->getDoubleAttribute(tag::kLoopFeedback, 0.9));
         //fLoop->SeekAbsolute(loop->getIntAttribute(tag::kLoopPosition, 0));

      }
      else
      {
         errors.add("File format error -- missing <loop> information");
      }


      // handle the post-effect block
      XmlElement* post = e->getChildByName(tag::kPostBlock);
      if (post)
      {
         fPostEffects->LoadXml(post, errors, formatVersion);
      }
      else
      {
         errors.add("File format error -- missing <post> effects block");
      }
   }
   else
   {
      errors.add("File format error -- missing <track> data");
   }


}


XmlElement* Track::DumpXml(int formatVersion) const
{
   XmlElement* node = new XmlElement(tag::kTrack);
   node->setAttribute(tag::kFileFormat, formatVersion);
   node->setAttribute(tag::kName, fName);
   node->setAttribute(tag::kMuted, fMuted);
   node->setAttribute(tag::kSoloed, (this == fScumbler->GetSoloTrack()));
   //node->setAttribute("active", this->IsActive());
   node->setAttribute(tag::kInputGain, fInputGain);
   node->setAttribute(tag::kPan, fPan);
   node->setAttribute(tag::kOutputVolume, fOutputVolume);
   node->setAttribute(tag::kChannels, static_cast<int>(this->GetEnabledChannels()));

   // store the pre-loop plugins
   XmlElement* preNode = node->createNewChildElement(tag::kPreBlock);
   preNode->addChildElement(fPreEffects->DumpXml(formatVersion));

   // !!! store the loop info
   XmlElement* loopNode = node->createNewChildElement(tag::kLoop);
   loopNode->setAttribute(tag::kLoopDuration, fLoop->GetLoopDuration());
   loopNode->setAttribute(tag::kLoopFeedback, fLoop->GetFeedback());
   LoopProcessor::LoopInfo info;
   fLoop->GetLoopInfo(info);
   loopNode->setAttribute(tag::kLoopPosition, info.fLoopSample);
   
   XmlElement* postNode = node->createNewChildElement(tag::kPostBlock);
   postNode->addChildElement(fPostEffects->DumpXml(formatVersion));


   return node;
}

void Track::SetName(const String& name)
{
   fName = name;
   //std::cout << "Track::SetName->sendChangeMessage" << std::endl;
   fScumbler->SetDirty();
   this->sendChangeMessage();
}

String Track::GetName() const
{
   return fName;
}

bool Track::IsPlaying() const
{
   return fPlaying && fScumbler->IsPlaying();
}


tk::Result Track::Solo(bool soloed)
{
   ScopedLock sl(fMutex);
   Track* track = soloed ? this : nullptr; 
   tk::Result retval = fScumbler->SoloTrack(track);
   //std::cout << "Track::Solo->sendChangeMessage" << std::endl;
   this->sendChangeMessage();

   return retval;
}

Track::SoloState Track::IsSoloed() const
{
   ScopedLock sl(fMutex);
   Track::SoloState retval = Track::kNoTracksSoloed;
   Track* soloTrack = fScumbler->GetSoloTrack();
   if (soloTrack)
   {
      if (this == soloTrack)
      {
         retval = Track::kThisTrackSoloed;
      }
      else
      {
         retval = Track::kOtherTrackSoloed;
      }
   }

   return retval;
}

tk::Result Track::Mute(bool muted)
{
   ScopedLock sl(fMutex);
   if (muted != fMuted)
   {
      fMuted = muted;
      //std::cout << "Track::Mute->sendChangeMessage" << std::endl;
      this->sendChangeMessage();
   }
   return tk::kSuccess;

}

bool Track::IsMuted() const
{
   ScopedLock sl(fMutex);
   return fMuted;
}

tk::Result Track::SetActive(bool isActive)
{
   fInputProcessor->SetActive(isActive);
   //std::cout << "Track::SetActive->sendChangeMessage" << std::endl;
   this->sendChangeMessage();
   if (isActive)
   {
      fScumbler->TrackIsActivating(this);
   }
   return tk::kSuccess;
}

bool Track::IsActive() const
{
   return fInputProcessor->IsActive();
}

void Track::SetInputGain(float gainInDb)
{
   if (gainInDb != fInputGain)
   {
      fInputGain = gainInDb;

      float gain = DbToFloat(fInputGain);
      fInputProcessor->SetGain(gain);

      // update our observers.
      // std::cout << "Track::SetInputGain->sendChangeMessage" << std::endl;
      this->sendChangeMessage();
   }
}

float Track::GetInputGain() const
{
   return fInputGain;
}

tk::Result Track::SetInputPan(float pan)
{
   if (fPan != pan)
   {
      fInputProcessor->SetPan(pan);
      fPan = pan;
      // std::cout << "Track::SetInputPan->sendChangeMessage" << std::endl;
      fScumbler->SetDirty();
      this->sendChangeMessage();
   }
   return tk::kSuccess;

}

float Track::GetInputPan() const
{
   return fPan;
}

void Track::SetEnabledChannels(tk::ChannelEnable channels)
{
   if (this->GetEnabledChannels() != channels)
   {
      fInputProcessor->SetEnabledChannels(channels);
      std::cout << "Track::SetEnabledChannels->sendChangeMessage" << std::endl;
      fScumbler->SetDirty();
      this->sendChangeMessage();
   }
}

tk::ChannelEnable Track::GetEnabledChannels() const
{
   return fInputProcessor->GetEnabledChannels();
}



void Track::ResetLoop()
{
   fLoop->Reset();
}


void Track::SeekAbsolute(int loopPos)
{
   if (fLoop)
   {
      fLoop->SeekAbsolute(loopPos);
   }

}



void Track::SetOutputVolume(float volumeInDb)
{
   if (volumeInDb != fOutputVolume)
   {
      fOutputVolume = volumeInDb;

      float gain = DbToFloat(fOutputVolume);
      fOutputGain->SetGain(gain);

      // update our observers.
      //std::cout << "Track::SetOutputVolume->sendChangeMessage" << std::endl;
      fScumbler->SetDirty();
      this->sendChangeMessage();
   }

}

float Track::GetOutputVolume() const
{
   return fOutputVolume;   
}


void Track::UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener)
{
   ChangeBroadcaster* sender;
   switch (target)
   {
      case kPreFx: sender = fPreEffects; break;

      case kTrack: sender = this; break;

      case kPostFx: sender = fPostEffects; break;

      case kLoop: sender = fLoop; break;

      // assert on an error.
      default: jassert(false); break;
   }

   if (add)
   {
      sender->addChangeListener(listener);
   }
   else
   {
      sender->removeChangeListener(listener);
   }
}


void Track::AskToBeDeleted()
{
   fDeleteMe = true;
   // std::cout << "Track::AskToBeDeleted->sendChangeMessage" << std::endl;
   this->sendChangeMessage();
}


bool Track::CanBeDeleted() const
{
   return fScumbler->GetNumTracks() > 1;
}


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif