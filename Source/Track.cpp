// Copyright (c) 2012 Bg Porter

#include "Track.h"




Track::Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name)
:  fScumbler(owner)
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
   fScumbler->InsertBetween(input, fInputId, output);


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
   fScumbler->RemoveBetween(input, fInputId, output, true);
}


void Track::SetName(const String& name)
{
   fName = name;
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
   return fScumbler->SoloTrack(track);
   this->sendChangeMessage();
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
   fMuted = muted;
   this->sendChangeMessage();
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
      this->sendChangeMessage();
   }
}

float Track::GetInputGain() const
{
   return fInputGain;
}

tk::Result Track::SetInputPan(float pan)
{
   fInputProcessor->SetPan(pan);
   fPan = pan;
   this->sendChangeMessage();
   return tk::kSuccess;

}

float Track::GetInputPan() const
{
   return fPan;
}

void Track::ResetLoop()
{
   fLoop->Reset();
}

void Track::SetOutputVolume(float volumeInDb)
{
   if (volumeInDb != fOutputVolume)
   {
      fOutputVolume = volumeInDb;

      float gain = DbToFloat(fOutputVolume);
      fOutputGain->SetGain(gain);

      // update our observers.
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


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif