// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Passthrough
#define h_Passthrough

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../ScumblerDefs.h"

/**
 * @class PassthroughProcessor
 *
 * No-op audio processor that we're reallyl only using for test. You can create an 
 * instance with 1 or 2 input/output channels. Whatever comes in goes out unaffected.
 */
class PassthroughProcessor : public AudioProcessor
{
public:
   /**
    *  @param inputChannelCount number of desired input channels
    *  @param outputChannelCount number of desired output channels.
    */
   PassthroughProcessor(int channelCount = 1);

   /**
    * Destructor.
    */
   ~PassthroughProcessor();

   /**
     * @name required overrides of pure virtual functions.
     */
   ///@{
   
   /**
    * get the name of this processor.
    * @return String filled with the name.
    */
   const String getName() const;

   int GetChannelCount() const { return fChannelCount; };
 
   /**
    * Called before playback starts to get things ready.
    * @param sampleRate               samples per second.
    * @param estimatedSamplesPerBlock 
    */
   void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

   void releaseResources();

   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   const String getInputChannelName(int channelIndex) const;

   const String getOutputChannelName(int channelIndex) const;

   bool isInputChannelStereoPair(int index) const;

   bool isOutputChannelStereoPair(int index) const;

   bool silenceInProducesSilenceOut() const;

   bool acceptsMidi() const;

   bool producesMidi() const;

   AudioProcessorEditor* createEditor();

   bool hasEditor() const;

   int getNumParameters();

   const String getParameterName(int parameterIndex);

   float getParameter(int parameterIndex);

   const String getParameterText(int parameterIndex);

   void setParameter(int parameterIndex, float newValue);

   int getNumPrograms();

   int getCurrentProgram();

   void setCurrentProgram(int index);

   const String getProgramName(int index);

   void changeProgramName(int index, const String& newName);

   void getStateInformation(juce::MemoryBlock& destData);

   void setStateInformation(const void* data, int sizeInBytes);


   ///@}


protected:
    int fChannelCount;
private:
   //JUCE_DECLARE_NON_COPYABLE(PassthroughProcessor);



};



#endif // this must be the last line in this file.
