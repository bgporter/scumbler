// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginSlotComponent
#define h_PluginSlotComponent

#include "../JuceLibraryCode/JuceHeader.h"


class PluginSlotComponent : public Component 
{
public:
   PluginSlotComponent(Track* track);
   ~PluginSlotComponent();

   void paint (Graphics& g);
   void resized();   
   void mouseDown (const MouseEvent& e);
};


#endif // this must be the last line in this file.
