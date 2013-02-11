// Copyright (c) 2013 Bg Porter

// !!! License 

#ifndef h_LoopProcessor
#define h_LoopProcessor

#include "Passthrough.h"

class LoopProcessor : public PassthroughProcessor
{
public:

   enum
   {

      kMinDuration = 100, /**< a resonable minimum duration */
      kMaxDuration = 20000, /**< an arbitrate maximum duration */

   };

   LoopProcessor(int channelCount = 1);

   ~LoopProcessor();

   /**
    * Set the duration of this loop. This may require the reallocation of loop 
    * buffers, and should probably only be permissable if we're not playing.
    * @param milliseconds Duration of the loop in ms.
    */
   void SetLoopDuration(int milliseconds);

   /**
    * Get the current duration of the loop 
    * @return integer, duration in ms.
    */
   int GetLoopDuration() const;

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

   int fLoopPosition;

};


#endif
