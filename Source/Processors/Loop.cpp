// Copyright (c) 2013 Bg Porter

#include "Loop.h"

#include "Track.h"

#define mMin(x, y) (x) < (y) ? (x) : (y)
#define mMax(x, y) (x) < (y) ? (y) : (x)

LoopProcessor::LoopProcessor(Track* track, int channelCount)
:  PassthroughProcessor(channelCount)
,  fTrack(track)
,  fLoopDuration(4000)
,  fFeedback(0.9f)
,  fLoopBuffer(nullptr)
,  fLoopPosition(0)
{

}

LoopProcessor::~LoopProcessor()
{

}

void LoopProcessor::SetLoopDuration(int milliseconds)
{
   // ensure that our duration falls within an accept20able range
   fLoopDuration = mMax((int)kMinDuration, milliseconds);
   fLoopDuration = mMin(fLoopDuration,  kMaxDuration);

}

int LoopProcessor::GetLoopDuration() const
{
   return fLoopDuration;
}

const String LoopProcessor::getName() const
{
   return "LoopProcessor";
}

void LoopProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
   PassthroughProcessor::prepareToPlay(sampleRate, estimatedSamplesPerBlock);
   // we may need to resize our internal buffers.
   int bufferLength = fLoopDuration * (sampleRate / 1000);
   if (nullptr == fLoopBuffer)
   {
      fLoopBuffer = new AudioSampleBuffer(fChannelCount, bufferLength);

   }
   else if (bufferLength != fLoopBuffer->getNumSamples())
   {
      fLoopBuffer->setSize(fChannelCount, bufferLength);
      fLoopBuffer->clear();

   }

}

void LoopProcessor::releaseResources()
{

}

void LoopProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   if (fTrack->IsPlaying())
   {
      int sampleCount = buffer.getNumSamples();
      int loopSampleCount = fLoopBuffer->getNumSamples();
      float feedbackGain = fFeedback;
      for (int channel = 0; channel < fChannelCount; ++channel)
      {
         // this is easy if we don't need to wrap around the loop buffer 
         // when processing this block
         if (fLoopPosition + sampleCount < loopSampleCount)
         {
            // Add samples from 1 loop ago, multiplying them by the feedback gain.
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, fLoopPosition,
                           sampleCount, feedbackGain);
            // ... and copy the mixed samples back into the loop buffer so we can 
            // play them // back out in one loop's time.
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, channel, 0, sampleCount);

         }
         else
         {
            // first, process as many samples as we can fit in at the end of the loop
            // buffer.
            int roomAtEnd = loopSampleCount - fLoopPosition;
            // and we need to put this many samples back at the beginning.
            int wrapped = sampleCount - roomAtEnd;

            // add samples from a loop ago, adjusting feedback gain.
            // part 1:
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, fLoopPosition,
                            roomAtEnd, feedbackGain);
            // part 2:
            buffer.addFrom(channel, roomAtEnd, *fLoopBuffer, channel, 0,
                            wrapped, feedbackGain);

            // and now copy the mixed samples back into the loop buffer:
            // part 1:
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, channel, 0, roomAtEnd);
            // part 2:
            fLoopBuffer->copyFrom(channel, 0, buffer, channel, roomAtEnd, wrapped);

         }
      }
   
      // set the loop position for the next block of data.
      fLoopPosition = fLoopPosition + sampleCount; 
      if (fLoopPosition >= loopSampleCount)
      {
         fLoopPosition -= loopSampleCount;
      }
   }
}

bool LoopProcessor::silenceInProducesSilenceOut() const
{
   return false;
}
