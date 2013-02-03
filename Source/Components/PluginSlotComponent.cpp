// Copyright (c) 2013 Bg Porter

#include "PluginSlotComponent.h"


extern KnownPluginList gKnownPlugins;

PluginSlotComponent::PluginSlotComponent(PluginBlock* block, int index)
:  fPluginBlock(block)
,  fIndex(index)
{

}


PluginSlotComponent::~PluginSlotComponent()
{

}


bool PluginSlotComponent::IsEmpty() const
{
   return (tk::kInvalidNode == fPluginBlock->NodeInSlot(fIndex));
}

void PluginSlotComponent::paint (Graphics& g)
{
   Rectangle<float> rect = Rectangle<float>(0, 0, this->getWidth(), this->getHeight());
   const float kCornerSize = 18.0f;
   if (this->IsEmpty())
   {
      //g.setColour(Colours::lightgrey);
      g.setColour(Colours::white);
      g.drawRoundedRectangle(rect, kCornerSize, 2);
   }
   else
   {
      g.setColour(Colours::goldenrod);
      g.fillRoundedRectangle(rect, kCornerSize);
   }

   if (0 == fIndex)
   {
      g.setColour(Colours::black);
      g.drawText(String(1.0 * this->getWidth()  / this->getHeight()), 0, 0, 
         this->getWidth(), this->getHeight(), 
         Justification::horizontallyCentred | Justification::verticallyCentred, false);
   }


}

void PluginSlotComponent::resized()
{


}


void PluginSlotComponent::mouseDown(const MouseEvent& e)
{
   // displaying the list of plugins to let the user 
   if (e.mods.isPopupMenu())
   {
      // If we are empty, we need to display the list of available plugins so the user can select one. 
      // If we have a plugin loaded, we need to instead display a memu to let us delete that plug-in. 
      PopupMenu m;

      if (this->IsEmpty())
      {
         gKnownPlugins.addToMenu(m, KnownPluginList::defaultOrder);
         const int r = m.show();
         int pluginIndex = gKnownPlugins.getIndexChosenByMenu(r);
         PluginDescription* pd = gKnownPlugins.getType(pluginIndex);
         if (pd)
         {
            String errorMsg;
            if (tk::kSuccess != fPluginBlock->LoadPluginAtIndex(fIndex, *pd, errorMsg))
            {
               AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error Loading Plugin",
                  errorMsg);
            }
         }

      }
      else
      {
         m.addItem(1, "Delete plugin");

         const int r = m.show();
         if (1 == r)
         {
            // Delete the filter we have loaded.
            fPluginBlock->RemoveNodeAtIndex(fIndex, true);
         }
      }
   }

}