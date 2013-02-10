// Copyright (c) 2013 Bg Porter.

// !!! License


#ifndef h_PluginEditorWindow
#define h_PluginEditorWindow

#include "JuceHeader.h"

class PluginSlotComponent;

class PluginEditorWindow : public DocumentWindow
{
public:
   /**
    * Display the Component that we're given
    *
    * @param ui The editor component to display. This might be the ui that's native to
    *           the plugin, or it may be the JUCE generic UI.
    * @param owner The PluginSlotComponent that owns this window. If we're still 
    *              open when that goes away, it will delete us automatically.
    */
   PluginEditorWindow(Component* const ui, PluginSlotComponent* owner);

   /**
    * Destroy the window & its component.
    */
   ~PluginEditorWindow();

   /**
    * Any time the user moves us around on the screen, tell our owner so that
    * our current location gets persisted to disk when we get saved.
    */
   void moved();

   /**
    * Tell our owner to destroy us.
    */
   void closeButtonPressed();







private:  
   Component* fUiComponent;
   PluginSlotComponent* fOwner; 

};




#endif
