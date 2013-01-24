// Copyright (c) 2013 Bg Porter

#include "PluginBlockComponent.h"

PluginBlockComponent::PluginBlockComponent(Track* track)
:  fTrack(track)
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
