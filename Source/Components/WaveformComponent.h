// Copyright (c) 2013 Bg Porter

// !!! License

#ifndef h_WaveformComponent
#define h_WaveformComponent

#include "../JuceLibraryCode/JuceHeader.h"


class LoopProcessor;
class Track;

class WaveformComponent :  public Component
                        ,  public ChangeListener   
{
public:
   WaveformComponent(LoopProcessor* loop);

   ~WaveformComponent();

   /**
    * Connect at runtime to a specific loop processor. This is useful because 
    * a particular TrackComponent (and its children) may be connected to several different 
    * Track objects over its lifetime.
    * @param loop Pointer to the LoopProcessor object that holds the samples we 
    * need to display. If nullptr, we disconnect.
    */
   void ConnectToLoop(LoopProcessor* loop);

   /**
    * Called by the loop processor when we need to know that something has changed.
    * @param source [description]
    */
   void changeListenerCallback(ChangeBroadcaster* source);

   /**
    * Called by JUCE when our size changes.
    */
   void resized();

   /**
    * Called by JUCE when we need to paint ourself.
    * @param g Graphics object to use to draw.
    */
   void paint(Graphics& g);


private:
   LoopProcessor* fLoop;

};

#endif