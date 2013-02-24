// Copyright (c) 2012 Bg Porter

#include "Track.h"




Track::Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name)
:  fScumbler(owner)
,  fName(name)
,  fPlaying(true)
,  fMuted(false)
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

   // create and insert the gain processor.
   fOutputGain = new GainProcessor(this);
   fVolumeId = fScumbler->AddProcessor(fOutputGain);
   fScumbler->InsertBetween(input, fVolumeId, output);

   // create & insert the loop processor
   fLoop = new LoopProcessor(this);
   fLoopId = fScumbler->AddProcessor(fLoop);
   fScumbler->InsertBetween(input, fLoopId, fVolumeId);


   // create the plugin blocks and hook them in.
   fPreEffects = new PluginBlock(fScumbler, input, fLoopId, fPreEffectCount);
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
   fScumbler->RemoveBetween(input, fLoopId, output, true);
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


void Track::ResetLoop()
{
   fLoop->Reset();
}

void Track::SetOutputVolume(float volumeInDb)
{
   if (volumeInDb != fOutputVolume)
   {
      fOutputVolume = volumeInDb;

      //!!! Send the new gain to the audio processor that actually controls
      // the output.
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