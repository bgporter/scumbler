// Copyright (c) 2014 Brett g Porter

#include "StyledComponent.h"


StyledComponent::StyledComponent(UiStyle* style, const String& name)
:  Component(name)
,  fStyle(style)
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

void LogPaint(Component* c, Graphics& g)
{
    String output = c->getName() + " ";
    Rectangle<int> clip = g.getClipBounds();
    
    output << clip.getX() << ", " << clip.getY() << " w = " << clip.getWidth() << " h = " << clip.getHeight();
    
    Logger::outputDebugString(output);
}