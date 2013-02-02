// Copyright (c) 2013 Bg Porter

#include "PluginListWindow.h"

#include "MainWindow.h"

extern ApplicationProperties* gAppProperties;
extern KnownPluginList  gKnownPlugins;


PluginListWindow::PluginListWindow(MainAppWindow* owner, AudioPluginFormatManager& formatManager)
:  DocumentWindow("Available Plugins", Colours::white, DocumentWindow::closeButton)
,  fOwner(owner)
,  fFormatManager(formatManager)
{
   // logic lifted directly from the JUCE sample app 'Audio Plugin Host', rewritten 
   // to make me happier stylistically.

   PropertiesFile* userSettings = gAppProperties->getUserSettings();
   const File crashFile(userSettings->getFile().getSiblingFile("CrashedPluginsList"));

   this->setContentOwned(new PluginListComponent(fFormatManager,
      gKnownPlugins, crashFile, userSettings), true);

   this->setResizable(true, false);
   this->setResizeLimits(300, 400, 800, 1500);
   this->setTopLeftPosition(60, 60);
   this->restoreWindowStateFromString(userSettings->getValue("listWindowPos"));
   this->setVisible(true);


}


PluginListWindow::~PluginListWindow()
{
   PropertiesFile* userSettings = gAppProperties->getUserSettings();
   userSettings->setValue("listWindowPos", this->getWindowStateAsString());


}


void PluginListWindow::closeButtonPressed()
{
   // delete this window.
   fOwner->ViewPlugins(false);
}