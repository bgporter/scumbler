// Copyright (c) 2013 Bg Porter

#include "PluginEditorWindow.h"
#include "PluginSlotComponent.h"

PluginEditorWindow::PluginEditorWindow(Component* const ui, PluginSlotComponent* owner)
:  DocumentWindow(ui->getName(), Colours::white, 
      DocumentWindow::minimiseButton | DocumentWindow::closeButton)
,  fUiComponent(ui)
,  fOwner(owner)
{
   this->setSize(400, 300);
   this->setContentOwned(ui, true); 
   this->setTopLeftPosition(fOwner->GetEditorPosition());
   this->setVisible(true);
   this->setAlwaysOnTop(true);

}

PluginEditorWindow::~PluginEditorWindow()
{
   this->clearContentComponent();
}

void PluginEditorWindow::moved()
{
   fOwner->SetEditorPosition(this->getX(), this->getY());
}

void PluginEditorWindow::closeButtonPressed()
{
   // tell our owning component to delete us.
   fOwner->ShowEditor(false);
}