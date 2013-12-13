// Copyright (c) 2013  Bg Porter

#include "Gain.h"

#include "Track.h"

GainProcessor::GainProcessor(Track* track, int inputChannelCount, int outputChannelCount)
:  PassthroughProcessor(inputChannelCount, outputChannelCount)
,  fTrack(track)
,  fGain(1.0f)
{

}

GainProcessor::~GainProcessor()
{

}

void GainProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   float gain = this->GetGain();
   if (fTrack)
   {
      if (fTrack->IsMuted() || Track::kOtherTrackSoloed == fTrack->IsSoloed())
      {
         gain = 0.0f;
      }

   }

   buffer.applyGain(0, buffer.getNumSamples(), gain);

}

tk::Result GainProcessor::SetGain(float gain)
{
   const ScopedLock sl(fMutex);
   fGain = gain;

   this->sendChangeMessage();
   return tk::kSuccess;
}

float GainProcessor::GetGain() const
{
   const ScopedLock sl(fMutex);

   return fGain;
}