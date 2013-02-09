// Copyright (c) 2013 Bg Porter

#include "PluginSlotComponent.h"


extern KnownPluginList gKnownPlugins;

PluginSlotComponent::PluginSlotComponent(PluginBlock* block, int index)
:  fPluginBlock(block)
,  fIndex(index)
,  fMouseOver(false)
,  fPluginName(String::empty)
{

}


PluginSlotComponent::~PluginSlotComponent()
{

}


bool PluginSlotComponent::IsEmpty() const
{
    return (tk::kInvalidNode == fPluginBlock->PluginInSlot(fIndex).id);
}

void PluginSlotComponent::paint (Graphics& g)
{
   Rectangle<float> rect = Rectangle<float>(0, 0, this->getWidth(), this->getHeight());
   rect.reduce(2.0f, 2.0f);
   const float kCornerSize = this->getHeight() / 2;
   if (!this->IsEmpty())
   {
      g.setColour(Colours::goldenrod);
      g.fillRoundedRectangle(rect, kCornerSize);
   }
   Colour c = Colours::lightgrey;
   if (fMouseOver)
   {
      c = Colours::white;
   }
   g.setColour(c);
   g.drawRoundedRectangle(rect, kCornerSize, 3.000f);
  
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
         enum
         {
            kDelete = 1,
            kEdit

         };
         m.addItem(kDelete, "Delete plugin");

         const int r = m.show();
         switch (r)
         {
            case kDelete:
            {
               // Delete the filter we have loaded.
               fPluginBlock->RemovePluginAtIndex(fIndex, true);
            }
            break;
            default:
            {
               // do nothing...
            }
            break;
         }
      }
   }
}

void PluginSlotComponent::mouseEnter(const MouseEvent& e)
{
   fMouseOver = true;
   this->repaint();
}


void PluginSlotComponent::mouseExit(const MouseEvent& e)
{
   fMouseOver = false;
   this->repaint();
}
