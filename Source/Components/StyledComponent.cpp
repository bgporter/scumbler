// Copyright (c) 2014 Brett g Porter

#include "StyledComponent.h"


StyledComponent::StyledComponent(UiStyle* style)
: fStyle(style)
{
   fStyle->addChangeListener(this);
}


StyledComponent::~StyledComponent()
{
   fStyle->removeChangeListener(this);
}

void StyledComponent::changeListenerCallback (ChangeBroadcaster *source)
{
   // re-implemented in derived classes that need to take action when the style changes.
}


void StyledComponent::UpdateStyle()
{
   // no-op implementation to be overidden by derived classes.
}