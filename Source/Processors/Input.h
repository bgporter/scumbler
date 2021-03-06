// Copyright (c) 2013 Bg Porter

#ifndef h_InputProcessor
#define h_InputProcessor

#include "Gain.h"

class InputProcessor : public GainProcessor
{
public:
   /**
    * Create the input processor object, passing in a pointer to the track that 
    * owns it. If we have more outputs than inputs we also support panning. If
    * inputChannelCount == outputChannelCount, we pass through as is.
    *
    * @param track non-owning pointer to the track object that owns us. 
    * @param channelCount number of input/output channels (default 1)
    */
   InputProcessor(Track* track, int channelCount=1);

   /**
    * Destroy the input processor.
    */
   ~InputProcessor();

   /**
    * Handle a block of samples -- we may pass things on unaffected, mute things
    * (if the track isn't active), pan the input into stereo or change gain.     .
    * @param buffer       Input/output sample buffer
    * @param midiMessages MIDI message buffer (ignored)
    */
   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   /**
    * Let the processor know whether it's active or not.  
    * @param isActive Turn the input on or off.
    */
   void SetActive(bool isActive);

   /**
    * Query the current active state of this processor.      
    * @return bool, is the input active?
    */
   bool IsActive() const;

   /**
    * Set the stereo pan for this input if we have 1 input & 2 output channels.
    * @param pan 0.0 == full left, 1.0 == full right. Default = 0.5. We should 
    *            clip in the range 0 <= pan <= 1.0
    */
   void SetPan(float pan);

   /**
    * Control whether we're looking at left, right, or both inputs.
    * @param channels Enum indicating what this input should be listening to.
    */
   void SetEnabledChannels(tk::ChannelEnable channels);

   /**
    * Get the currently enabled channel(s)
    * @return Enum with the channel settings.
    */
   tk::ChannelEnable GetEnabledChannels() const;


private:
   enum ActiveState
   {
      kInactive = 0,
      kActive,
      kFadeIn,
      kFadeOut
   };

   /**
    * Get the activity state for the next buffer to be processed -- after we
    * change from active to inactive, we do one buffer's worth of fade in/out
    * to prevent pops.
    */
   ActiveState GetActiveState() const;

   /**
    * Recalculate the per-channel gain settings whenever any of 
    * - pan position
    * - channel enable
    *
    * changes. This function assumes that the calling code is holding the 
    * mutex for this object. 
    */
   void CalculateGains();


private:
   /**
    * if we're active, we pass samples along to  our output. 
    */
   bool fIsActive;

   /**
    * save the activity state for the audio processing code.
    */
   ActiveState fActiveState;

   /**
    * Save the pan position (0 == hard left, 1 == hard right)
    */
   float fPan;

   /**
    * When we get a new pan value (0..1) passed to us, we calculate the corresponding
    * left/right gain values once and store them until they change. fPanGain[0]
    * is left, fPanGain[1] is right.
    */
   float fPanGain[2];

   /**
    * Enum indicating whether we're taking input from the left, right, or 
    * both input channels. We only apply pan when we're processing a single
    * input channel and generating stereo output.
    */
    tk::ChannelEnable fEnabledChannels;

   /**
    * Do we need to pan the audio or pass it straight through?
    */
   bool fPanRequired;



};

#endif // this MUST be the last line in this file.
