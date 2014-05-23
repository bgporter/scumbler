// Copyright (c) 2014  Bg Porter

#include "SampleCounter.h"

#include "Scumbler.h"

SampleCounterProcessor::SampleCounterProcessor(Scumbler* scumbler, 
   unsigned int notifyCount, int channelCount)
:  PassthroughProcessor(channelCount, channelCount) 
,  fScumbler(scumbler)
,  fSamples(0)
,  fNotifyCount(notifyCount)
{
   this->Reset();
}

SampleCounterProcessor::~SampleCounterProcessor()
{

}

uint64 SampleCounterProcessor::GetSampleCount() const
{
   const ScopedLock sl(fMutex);
   return fSamples;
}

void SampleCounterProcessor::Reset()
{
   const ScopedLock sl(fMutex);
   fSamples = 0;
   fNextNotify = fNotifyCount;   
}

void SampleCounterProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   if (fScumbler->IsPlaying())
   {
      const ScopedLock sl(fMutex);
      fSamples += static_cast<unsigned long>(buffer.getNumSamples());
      if (fSamples > fNextNotify)
      {
         fNextNotify += fNotifyCount;
         this->sendChangeMessage();
      }
   }

}
