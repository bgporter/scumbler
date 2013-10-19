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

int PluginSlotComponent::GetPreferredWidth() const
{
   return kNoPreferredWidth;
}

bool PluginSlotComponent::IsEmpty() const
{
    return (tk::kInvalidNode == fPluginBlock->PluginInSlot(fIndex).id);
}

void PluginSlotComponent::paint (Graphics& g)
{
   Rectangle<float> rect = Rectangle<float>(0, 0, this->getWidth(), this->getHeight());
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(rect);
#endif

   // we reduce so we're sure that our fat borders are all displayed. 
   rect.reduce(2.0f, 2.0f);
   const float kCornerSize = this->getHeight() / 2;
   if (this->IsEmpty())
   {
      g.setColour(Colours::white);
   }
   else
   {
      g.setColour(Colours::black);
   }

   g.fillRoundedRectangle(rect, kCornerSize);

   Colour c = Colours::black;
   if (fMouseOver)
   {
      c = Colours::red;
   }
   g.setColour(c);
   g.drawRoundedRectangle(rect, kCornerSize, 3.000f);
  
   PluginInfo info = fPluginBlock->PluginInSlot(fIndex);
   if (String::empty != info.name)
   {
      // calculate the largest rectangle we can fit fully inside this
      // rounded rect -- we use the points at the 45 degree point on each arc
      // as the vertexes of this rect.
      const float kSin45 = 0.70710678118654746;
      float inset = kCornerSize - (kCornerSize * kSin45);
      rect += Point<float>(inset, inset);
      inset *= 2;
      rect.setWidth(rect.getWidth() - inset);
      rect.setHeight(rect.getHeight() - inset);

      g.setColour(Colours::white);
      g.drawFittedText(info.name, mRoundInt(rect.getX()), mRoundInt(rect.getY()),
         mRoundInt(rect.getWidth()), mRoundInt(rect.getHeight()), 
         Justification::horizontallyCentred | Justification::verticallyCentred, 2);
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
         // add all of the plugins to the menu
         gKnownPlugins.addToMenu(m, fPluginBlock->GetPluginSortOrder());
         // show the menu. On return, `r` will be the index of the selected item
         const int r = m.show();
         // get a PluginDescription object for the selected plugin
         int pluginIndex = gKnownPlugins.getIndexChosenByMenu(r);
         PluginDescription* pd = gKnownPlugins.getType(pluginIndex);
         if (pd)
         {
            String errorMsg;
            // pass this plugin description down toward the Scumbler object, 
            // which will:
            // - Load it
            // - connect its inputs/outputs correctly 
            // - let us know if that was done successfully.
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
               fPluginName = String::empty;
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
