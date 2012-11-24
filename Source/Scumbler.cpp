// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"
#include "Commands.h"
Scumbler::Scumbler(AudioDeviceManager& deviceManager)
: fDeviceManager(deviceManager)
, fPlaying(false)
, fInputNode(-1)
, fOutputNode(-1)
{
   this->Reset();
}

Scumbler::~Scumbler()
{
   
}


void Scumbler::TogglePlay()
{
   if (fPlaying)
   {
      this->Pause();
   }
   else
   {
      this->Play();
   }
}

bool Scumbler::IsPlaying() const
{
   return fPlaying;
}

void Scumbler::Reset()
{
   this->Pause();

   // clear out the processor graph
   fGraph.clear();
   
   // Create and add new input/output processor nodes. 
   AudioProcessorGraph::Node* node;
   
   AudioProcessorGraph::AudioGraphIOProcessor* in = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
   node = fGraph.addNode(in);
   fInputNode = node->nodeId;

   AudioProcessorGraph::AudioGraphIOProcessor* out = 
    new AudioProcessorGraph::AudioGraphIOProcessor(
      AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
   node = fGraph.addNode(out);
   fOutputNode = node->nodeId;


}

void Scumbler::Play()
{
   if (!fPlaying)
   {
      fPlaying = true;
   }
}

void Scumbler::Pause()
{
   if (fPlaying)
   {
      fPlaying = false;
   }
}