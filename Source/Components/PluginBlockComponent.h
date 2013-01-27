// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginBlockComponent
#define h_PluginBlockComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"

class PluginBlockComponent : public Component 
                           , public ChangeListener
{
public:
   PluginBlockComponent(PluginBlock* plugins=nullptr);
   ~PluginBlockComponent();

   void paint (Graphics& g);
   void resized();   

   /**
    * Wire this component up to the plugin block that we will be representing
    * on the screen. This may change at runtime. Pass in `nullptr` to remove 
    * all connections.
    * @param plugins The PluginBlock object that will be updating us.
    */
   void ConnectToPluginBlock(PluginBlock* plugins);

   /**
    * Called when something we're watching calls us back with a notification.
    */
   void changeListenerCallback(ChangeBroadcaster* source);


private:
   PluginBlock* fPlugins;
   
};



#endif // this must the be last line in this file.