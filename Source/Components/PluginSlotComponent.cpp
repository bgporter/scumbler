// Copyright (c) 2013 Bg Porter

#include "PluginSlotComponent.h"


PluginSlotComponent::PluginSlotComponent(PluginBlock* block, int index)
:  fPluginBlock(block)
,  fIndex(index)
{

}


PluginSlotComponent::~PluginSlotComponent()
{

}

void PluginSlotComponent::paint (Graphics& g)
{
   Rectangle<int> bounds = this->getBounds();
   if (tk::kInvalidNode == fPluginBlock->NodeInSlot(fIndex))
   {
      g.setColour(Colours::lightgrey);
   }
   else
   {
      g.setColour(Colours::goldenrod);
   }
   g.fillRect(bounds);
   
   g.setColour(Colours::white);
   g.drawRect(bounds, 3);


}

void PluginSlotComponent::resized()
{


}


void PluginSlotComponent::mouseDown(const MouseEvent& e)
{

}