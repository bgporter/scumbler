// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginBlockComponent
#define h_PluginBlockComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "PluginSlotComponent.h"
#include "TrackComponent.h"

class PluginBlockComponent : public Component 
                           , public ChangeListener
{
public:
   PluginBlockComponent(TrackComponent::PluginColors const& colors, PluginBlock* plugins=nullptr);
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
    * Set the bounds of an individual plugin slot component that we own.    
    * @param index Index of the slot.
    * @param slot  pointer to the slot object.
    */
   void SetSlotBounds(int index, PluginSlotComponent* slot);

   /**
    * Called when something we're watching calls us back with a notification.
    */
   void changeListenerCallback(ChangeBroadcaster* source);


private:
   TrackComponent::PluginColors const&  fColors;
   PluginBlock* fPlugins;
   OwnedArray<PluginSlotComponent>  fSlots;
   
};



#endif // this must the be last line in this file.