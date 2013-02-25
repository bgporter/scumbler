// Copyright (c) 2013 Bg Porter

#ifndef h_Gain
#define h_Gain

#include "Passthrough.h"

class Track;

class GainProcessor : public PassthroughProcessor
                    , public ChangeBroadcaster
{
public:
   /**
    * Create the gain processor, which just multiplies all the samples 
    * that pass through it by a floating point gain value. 
    *
    * @param track If a pointer to a Track object is passed in, this 
    *              processor will also obey the solo and mute states
    *              of the track to possibly set the gain value to 
    *              zero as approriate.
    */
   GainProcessor(Track* track=nullptr, int channelCount=1);

   ~GainProcessor();

   /**
    * Multiply all the samples in this buffer by our current gain value.
    * If we are connected to a Track object, check its mute & solo states
    * to decide what to do -- temporarily overriding our gain to zero if the 
    * track is muted, or if some other track is being soloed. We check those 
    * track settings every time we process a block of audio.
    */
   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   /**
    * If there's no input to this processor, the output will be silence.
    * @return True (always)
    */
   bool silenceInProducesSilenceOut() const { return true; };

   /**
    * Change the current gain value.
    * @param  gain Floating point value, 0.0.. 1.0 (typically -- we don't range check this.)
    * @return      Success/fail.
    */
   tk::Result SetGain(float gain);

   /**
    * Return the current gain setting for this processor.
    * @return float gain value.
    */
   float GetGain() const;


private:

   /**
    * our (optional) track object. We don't own it, so don't delete it.
    */
   Track* fTrack;

   /**
    * 0.0 .. 1.0. Defaults to 1.0/unity gain.
    */
   float fGain;

   /**
    * Protect our data. 
    */
   CriticalSection   fMutex;

};

#endif // this must be the last line in this file.