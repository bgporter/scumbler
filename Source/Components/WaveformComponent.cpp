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
         // if we don't have a struct to hold thumbnail data yet, or we do, but 
         // it's got the wrong number of channels for this loop processor, create
         // one that's right.
         if (!fThumbData || (fThumbData->fChannelCount != fLoop->GetChannelCount()))
         {
            // we're using a scoped pointer, so this will delete an old one if 
            // it's there.
            fThumbData = new LoopProcessor::ThumbnailData(fLoop->GetChannelCount());
         }
         this->CalculateSamplesPerPixel();
      }
   }   
}

void WaveformComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fLoop)
   {
      // get information about the current state of the loop
      LoopProcessor::LoopInfo info;
      fLoop->GetLoopInfo(info);
      if (fLoopInfo != info)
      {
         // compare current loop info to our last known loop info to decide how to proceed.
         if (info.fLoopLength != fLoopInfo.fLoopLength)
         {
             // size of the loop has changed; we need to recalculate things.
             this->LoopSizeChanged();
         }
         else if ((0 == info.fLoopSample) && (0 != fLoopInfo.fLoopSample) &&
            (0 == info.fLoopCount) && (0 != fLoopInfo.fLoopCount))
         {
            // loop was just reset & cleared..
            this->Clear();
         }

         else if (info.fLoopSample != fLoopInfo.fLoopSample)
         {
            // the loop has new samples in it that we need to deal with.
            //!!! TEMP -- we wait for a full buffer & then let this code run for 
            //development
            if (1 == info.fLoopCount)
            {
               fThumbData->fStart = 0.f;
               this->GetThumbnailData();
               this->repaint();
            }
         }
         // save this data for the next update.
         fLoopInfo = info;
      }
   }
}

void WaveformComponent::resized()
{
   fPixels.resize(this->getWidth());

   // if we resize, we need to repaint everything.
   this->CalculateSamplesPerPixel();
   // figure out how high a full-scale waveform pixel should go from 
   // the center line
   fCenterYPos = this->getHeight() / 2.0;
   fFullScaleHeight = fCenterYPos * 0.9f;
   LoopProcessor::LoopInfo info;
   if (fLoop)
   {
      fLoop->GetLoopInfo(info);
      if (info.fLoopLength > 0)
      {
         this->Clear();
      }
   }

}

void WaveformComponent::CalculateSamplesPerPixel()
{
   LoopProcessor::LoopInfo info;
   fLoop->GetLoopInfo(info);
   fThumbData->fSamplesPerPixel = 1.0f * info.fLoopLength / this->getWidth();
}


void WaveformComponent::Clear()
{
   // !!! mark the entire display as needing refresh.
   fThumbData->fStart = 0.0f;
   this->GetThumbnailData();
   this->repaint();
}


void WaveformComponent::LoopSizeChanged()
{
   // set the scaling factor between the sample buffer and this view
   this->CalculateSamplesPerPixel();
   // ...and make sure we recalc/redraw everything.
   this->Clear();
}

void WaveformComponent::GetThumbnailData()
{
   // remember the first sample we're interested in, because the value
   // is overwritten after we fetch thumbnail data
   int startSample = static_cast<int>(fThumbData->fStart);
   // ...and use that sample to know where the returned data is going to need 
   // to go for display.
   int pixelIndex = static_cast<int>(startSample / fThumbData->fSamplesPerPixel);
   fThumbData->fMaxThumbnailValues = fPixels.size() - pixelIndex;

   // get the data
   fLoop->GetThumbnailData(fThumbData);

   // ...and update the display buffer.
   for (int i = 0; i < fThumbData->fPixelsReturned; ++i, ++pixelIndex)
   {
      // each value in the fPixelData array is absolute value 0..1 of the 
      // max sample in the pertinent chunk. We need to convert that into a pair
      // of pixel y-values that are symmetrical about the vertical center of this component.
      float pixelVal = fThumbData->fPixelData[i];
      float deflection  = fFullScaleHeight * pixelVal;
      fPixels.set(pixelIndex, WaveformPoint(fCenterYPos + deflection, fCenterYPos - deflection));
   }

}

void WaveformComponent::paint(Graphics& g)
{
   g.fillAll (Colours::white);
   g.setColour(Colours::black);
   g.drawLine(0, fCenterYPos, this->getWidth(), fCenterYPos);

   // draw a line for every waveform deflection from the zero point.
   for (int x = 0; x < this->getWidth(); ++x)
   {
      WaveformPoint p(fPixels[x]);
      if (p.top > p.bottom)
      {
         g.drawLine(x, p.top, x, p.bottom);
      }
   }
}