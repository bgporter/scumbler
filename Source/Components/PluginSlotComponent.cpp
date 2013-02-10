// Copyright (c) 2013 Bg Porter

#include "PluginSlotComponent.h"


extern KnownPluginList gKnownPlugins;

PluginSlotComponent::PluginSlotComponent(PluginBlock* block, int index)
:  fPluginBlock(block)
,  fIndex(index)
,  fMouseOver(false)
,  fPluginName(String::empty)
{
   this->setTooltip("(empty)");
   Random r;
   this->SetEditorPosition(r.nextInt(200), r.nextInt(200));
}


PluginSlotComponent::~PluginSlotComponent()
{

}

void PluginSlotComponent::SetEditorPosition(int x, int y)
{
   fPluginEditorLocation.setXY(x, y);
}

Point<int> PluginSlotComponent::GetEditorPosition() const
{
   return fPluginEditorLocation;
}

void PluginSlotComponent::ShowEditor(bool display)
{
   if (!display)
   {
      // just delete the editor window. 
      fEditor = nullptr;
   }
   else
   {
      // either restore or show the editor window.
      if (nullptr != fEditor)
      {
         // restore the window.
         fEditor->setMinimised(false);
      }
      else
      {
         // create the window.
         AudioProcessorEditor* editor = fPluginBlock->GetEditorForIndex(fIndex, false);
         if (nullptr != editor)
         {
            editor->setName(fPluginName);
            fEditor = new PluginEditorWindow(editor, this);
         }
         else
         {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error Loading Editor",
               "Can't load the editor for " + fPluginName);            
         }
      }
   }
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
  
   PluginInfo info = fPluginBlock->PluginInSlot(fIndex);
   if (String::empty != info.name)
   {
      g.setColour(Colours::black);
      g.drawText(info.name, 2, 0, 
         this->getWidth() - 2, this->getHeight(), 
         Justification::horizontallyCentred | Justification::verticallyCentred, true);
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
            if (tk::kSuccess == fPluginBlock->LoadPluginAtIndex(fIndex, *pd, errorMsg))
            {
               this->setTooltip(pd->manufacturerName + ": " + pd->name);
               fPluginName = pd->name;
            }
            else
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
         m.addItem(kEdit, "Show plugin editor");

         const int r = m.show();
         switch (r)
         {
            case kDelete:
            {
               // if there's an editor window open for this plugin, close it.
               fEditor = nullptr;
               // Delete the filter we have loaded.
               fPluginBlock->RemovePluginAtIndex(fIndex, true);
               this->setTooltip("(empty)");
            }
            break;
            
            case kEdit:
            {
               this->ShowEditor();
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


void PluginSlotComponent::mouseDoubleClick(const MouseEvent& e)
{
   if (! this->IsEmpty())
   {
      this->ShowEditor();
   }
}
