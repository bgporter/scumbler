// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Track
#define h_Track

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "Scumbler.h"



class Track
{
public:
   /**
    * \brief Simple constructor; creates an empty track object.
    *
    * We pass in a (non-const) reference to the Scumbler object that owns us. 
    * A reference both to eliminate the temptation to delete the pointer and 
    * because a track object doesn't make sense absent a Scumbler.
    */
   Track(Scumbler* owner, const String& name=String::empty);

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


private:
   // !!! NOTE that at some point I expect Tracks to be copyable, either with 
   // or without the audio contents of a loop buffer. Not today, though.
   JUCE_DECLARE_NON_COPYABLE(Track);

   // SIMPLE version to start with.
   
   /**
    * The scumbler object that owns us. We do not own this pointer, so 
    * don't delete it.
    */
   Scumbler* fOwner;

   /**
    * We can give each track a name for display in the UI.
    */
   String fName;

   /**
    * node id of a single pre-loop plug-in -- eventually we'll support 
    * multiple slots here
    */
   NodeId fPreLoopPlugin;

   /**
    * node id of the loop.
    */
   NodeId fLoop;

   /**
    * node id of the post-loop plug-in. (again, the plan is to 
    * expand this to support multiple plugins here.)
    */
   NodeId fPostLoopPlugin;


};


#endif // this must be the last line in this file.
