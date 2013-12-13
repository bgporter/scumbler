// Copyright (c) 2013 Bg Porter

#include "WaveformComponent.h"

#include "Track.h"

#define kPixelsPerRedraw 5

// turn on/off log statements in the paint() loop.
//#define qLogPaint

WaveformComponent::WaveformComponent(LoopProcessor* loop)
:  fLoop(nullptr)
,  fPendingSamples(0)
,  fRedrawAfterSampleCount(0)
,  fDirtyStart(INT_MAX)
,  fDirtyPixels(0)
,  fNow(0)
{
   // this should make redraws from things like the component being obscured & 
   // then revealed much faster -- we'll only execute the paint() method when 
   // an explicit call to repaint() is made.
   // 
   this->setBufferedToImage(true);
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
         if (!fThumbData || (fThumbData->fChannelCount != fLoop->GetInputChannelCount()))
         {
            // we're using a scoped pointer, so this will delete an old one if 
            // it's there.
            fThumbData = new LoopProcessor::ThumbnailData(fLoop->GetInputChannelCount());
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
#ifdef qLogPaint
            Logger::outputDebugString("LOOP RESET");
#endif   
            // loop was just reset & cleared..
            fDirtyStart = INT_MAX;
            fDirtyPixels = 0;
            fNow = 0;
            fPendingSamples = 0;
            fThumbData->fStart = 0;
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
               fNow = info.fLoopSample;
               int startIndex = jmax(0, fDirtyStart-2);
               // redraw just the region around our newly updated pixels.
               this->repaint(startIndex, 0, fDirtyPixels + 4, this->getHeight());
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
#ifdef qLogPaint
   String s("RESIZED:");
   s << this->getWidth() << "x" << this->getHeight();
   Logger::outputDebugString(s);
#endif   

   int width = this->getWidth();
   fPixels.resize(width);

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

   // recalculate where the tick markers need to go.
   fTicks.clear();
   for (int i = 0; i < info.fLoopLength; i += info.fSampleRate)
   {
      int pixel = this->PixelForSample(i);
      fTicks.add(pixel);
   }
   fTicks.add(this->getWidth() -1);
}

void WaveformComponent::Clear()
{
   // !!! mark the entire display as needing refresh.
#ifdef qLogPaint
   Logger::outputDebugString("CLEAR!");
#endif   
   fThumbData->fMaxThumbnailValues = this->getWidth();
   float oldStart = fThumbData->fStart;
   fThumbData->fStart = 0;
   this->GetThumbnailData();
   // the next time we receive samples, they'll come right after the current
   // loop position.
   fThumbData->fStart = oldStart;

   this->repaint();
}


void WaveformComponent::LoopSizeChanged()
{
#ifdef qLogPaint
   Logger::outputDebugString("LoopSizeChanged!");
#endif   
   // set the scaling factor between the sample buffer and this view
   this->CalculateSamplesPerPixel();
   // ...and make sure we recalc/redraw everything after a full reset
   fPendingSamples = 0;
   fDirtyStart = INT_MAX;
   fDirtyPixels = 0;
   fThumbData->fStart = 0;
   fNow = 0;
   this->Clear();
}


int WaveformComponent::PixelForSample(int sampleNum)
{
   return static_cast<int>(sampleNum / fThumbData->fSamplesPerPixel);
}


void WaveformComponent::GetThumbnailData()
{
   // remember the first sample we're interested in, because the value
   // is overwritten after we fetch thumbnail data
   int startSample = static_cast<int>(fThumbData->fStart);
   // ...and use that sample to know where the returned data is going to need 
   // to go for display.
   int pixelIndex = this->PixelForSample(startSample);
   // if this is the lowest-numbered pixel that we've seen since we were last
   // displayed, it's the start of our new dirty region that will need to 
   // be displayed.
#ifdef qLogPaint
   String s("GTD: fDirtyStart was ");
   s << fDirtyStart;
#endif   
   fDirtyStart = jmin(fDirtyStart, pixelIndex);

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
#ifdef qLogPaint
   s << " is now " <<  fDirtyStart << " fDirtyPixels = " << fDirtyPixels; 
   Logger::outputDebugString(s);
#endif   

}

void WaveformComponent::paint(Graphics& g)
{
   g.fillAll(Colours::white);

   Rectangle<int> clip = g.getClipBounds();
#ifdef qLogPaint
   String c("  PAINT redrawing ");
   c << clip.getX() << ".." << clip.getX() + clip.getWidth();
   Logger::outputDebugString(c); 
#endif   
   g.setColour(Colours::black);
   g.drawLine(0, fCenterYPos, this->getWidth(), fCenterYPos);

   int nowPixel = this->PixelForSample(fNow);
#ifdef qLogPaint
   String s("  PAINT Now = "); 
   s << nowPixel;
   Logger::outputDebugString(s);
#endif   

   int startIndex = clip.getX();
   int endIndex = startIndex + clip.getWidth();
   int height = this->getHeight();

   // draw tick markers.
   g.setColour(Colours::grey);
   for (int tick = 0; tick < fTicks.size(); ++tick)
   {
      int tickPixel = fTicks.getUnchecked(tick);
      int tickLeftX = tickPixel - 4;
      int tickRightX = tickPixel + 4;
      if ((tickRightX >= startIndex) && tickLeftX <= endIndex)
      {
         // top tick
         g.drawLine(tickLeftX, 0, tickRightX, 0, 2);
         g.drawVerticalLine(tickPixel, 0, 8);
         // bottom tick
         g.drawLine(tickLeftX, height, tickRightX, height, 2);
         g.drawVerticalLine(tickPixel, height-9, height);
      }
   }
   g.setColour(Colours::black);

   // draw a line for every waveform deflection from the zero point.
   for (int x = startIndex;  x < endIndex; ++x)
   {
      WaveformPoint p(fPixels[x]);
      if ( (x == nowPixel) || (x == (nowPixel + 1)) || (x == nowPixel - 1) )
      {
         g.setColour(Colours::red);
         g.drawVerticalLine(x, 0, height);
         g.setColour(Colours::white);
         g.drawVerticalLine(x, p.top, p.bottom);
         g.setColour(Colours::black);

      }
      else if (p.top - p.bottom > 1)
      {
         g.drawVerticalLine(x, p.top, p.bottom);
      }
   }
   // there's nothing dirty anymore because we've displayed everything. 
   fDirtyStart = INT_MAX;
   fDirtyPixels = 0;
}