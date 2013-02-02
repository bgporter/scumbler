// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginListWindow
#define h_PluginListWindow

#include "JuceHeader.h"

class MainAppWindow;

class PluginListWindow : public DocumentWindow
{
public:
   /**
    * Create the plugin list window. Should only be instantiated/owned by the
    * MainAppWindow and have its lifecycle controlled using that class' 
    * `ViewPlugins()` method.
    */
   PluginListWindow(MainAppWindow* owner, AudioPluginFormatManager& formatManager);

   /**
    * Destroy the window.
    */
   ~PluginListWindow();

   /**
    * User wants to close this window.
    */
   void closeButtonPressed();


private:   
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginListWindow);
   
   MainAppWindow* fOwner;

   AudioPluginFormatManager& fFormatManager;

};



#endif