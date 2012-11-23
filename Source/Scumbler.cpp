// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"

Scumbler::Scumbler(AudioDeviceManager& deviceManager)
:  fDeviceManager(deviceManager)
{

}

Scumbler::~Scumbler()
{
   
}

ApplicationCommandTarget* Scumbler::getNextCommandTarget()
{
   return this->findFirstTargetParentComponent();
}


void Scumbler::getAllCommands(Array<CommandID>& commands)
{

}

void Scumbler::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{

}

bool Scumbler::perform(const InvocationInfo& info)
{
  return false; 
}
