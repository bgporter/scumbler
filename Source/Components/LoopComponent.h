// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_LoopComponent
#define h_LoopComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "Loop.h"

class LoopComponent  :  public Component 
                     ,  public ChangeListener
{
public:
   LoopComponent(LoopProcessor* loop=nullptr);

   ~LoopComponent();

   /**
    * Wire this component up at runtime to the LoopProcessor we'll be representing on screen.
    * @param loop Pointer to the loop. If NULL, we disconnect.
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

#endif // this must be the last line in this file.
