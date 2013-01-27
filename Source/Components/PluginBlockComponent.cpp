// Copyright (c) 2013 Bg Porter

#include "PluginBlockComponent.h"

PluginBlockComponent::PluginBlockComponent(PluginBlock* plugins)
:  fPlugins(plugins)
{

}

PluginBlockComponent::~PluginBlockComponent()
{

}

void PluginBlockComponent::paint (Graphics& g)
{
   g.fillAll(Colours::blue);
}


void PluginBlockComponent::resized()
{

}



void PluginBlockComponent::ConnectToPluginBlock(PluginBlock* plugins)
{
   // only do anything if we're changing something.
   if (plugins != fPlugins)
   {
      if (fPlugins)
      {
         // disconnect.
         fPlugins->removeChangeListener(this);
      }
   }
   fPlugins = plugins;

   if (fPlugins)
   {
      // connect to our new source of data.
      fPlugins->addChangeListener(this);
   }
}

void PluginBlockComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fPlugins)
   {

   }

}