// Copyright (c) 2013 Bg Porter

#include "WaveformComponent.h"

#include "Track.h"

WaveformComponent::WaveformComponent(LoopProcessor* loop)
:  fLoop(nullptr)
{
   if (loop)
   {
      this->ConnectToLoop(loop);
   }
}

WaveformComponent::~WaveformComponent()
{

}

void WaveformComponent::ConnectToLoop(LoopProcessor* loop)
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

void WaveformComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   //!!! Not sure how we'll handle this. 

}

void WaveformComponent::resized()
{

}

void WaveformComponent::paint(Graphics& g)
{
   // !!! for now, we'll just draw the bounds and a big X as a placeholder.
   g.setColour(Colours::black);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
   g.drawLine(0, 0, this->getWidth(), this->getHeight());
   g.drawLine(0, this->getHeight(), this->getWidth(), 0);

}