// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginBlockComponent
#define h_PluginBlockComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "Scumbler.h"

class PluginBlockComponent : public Component 
{
public:
   PluginBlockComponent(Track* track);
   ~PluginBlockComponent();

   void paint (Graphics& g);
   void resized();   


private:
   Track* fTrack;
   
};



#endif // this must the be last line in this file.