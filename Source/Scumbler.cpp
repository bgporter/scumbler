// Copyright (c) 2012 Bg Porter

#include "Scumbler.h"
#include "Commands.h"
Scumbler::Scumbler(AudioDeviceManager& deviceManager)
: fDeviceManager(deviceManager)
, fPlaying(false)
, fInputNode(-1)
, fOutputNode(-1)
{
   fPlayer.setProcessor(&fGraph);
   fDeviceManager.addAudioCallback(&fPlayer);
   this->Reset();
}

Scumbler::~Scumbler()
{
   fDeviceManager.removeAudioCallback(&fPlayer);
   fPlayer.setProcessor(nullptr);
   fGraph.clear();
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

   this->Connect(fInputNode, fOutputNode);


}


bool Scumbler::Connect(uint32 source, uint32 dest)
{
   bool retval = false;
   AudioProcessorGraph::Node* srcNode = fGraph.getNodeForId(source);
   AudioProcessorGraph::Node* destNode = fGraph.getNodeForId(dest);
   // only proceed if those nodes exist.
   if (srcNode && destNode)
   {
      retval = true;
      AudioProcessor* srcFilter = srcNode->getProcessor();
      int numSrcChannels = srcFilter->getNumOutputChannels();
      AudioProcessor* destFilter = destNode->getProcessor();
      int numDestChannels = destFilter->getNumInputChannels();
      // in our immediate situation, we're only interested in 2 channels at most. 
      // Future versions might be interested in more.
      if (numSrcChannels == numDestChannels)
      {
         for (int index = 0; index < numSrcChannels; ++index)
         {
            fGraph.addConnection(source, index, dest, index);
         }

      }
      else if (numSrcChannels < numDestChannels)
      {
         // connect the single input to both pins of the destination filter
         fGraph.addConnection(source, 0, dest, 0);
         fGraph.addConnection(source, 0, dest, 1);
      }
      else // numSrcChannels > numDestChannels
      {
         // connect both source pins to the single pin of the dest filter.
         fGraph.addConnection(source, 0, dest, 0);
         fGraph.addConnection(source, 1, dest, 0);
      }
   }
   return retval;
}


void Scumbler::Play()
{
   if (!fPlaying)
   {
      fPlaying = true;
      // !!! TODO
   }
}

void Scumbler::Pause()
{
   if (fPlaying)
   {
      fPlaying = false;
      // !!! TODO
   }
}