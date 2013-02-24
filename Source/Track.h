// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Track
#define h_Track

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "Processors/Gain.h"
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


};


#endif // this must be the last line in this file.
