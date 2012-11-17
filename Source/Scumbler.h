// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Scumbler
#define h_Scumbler

#include "../JuceLibraryCode/JuceHeader.h"

/**
 * \class Scumbler
 * \brief The 'model' class for our MVC architecture in the Scumbler app
 *
 * This class owns/manages all of
 * - the audio IO and the filter graph
 * - the track objects
 * - links together the plugins owned by each track and the filter graph
 * - writing output to file, etc.
 */
class Scumbler : public ChangeBroadcaster
{
public:
   /**
    * Constructor
    * \param deviceManager A reference to an AudioDeviceManager object that's
    *     owned elsewhere in the application. 
    */
   Scumbler(AudioDeviceManager& deviceManager);

   /**
    * dtor.
    */
   ~Scumbler();
   
private:
   JUCE_DECLARE_NON_COPYABLE(Scumbler);

   /**
    * The player object that pushes audio through the filter graph.
    */
   AudioProcessorPlayer fPlayer;

   /**
    * The graph of audio processors that do all the work.
    */
   AudioProcessorGraph  fGraph;

   /**
    * The audio hardware. 
    * In our initial version, we'll just initialize with defaults, and use 
    * the Juce-supplied AudioDeviceSelectorComponent class to let the user tweak 
    * settings.
    */
   AudioDeviceManager&   fDeviceManager;

};


#endif // this must be the final line in this file.