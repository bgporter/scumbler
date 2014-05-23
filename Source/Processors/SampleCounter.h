// Copyright (c) 2014 Bg Porter

#ifndef h_SampleCounter
#define h_SampleCounter

#include "Passthrough.h"

class Scumbler;

class SampleCounterProcessor  : public PassthroughProcessor
                              , public ChangeBroadcaster
{
public:
   /**
    * Simple no-op processor that keeps track of how many samples it's processed while
    * the Scumbler that owns it is in a playing state.
    *
    * @param scumbler The Scumbler object that owns us.
    * @param notifyCount The number of new samples to accumulate before notifying listeners
    * @param channelCount the number of channels we support.
    */
   SampleCounterProcessor(Scumbler* scumbler, unsigned int notifyCount, int channelCount = 1);

   ~SampleCounterProcessor();

   /**
    * Return the number of samples that we've 'played' since we were last reset.
    * @return ount of samples.
    */
   uint64 GetSampleCount() const;

   /**
    * Reset the sample count back to zero.
    */
   void Reset();

   /**
    * If the Scumbler that owns us is playing, we add the number of samples in the buffer 
    * to our running count. Occasionally we notify listeners that there's new data.
    * @param buffer       [description]
    * @param midiMessages [description]
    */
   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   /**
    * If there's no input to this processor, the output will be silence.
    * @return True (always)
    */
   bool silenceInProducesSilenceOut() const { return true; };





private:
   /**
    * The Scumbler object that owns us. We don't own it, so don't delete it when 
    * we go away.
    */
   Scumbler* fScumbler;

   /**
    * How many samples have we seen so far?
    */
   uint64 fSamples;

   /**
    * How many new samples must arrive before we notify listeners that 
    * there are more samples?
    */
   unsigned int fNotifyCount;

   /** When fSamples >= fNextNotify, we send a change message to listeners and set
       fNextNotify += fNotifyCount.
   */
   uint64 fNextNotify;

   /**
    * Protect our data. 
    */
   CriticalSection   fMutex;

};

#endif // this MUST be the last line in this file.