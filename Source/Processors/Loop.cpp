// Copyright (c) 2013 Bg Porter

#include "Loop.h"

#include <math.h>

#include "Track.h"


#if 0
#define mMin(x, y) (x) < (y) ? (x) : (y)
#define mMax(x, y) (x) < (y) ? (y) : (x)
#endif


LoopProcessor::ThumbnailData::ThumbnailData(int channelCount)
{
   fPixelData.resize(channelCount);
}

void LoopProcessor::ThumbnailData::Resize(int newCapacity)
{
   // for (int channel = 0; channel < fPixelData.size(); ++channel)
   // {
   //    fPixelData[channel].resize(newCapacity);
   // }
   fPixelData.resize(newCapacity);
}

void LoopProcessor::ThumbnailData::SetPixelValue(int channel, int pixelNum, float val)
{
   // DANGER: assuming that all parameters are sane values
   //fPixelData[channel].set(pixelNum, val);
   fPixelData.set(pixelNum, val);

}

LoopProcessor::LoopProcessor(Track* track, int channelCount)
:  PassthroughProcessor(channelCount, channelCount)
,  fTrack(track)
,  fSampleRate(44100.0)
,  fLoopDuration(4000)
,  fFeedback(0.9f)
,  fLoopBuffer(nullptr)
,  fLoopPosition(0)
,  fLoopCount(0)
{

}

LoopProcessor::~LoopProcessor()
{

}

tk::Result LoopProcessor::SetLoopDuration(int milliseconds)
{
   tk::Result retval = tk::kNotWhenPlaying;
   if (!fTrack->IsPlaying())
   {
      // Hey! If this can't be called while we're playing, why are we worried about
      // thread safety? I'm concerned that it's possible to start playback while the 
      // buffer reallocation is in progress and BOOM.
      ScopedLock sl(fMutex);
      // ensure that our duration falls within an accept20able range
      fLoopDuration = mMax((int)kMinDuration, milliseconds);
      fLoopDuration = mMin(fLoopDuration,  kMaxDuration);

      int sampleCount = static_cast<int>(fSampleRate * fLoopDuration / 1000.0);

      if (nullptr == fLoopBuffer)
      {
          fLoopBuffer = new AudioSampleBuffer(fInputChannelCount, sampleCount);
          this->Reset();
      }
      else if (sampleCount != fLoopBuffer->getNumSamples())
      {
         // resizing
         fLoopBuffer->setSize(fInputChannelCount, sampleCount);
         this->Reset();
      }
      retval = tk::kSuccess;

      this->sendChangeMessage();
   }
   return retval;
}

int LoopProcessor::GetLoopDuration() const
{
   ScopedLock sl(fMutex);
   return fLoopDuration;
}


void LoopProcessor::SetFeedback(float gain)
{
   ScopedLock sl(fMutex);
   fFeedback = gain;
   this->sendChangeMessage();

}

float LoopProcessor::GetFeedback() const
{
   ScopedLock sl(fMutex);
   return fFeedback;

}

bool LoopProcessor::IsPlaying() const
{
   bool retval = false;
   if (fTrack)
   {
      retval = fTrack->IsPlaying();
   }
   return retval;
}


void LoopProcessor::GetThumbnailData(ThumbnailData* data)
{
   // !!! Note that all of this logic breaks down when samples per pixel 
   // is < 1.0. It's not clear what we'd want to display in that case anyway
   // at the current moment -- we'll re-evaluate when we get there (and get
   // this code working)

   float accum = data->fStart;
   int startSample = static_cast<int>(accum);
   int samplesAvailable = fLoopBuffer->getNumSamples() - startSample;
   int pixelsDesired = data->fMaxThumbnailValues;
   int pixelsAvailable = 0;
   int samplesDesired = pixelsDesired * data->fSamplesPerPixel;

   if (samplesAvailable < data->fSamplesPerPixel)
   {
      // not enough samples there for us to deal with. This shouldn't happen.
      data->fPixelsReturned = 0;
      // set the thumbnail data to restart at the beginning on the next loop.
      data->fStart = 0;     
      return;
   }
   else if (samplesAvailable < samplesDesired)
   {
      // we can only give them some of the data that they'd like.
      pixelsAvailable = int(samplesAvailable / data->fSamplesPerPixel);
   }
   else
   {
      // typical case -- we can give them all the data that they want.
      pixelsAvailable = pixelsDesired;
   }

   
   // !!! first iteration -- only deal with one channel.
   int channel = 0;
   for (int pixelIndex = 0; pixelIndex < pixelsAvailable; ++pixelIndex)
   {
      accum += data->fSamplesPerPixel;
      int endSample = static_cast<int>(accum);
      endSample = mMin(endSample, fLoopBuffer->getNumSamples());
      // Currently: We look for the highest absolute sample value across *all* samples
      // in the buffer. If we ever want to handle the channels separately, we'd need to 
      // run this loop inside another loop for each of the channels.
      float pixelVal = fLoopBuffer->getMagnitude(startSample, (endSample - startSample));
      data->SetPixelValue(channel, pixelIndex, pixelVal);
      // get ready for the next pixel.
      startSample = endSample;
   }
   // see if we need to wrap on the next call.
   if (static_cast<int>(accum + 0.5) >= fLoopBuffer->getNumSamples())
   {
      accum = 0.0f;
   }
   data->fStart = accum;
   data->fPixelsReturned = pixelsAvailable;
}


