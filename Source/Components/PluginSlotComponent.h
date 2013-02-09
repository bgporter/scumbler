// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginSlotComponent
#define h_PluginSlotComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"


class PluginSlotComponent  : public Component
{
public:
   PluginSlotComponent(PluginBlock* block, int index);
   ~PluginSlotComponent();

   bool IsEmpty() const;

   void paint (Graphics& g);
   void resized();   
   void mouseDown(const MouseEvent& e);
   void mouseEnter(const MouseEvent& e);
   void mouseExit(const MouseEvent& e);

private:
   PluginBlock*   fPluginBlock;
   int            fIndex;
   bool           fMouseOver;
   String         fPluginName;
};


#endif // this must be the last line in this file.
