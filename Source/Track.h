// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Track
#define h_Track

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
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

   enum ListenTo
   {
      kPreFx = 0,
      kTrack,
      kPostFx
   };
   /**
    * Connect a ChangeListener object to one of the things we own that can send change notifications.
    * @param  add     If true, add the `listener`, otherwise remove it.
    * @param target   One of the `ListenTo` enum items (kPreFx, kTrack, kPostFx)
    * @param listener Pointer to the object we need to send updates to.
    */
   void UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener);

   int GetPreEffectCount() const { return fPreEffectCount; };
   int GetPostEffectCount() const { return fPostEffectCount; };
   


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
    * A block of effects that should be applied before the loop processor.
    */
   int fPreEffectCount;
   ScopedPointer<PluginBlock>  fPreEffects;

   /**
    * node id of the loop.
    */
   NodeId fLoop;

   /**
    * A block of effects that should be applied after the loop processor.
    */
   int fPostEffectCount;
   ScopedPointer<PluginBlock>   fPostEffects;



};


#endif // this must be the last line in this file.
