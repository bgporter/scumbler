// Copyright (c) 2013 Bg Porter

#include "Input.h"

const float kPiOver2 = (2 * atan(1));

InputProcessor::InputProcessor(Track* track, int channelCount)
:  GainProcessor(track, channelCount)
,  fIsActive(false)
,  fActiveState(kInactive)
,  fEnabledChannels(tk::kStereo)
{
   // default to center pan.
   this->SetPan(0.5f);

}

InputProcessor::~InputProcessor()
{

}

void InputProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
   ActiveState fadeState;
   bool panRequired;
   float pan[2];
   
   if (1)
   {
      // restricted scope for the mutex so we can get our member vars safely
      // and then release the lock.
      const ScopedLock sl(fMutex);
      fadeState = fActiveState;
      panRequired = fPanRequired;
      
      float gain = fGain * (fadeState == kInactive ? 0.f : 1.f);
      pan[0] = fPanGain[0] * gain;
      pan[1] = fPanGain[1] * gain;
      // set up for the next buffer if we're fading in or out...
      if (kFadeOut == fadeState)
      {
         fActiveState = kInactive;
      }
      else if (kFadeIn == fadeState)
      {
         fActiveState = kActive;
      }
   }

   float panEnd[2];
   switch (fadeState)
   {
      case kInactive:
      case kActive:
      {
         // gain constant throughout buffer
         panEnd[0] = pan[0];
         panEnd[1] = pan[1];
      }
      break;

      case kFadeIn:
      {
         panEnd[0] = pan[0];
         pan[0] = 0.f;
         panEnd[1] = pan[1];
         pan[1] = 0.f;
      }
      break;

      case kFadeOut:
      {
         panEnd[0] = panEnd[1] = 0.f;
      }
      break;

   }

   int sampleCount = buffer.getNumSamples();
   // do we need to copy data to go from mono->stereo?
   if (panRequired)
   {
      // if there's one input channel, it's zero and we pan to the other output
      int sourceChannel = 0;
      int destChannel = 1;
      if (2 == fInputChannelCount)
      {
         if (tk::kRightChannel == fEnabledChannels)
         {
            sourceChannel = 1;
            destChannel = 0;
         }
      }

      // 1. Copy the source input data to the destination channel, applying the pan/gain
      if (pan[destChannel] == panEnd[destChannel])
      {
         buffer.copyFrom(destChannel, 0, buffer.getSampleData(sourceChannel), 
            sampleCount, pan[destChannel]);
      }
      else
      {
         buffer.copyFromWithRamp(destChannel, 0, buffer.getSampleData(sourceChannel), 
            sampleCount, pan[destChannel], panEnd[destChannel]);
      }
      // 2. Apply the pan/gain to the first channel.
      if (pan[sourceChannel] == panEnd[sourceChannel])
      {
         buffer.applyGain(sourceChannel, 0, sampleCount, pan[sourceChannel]);
      }
      else 
      {
         buffer.applyGainRamp(sourceChannel, 0, sampleCount, pan[sourceChannel], panEnd[sourceChannel]);
      }
      
   }
   else
   {
      if (pan[0] == panEnd[0])
      {
         buffer.applyGain(0, 0, sampleCount, pan[0]);
      }
      else
      {
         buffer.applyGainRamp(0, 0, sampleCount, pan[0], panEnd[0]);
      }
      if (fOutputChannelCount == 2)
      {
         if (pan[1] == panEnd[1])
         {
            buffer.applyGain(1, 0, sampleCount, pan[1]);
         }
         else
         {
            buffer.applyGainRamp(1, 0, sampleCount, pan[1], panEnd[1]);
         }
      }
   }
}

void InputProcessor::SetActive(bool isActive)
{
   const ScopedLock sl(fMutex);
   // we can ignore this if nothing's changing.
   if (fIsActive != isActive)
   {
      // handle the next block of audio correctly.
      fActiveState = isActive ? kFadeIn : kFadeOut;
      fIsActive = isActive;
   }
}

bool InputProcessor::IsActive() const
{
   const ScopedLock sl(fMutex);
   return fIsActive;

}

InputProcessor::ActiveState InputProcessor::GetActiveState() const
{
   const ScopedLock sl(fMutex);
   return fActiveState;
}

void InputProcessor::SetPan(float pan)
{

   bool panRequired = false;
    

   if (1 == fInputChannelCount)
   {
      if (2 == fOutputChannelCount)
      {
         panRequired = true;
      }
   }
   else // 2 input channels.
   {
      if ((2 == fOutputChannelCount) && (fEnabledChannels < tk::kStereo))
      {
         panRequired = true;
      }
   }


   if (panRequired)
   {
      // we get this pan definition from the MIDI 'Default Pan formula'
      // http://www.midi.org/techspecs/rp36.php
      // It's down 3dB at the center and increases to full scale as you go to either
      // hard pan position.
      pan = mMax(0, mMin(pan, 1.0));
      pan *= kPiOver2;
      const ScopedLock sl(fMutex);  
      fPanRequired = panRequired;
      fPanGain[0] = cos(pan);
      fPanGain[1] = sin(pan);
       
       
   }
   else
   {
      // pass through with no panning.
      const ScopedLock sl(fMutex);  
      fPanRequired = panRequired;
      fPanGain[0] = fPanGain[1] = 1.0f;      
   }
}


void InputProcessor::SetEnabledChannels(tk::ChannelEnable channels)
{
   const ScopedLock sl(fMutex);
   fEnabledChannels = channels;

}

tk::ChannelEnable InputProcessor::GetEnabledChannels() const
{
   const ScopedLock sl(fMutex);
   return fEnabledChannels;
}

