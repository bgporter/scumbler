// Copyright (c) 2013 Bg Porter

#ifndef h_InputProcessor
#define h_InputProcessor

#include "Gain.h"

class InputProcessor : public GainProcessor
{
public:

   enum ChannelEnable
   {
      kLeftChannel = 0x01,
      kRightChannel = 0x02,
      kStereo = 0x03
   };

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
    * When we get a new pan value (0..1) passed to us, we calculate the corresponding
    * left/right gain values once and store them until they change. fPanGain[0]
    * is left, fPanGain[1] is right.
    */
   float fPanGain[2];



};

#endif // this MUST be the last line in this file.
