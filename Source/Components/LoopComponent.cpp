// Copyright (c) 2013 Bg Porter

#include "LoopComponent.h"


LoopComponent::LoopComponent(LoopProcessor* loop)
:  fLoop(loop)
{
   
}

LoopComponent::~LoopComponent()
{
   
}

void LoopComponent::ConnectToLoop(LoopProcessor* loop)
{
   if (loop != fLoop)
   {
      if (fLoop)
      {
         fLoop->removeChangeListener(this);
      }
      fLoop = loop;
      if (fLoop)
      {
         fLoop->addChangeListener(this);
      }
   }
   
} 

void LoopComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fLoop)
   {
      this->repaint();
   }
   
}

void LoopComponent::resized()
{
   
}

void LoopComponent::paint(Graphics& g)
{
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif   
}