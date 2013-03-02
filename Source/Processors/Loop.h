// Copyright (c) 2013 Bg Porter

// !!! License 

#ifndef h_LoopProcessor
#define h_LoopProcessor

#include "Passthrough.h"

class Track;

class LoopProcessor : public PassthroughProcessor
                    , public ChangeBroadcaster
{
public:

   enum
   {

      kMinDuration = 100, /**< a resonable minimum duration */
      kMaxDuration = 20000, /**< an arbitrary maximum duration */

   };

   /**
    * @struct LoopInfo
    *
    * UI components can use this structure to get useful information about 
    * the current state of this processor.
    */
   struct LoopInfo
   {
      double  fSampleRate;  //**< current sample rate
      int     fLoopSample;  //**< index of the *next* sample that will be written to 
      int     fLoopLength;  //**< number of samples in the loop
      int     fLoopCount;   //**< how many times have we played through the loop?
      bool    fIsPlaying;   //**< are we currently playing?
   };

   LoopProcessor(Track* track, int channelCount = 1);

   ~LoopProcessor();

   /**
    * Set the duration of this loop. This may require the reallocation of loop 
    * buffers, and should probably only be permissable if we're not playing.
    * @param milliseconds Duration of the loop in ms.
    */
   tk::Result SetLoopDuration(int milliseconds);

   /**
    * Get the current duration of the loop 
    * @return integer, duration in ms.
    */
   int GetLoopDuration() const;

   /**
    * Change the feedback applied to the loop.
    * @param gain 0..1
    */
   void SetFeedback(float gain);  

   /**
    * Get the current feedback being applied to the loop.
    * @return [description]
    */
   float GetFeedback() const;

   /**
    * Is this loop playing? (true if the track is playing..   .)
    * @return bool.
    */
   bool IsPlaying() const;

   /**
    * Clear and reset the loop, setting all read/write positions back to zero.
    */
   void Reset();

   /**
    * Retrieve useful and current data about the state of our looping so we can 
    * update the UI correctly.
    * @param info LoopInfo struct. Filled on output.
    */
   void GetLoopInfo(LoopInfo& info) const;
   /**
     * @name required overrides of pure virtual functions.
     */
   ///@{
   
   /**
    * get the name of this processor.
    * @return String filled with the name.
    */
   const String getName() const;
 
   /**
    * Called before playback starts to get things ready.
    * @param sampleRate               samples per second.
    * @param estimatedSamplesPerBlock 
    */
   void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

   void releaseResources();

   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   bool silenceInProducesSilenceOut() const;

   // AudioProcessorEditor* createEditor();

   // bool hasEditor() const;

   // int getNumParameters();

   // const String getParameterName(int parameterIndex);

   // float getParameter(int parameterIndex);

   // const String getParameterText(int parameterIndex);

   // void setParameter(int parameterIndex, float newValue);

   // int getNumPrograms();

   // int getCurrentProgram();

   // void setCurrentProgram(int index);

   // const String getProgramName(int index);

   // void changeProgramName(int index, const String& newName);

   // void getStateInformation(juce::MemoryBlock& destData);

   // void setStateInformation(const void* data, int sizeInBytes);


   ///@}



private:

   /**
    * Track that we belong to.
    */
   Track* fTrack;

   /**
    * number of samples per second.
    */
   double fSampleRate;


   /**
    * Loop duration in ms.
    */
   int fLoopDuration;

   /**
    * feedback gain -- we default to 0.9f unity gain. 
    */
   float fFeedback;

   /**
    * buffer to hold our loop data.
    */
   ScopedPointer<AudioSampleBuffer> fLoopBuffer;

   /**
    * Index of the next sample inside the buffer for us to deal with; updated 
    * on each call to processBlock()
    */
   int fLoopPosition;

   /**
    * Number of times we've looped. Updated each time 
    */
   int fLoopCount;

   /**
    * Make access to variables threadsafe.
    */
   CriticalSection fMutex;

};


#endif
