// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"
#include "Commands.h"
Scumbler::Scumbler(AudioDeviceManager& deviceManager)
:  fDeviceManager(deviceManager)
,  fPlaying(false)
{

}

Scumbler::~Scumbler()
{
   
}


void Scumbler::Play()
{
   fPlaying = !fPlaying;
}

bool Scumbler::IsPlaying() const
{
   return fPlaying;
}



