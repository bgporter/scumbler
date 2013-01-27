// Copyright (c) 2013 Bg Porter

#include "PluginBlockComponent.h"

PluginBlockComponent::PluginBlockComponent(PluginBlock* plugins)
:  fPlugins(nullptr)
{
   if (nullptr != plugins)
   {
      this->ConnectToPluginBlock(plugins);
   }

}

PluginBlockComponent::~PluginBlockComponent()
{

}

void PluginBlockComponent::paint (Graphics& g)
{
   g.fillAll(Colours::blue);
}


void PluginBlockComponent::resized()
{
   for (int i = 0; i < fSlots.size(); ++i)
   {
      PluginSlotComponent* slot = fSlots.getUnchecked(i);
      this->SetSlotBounds(i, slot);
   }
}


void PluginBlockComponent::SetSlotBounds(int index, PluginSlotComponent* slot)
{
   int blockWidth = this->getWidth();
   int blockHeight = this->getHeight();
   float slotHeight = blockHeight * 0.8;
   float elementCount = fSlots.size() + 1.0;
   float slotWidth = blockWidth / elementCount;
   float margin = slotWidth / elementCount;

   int x = margin + index * slotWidth;
   int y =  blockHeight * 0.1;
   slot->setBounds(x, y, slotWidth, slotHeight);


}

void PluginBlockComponent::ConnectToPluginBlock(PluginBlock* plugins)
{
   // only do anything if we're changing something.
   if (plugins != fPlugins)
   {
      if (fPlugins)
      {
         // disconnect.
         fPlugins->removeChangeListener(this);
      }
   }
   fPlugins = plugins;

   if (fPlugins)
   {
      // connect to our new source of data.
      fPlugins->addChangeListener(this);
      if (fPlugins->Size() != fSlots.size())
      {
         fSlots.clear();
         for (int i = 0; i < fPlugins->Size(); ++i)
         {
            PluginSlotComponent* slot = new PluginSlotComponent(fPlugins, i);
            fSlots.add(slot);
            this->addAndMakeVisible(slot);
            //this->SetSlotBounds(i, slot);
         }
      }
   }
}

void PluginBlockComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fPlugins)
   {

   }

}