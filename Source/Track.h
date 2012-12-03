// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Track
#define h_Track

#include "../JuceLibraryCode/JuceHeader.h"

#include "Loop.h"
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
   Track(Scumbler& owner);

   /**
    * \brief destructor. 
    */
   ~Track();




private:
   // !!! NOTE that at some point I expect Tracks to be copyable, either with 
   // or without the audio contents of a loop buffer. Not today, though.
   JUCE_DECLARE_NON_COPYABLE(Track);

   // SIMPLE version to start with.
   
   /**
    * node id of a single pre-loop plug-in -- eventually we'll support 
    * multiple slots here
    */
   uint_32 fPreLoopPlugin;

   /**
    * node id of the loop.
    */
   uint32 fLoop;

   /**
    * node id of the post-loop plug-in. (again, the plan is to 
    * expand this to support multiple plugins here.)
    */
   uint32 fPostLoopPlugin;


};


#endif // this must be the last line in this file.
