// Copyright (c) 2013 Bg Porter

#include "WaveformComponent.h"

#include "Track.h"

#define kPixelsPerRedraw 15

WaveformComponent::WaveformComponent(LoopProcessor* loop)
:  fLoop(nullptr)
,  fPendingSamples(0)
,  fRedrawAfterSampleCount(0)
,  fDirtyStart(0)
,  fDirtyPixels(0)
,  fRedrawAll(false)
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
            fDirtyStart = 0;
            fDirtyPixels = 0;
            this->Clear();
         }

         else if (info.fLoopSample != fLoopInfo.fLoopSample)
         {
            // the loop has new samples in it that we need to deal with.
            int newSamples = info.fLoopSample - fLoopInfo.fLoopSample;

            if (newSamples < 0)
            {
               // we've wrapped in the loop -- calculate the number of samples from 
               // the old position to the end of the loop and from the beginning of 
               // the loop to the new position
               newSamples = (info.fLoopLength - fLoopInfo.fLoopSample) +  info.fLoopSample;
            }
            fPendingSamples += newSamples;
            if (fPendingSamples > fRedrawAfterSampleCount)
            {
               fThumbData->fMaxThumbnailValues = fPendingSamples / fThumbData->fSamplesPerPixel;
               // We can find out how many samples' worth of data we got thumbnail data
               // for by comparing fThumbData->fStart before and after calling 
               // GetThumbnailData().
               int startSample = static_cast<int>(fThumbData->fStart);
               this->GetThumbnailData();
               int endSample = static_cast<int>(fThumbData->fStart);
               int samplesDrawn = endSample - startSample;
               if (0 == endSample)
               {
                  // we wrapped, so we're only drawing to the end of the loop buffer.
                  samplesDrawn = info.fLoopLength - startSample;
               }
               else
               {
                  samplesDrawn = endSample - startSample;
               }
               //this->repaint(fDirtyStart, 0, fDirtyPixels, this->getHeight());
               this->repaint();
               this->fPendingSamples -= samplesDrawn;
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
   float spp = 1.0f * info.fLoopLength / this->getWidth();
   fThumbData->fSamplesPerPixel = spp;
   // Change how frequently we redraw by changing the constant here.
   // Yes, this should be bumped out into a named constant or configurable
   // value.
   fRedrawAfterSampleCount = static_cast<int>(kPixelsPerRedraw * spp);
}


void WaveformComponent::Clear()
{
   // !!! mark the entire display as needing refresh.
   fThumbData->fStart = 0.0f;
   fThumbData->fMaxThumbnailValues = this->getWidth();
   this->GetThumbnailData();
   fRedrawAll = true;
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
   fDirtyPixels += fThumbData->fPixelsReturned;

}

void WaveformComponent::paint(Graphics& g)
{
   g.fillAll(Colours::white);

   // testing... -- higlight the current dirty rect.
   Random r;
   g.setFillType(FillType(Colour(r.nextInt())));
   g.fillRect(fDirtyStart, 0, fDirtyPixels, this->getHeight());
   // ...testing
   g.setColour(Colours::black);
   g.drawLine(0, fCenterYPos, this->getWidth(), fCenterYPos);

   int nowPixel = static_cast<int>(fLoopInfo.fLoopSample / fThumbData->fSamplesPerPixel);

   int startIndex;
   int endIndex;
   if (fRedrawAll)
   {
      startIndex = 0;
      endIndex = fPixels.size();
      fRedrawAll = false;

   }
   else
   {
      startIndex = fDirtyStart;
      endIndex = fDirtyStart + fDirtyPixels;
       startIndex = 0;
       endIndex = fPixels.size();
   }

   // draw a line for every waveform deflection from the zero point.
   //for (int x = fDirtyStart, pt=0; pt < fDirtyPixels; ++x, ++pt)
   for (int x = startIndex;  x < endIndex; ++x)
   {
      WaveformPoint p(fPixels[x]);
      if (x == nowPixel)
      {
         g.drawLine(x, 0, x, this->getHeight());
         g.setColour(Colours::white);
         g.drawLine(x, p.top, x, p.bottom);
         g.setColour(Colours::black);

      }
      else if (p.top - p.bottom > 1)
      {
         g.drawLine(x, p.top, x, p.bottom);
      }
   }
   fDirtyStart += fDirtyPixels;
   fDirtyPixels = 0;
   if (fDirtyStart >= this->getWidth())
   {
      fDirtyStart = 0;
   }
}