void LoopProcessor::Reset()
{
   ScopedLock sl(fMutex);
   fLoopBuffer->clear();
   fLoopPosition = 0;
   fLoopCount = 0;    // ?
   this->sendChangeMessage();
}

void LoopProcessor::GetLoopInfo(LoopInfo& info) const
{
   ScopedLock sl(fMutex);   
   info.fSampleRate = fSampleRate;
   info.fLoopSample = fLoopPosition;
   if (fLoopBuffer)
   {
      info.fLoopLength = fLoopBuffer->getNumSamples();
   }
   else
   {
      info.fLoopLength = 0;
   }
   info.fLoopCount = fLoopCount;
   info.fIsPlaying = fTrack->IsPlaying();
}

const String LoopProcessor::getName() const
{
   return "LoopProcessor";
}

void LoopProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
   PassthroughProcessor::prepareToPlay(sampleRate, estimatedSamplesPerBlock);

   fSampleRate = sampleRate;
   // we may need to resize our internal buffers.
   this->SetLoopDuration(fLoopDuration);

}

void LoopProcessor::releaseResources()
{

}

void LoopProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   if (fTrack->IsPlaying())
   {
      // Lock down all of the protected code sections.
      ScopedLock sl(fMutex);   
      int sampleCount = buffer.getNumSamples();
      int loopSampleCount = fLoopBuffer->getNumSamples();
      float feedbackGain = fFeedback;
      for (int channel = 0; channel < fInputChannelCount; ++channel)
      {
         // this is easy if we don't need to wrap around the loop 
         // buffer when processing this block
         if (fLoopPosition + sampleCount < loopSampleCount)
         {
            // Add samples from 1 loop ago, multiplying them by 
            // the feedback gain.
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, 
               fLoopPosition, sampleCount, feedbackGain);
            // ... and copy the mixed samples back into the loop buffer 
            // so we can play them back out in one loop's time.
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, 
               channel, 0, sampleCount);

         }
         else
         {
            // first, process as many samples as we can fit in at the 
            // end of the loop buffer.
            int roomAtEnd = loopSampleCount - fLoopPosition;
            // and we need to put this many samples back at the 
            // beginning.
            int wrapped = sampleCount - roomAtEnd;

            // add samples from a loop ago, adjusting feedback gain.
            // part 1:
            buffer.addFrom(channel, 0, *fLoopBuffer, channel, 
               fLoopPosition, roomAtEnd, feedbackGain);
            // part 2:
            buffer.addFrom(channel, roomAtEnd, *fLoopBuffer, channel, 
               0, wrapped, feedbackGain);

            // and now copy the mixed samples back into the loop buffer:
            // part 1:
            fLoopBuffer->copyFrom(channel, fLoopPosition, buffer, 
               channel, 0, roomAtEnd);
            // part 2:
            fLoopBuffer->copyFrom(channel, 0, buffer, channel, 
               roomAtEnd, wrapped);
         }
      }
   
      // set the loop position for the next block of data.
      fLoopPosition = fLoopPosition + sampleCount; 
      if (fLoopPosition >= loopSampleCount)
      {
         fLoopPosition -= loopSampleCount;
         ++fLoopCount;
      }
      // Notify anyone who's observing this processor that we've 
      // gotten new sample data.
      this->sendChangeMessage();
   }
}

bool LoopProcessor::silenceInProducesSilenceOut() const
{
   return false;
}
