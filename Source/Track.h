// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Track
#define h_Track

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "Processors/Gain.h"
#include "Processors/Input.h"
#include "Processors/Loop.h"
#include "Processors/Passthrough.h"
#include "Scumbler.h"


class Track : public ChangeBroadcaster
{
public:
   /**
    * \brief Simple constructor; creates an empty track object.
    *
    * We pass in a (non-const) reference to the Scumbler object that owns us. 
    * A reference both to eliminate the temptation to delete the pointer and 
    * because a track object doesn't make sense absent a Scumbler. 
    */
   Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name=String::empty);

   /**
    * \brief destructor. 
    */
   ~Track();

   /**
    * Change the name of this track.
    * @param name new name.
    */
   void SetName(const String& name);

   /**
    * Return the name of this track
    * @return string, track name.
    */
   String GetName() const;

   /**
    * Should this track be playing (i.e. looping audio) right now? 
    * @return [description]
    */
   bool IsPlaying() const;

   /**
    * Set this track as soloed in the Scumbler.
    * @param soloed -- are we turning the solo on or off? Note that if another
    *               track get soloed, this happens automatically.
    * @return success/failure
    */
   tk::Result Solo(bool soloed);

   enum SoloState
   {
      kNoTracksSoloed = 0,
      kOtherTrackSoloed,
      kThisTrackSoloed
   };

   /**
    * Is this track currently being soloed?
    * @return SoloState enum value.
    */
   SoloState IsSoloed() const;

   /**
    * Mute/unmute this track.
    * @param  muted Should we mute or unmute this channel?
    * @return       Success/fail.
    */
   tk::Result Mute(bool muted);

   /**
    * Is this track currently muted?
    * @return bool, current mute state.
    */
   bool IsMuted() const;

   /**
    * Activate/deactivate this track. 
    * NOTE that I'm not sure I've got the atomicity of this action figured out yet;
    * worried that we'll (e.g.) activate 1 and handle a block of samples before we execute
    * the instructions to deactivate 0. I'll deal with that later.
    * @param  isActive is this track supposed to be receiving samples?
    * @return          Success/fail.
    */
   tk::Result SetActive(bool isActive);

   /**
    * Is this track currently set as active?
    * @return true/false. If true, the input processor for this track will pass us samples.
    * 
    */
   bool IsActive() const;

   /**
    * Set the input gain to be applied to this track before going into the pre-effects
    * block. default = 0.0.
    * @param gainInDb gain in dB, probably -96..+6 (?)
    */
   void SetInputGain(float gainInDb);

   /**
    * Return the current gain in dB applied to the input of this track
    * @return Gain value.
    */
   float GetInputGain() const;

   /**
    * Set the pan value to be applied to the input to this track. 
    * NOTE that this only has an effect when the input node is a single channel and
    * the InputProcessor is set to generate a stereo pair.
    * @param  pan 0.0 = fully left, 1.0 == fully right. Default = 0.5
    * @return     success/fail.
    */
   tk::Result SetInputPan(float pan);

   /**
    * Get the pan setting applied to the input of this channel (0 <= pan <= 1)
    * @return float.
    */
   float GetInputPan() const;




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


   /**
    * Reset the contents of the loop. Zero out all samples & return the loop 
    * read/write position to zero.
    */
   void ResetLoop();

    /**
     * Set the track's output volume. 
     * @param volumeInDb dB, probably -96..0
     */
    void SetOutputVolume(float volumeInDb);

    /**
     * Get the current track output volume in dB
     * @return floating point dB value, probably in the range -96.0 .. 0.0
     */
    float GetOutputVolume() const;




   enum ListenTo
   {
      kPreFx = 0,
      kTrack,
      kLoop,
      kPostFx
   };
   /**
    * Connect a ChangeListener object to one of the things we own that can send change notifications.
    * @param  add     If true, add the `listener`, otherwise remove it.
    * @param target   One of the `ListenTo` enum items (kPreFx, kTrack, kPostFx)
    * @param listener Pointer to the object we need to send updates to.
    */
   void UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener);

   /**
    * Return the number of slots in our block of pre-effects.
    * @return count of effects.
    */
   int GetPreEffectCount() const { return fPreEffectCount; };

   /**
    * Get a pointer to the PluginBlock holding our pre-effects.
    * @return pointer.
    */
   PluginBlock* GetPreEffectBlock() const { return fPreEffects; };

   /**
    * Return the number of slots in our block of post-effects.
    * @return count of effects.
    */
   int GetPostEffectCount() const { return fPostEffectCount; };

   /**
    * Get a pointer to the PluginBlock holding our pre-effects.
    * @return pointer.
    */
   PluginBlock* GetPostEffectBlock() const { return fPostEffects; };


   /**
    * Get  pointer to the LoopProcessor for this track.
    * @return pointer.
    */
   LoopProcessor* GetLoop() const { return fLoop; };
   

   /**
    * When the user deletes a track with the UI, we need to tell the 
    * Scumbler that owns us asynchronously to delete us. When we notify
    * it that we want to be deleted, it checks each track to see which 
    * one wants to go away and deletes it.
    * @return Does this track want to be deleted?
    */
   bool WantsToBeDeleted() const { return fDeleteMe; };

   /**
    * Sets the track into a state where the Scumbler can tell that 
    * it should be deleted.
    */
   void AskToBeDeleted();

   /**
    * Returns true if there's more than one track in the scumbler, otherwise
    * false.
    * @return bool.
    */
   bool CanBeDeleted() const;

private:
   // !!! NOTE that at some point I expect Tracks to be copyable, either with 
   // or without the audio contents of a loop buffer. Not today, though.
   JUCE_DECLARE_NON_COPYABLE(Track);

   // SIMPLE version to start with.
   
   /**
    * The scumbler object that owns us. We do not own this pointer, so 
    * don't delete it.
    */
   Scumbler* fScumbler;

   /**
    * Set to true when this track wants the Scumbler to delete it.
    */
   bool fDeleteMe;

   /**
    * We can give each track a name for display in the UI.
    */
   String fName;

   /**
    * Is this track playing (a separate variable here so that eventually we'll
    * be able to stop individual tracks while others keep playing.)
    */
   bool fPlaying;

   /**
    * Is this track currently muted?
    */
   bool fMuted;

   /**
    * Is this track active (should the inputProcessor be passing samples through?)
    */
   bool fInputActive;

   /**
    * dB of gain to apply to the input to this track.
    */
   float fInputGain;


   /**
    * Where should a mono input signal be panned?
    */
   float fPan;

   /**
    * A non-owning pointer to the input processor for this track
    */
   InputProcessor* fInputProcessor;
   
   /**
    * the node Id of the input processor
    */
   NodeId fInputId;

   /**
    * A block of effects that should be applied before the loop processor.
    */
   int fPreEffectCount;
   ScopedPointer<PluginBlock>  fPreEffects;



   /**
    * A block of effects that should be applied after the loop processor.
    */
   int fPostEffectCount;
   ScopedPointer<PluginBlock>   fPostEffects;

   /**
    * A non-owning pointer to the loop processor for this track.
    */
   LoopProcessor* fLoop;

   /**
    * node id of the loop.
    */
   NodeId fLoopId;

   /**
    * A non-owning pointer to the output volume processor for this track.
    */
   GainProcessor*   fOutputGain;
   NodeId fVolumeId;

   /**
    * output volume in dB
    */
   float fOutputVolume;

   CriticalSection fMutex;

   /**
    * The first track to be created and the last one to be deleted need to 
    * be handled differently (so we disconnect input & output when we create the first
    * track, but not the others, and in reverse we only reconnect in/out when we are deleting
    * the final track.)
    */
   static int sTrackCount;
};


#endif // this must be the last line in this file.
