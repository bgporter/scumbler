// Copyright (c) 2013 Bg Porter

#include "WaveformComponent.h"

#include "Track.h"

#define kPixelsPerRedraw 1

// turn on/off log statements in the paint() loop.
//#define qLogPaint

#if 0
WaveformPointArray::WaveformPointArray(int channels)
:  fChannelCount(channels)
{
   // empty
}

WaveformPointArray::~WaveformPointArray()
{
   // empty
}

void WaveformPointArray::Resize(int width)
{
   fPoints.resize(width * fChannelCount);
}

void WaveformPointArray::SetPoint(int channel, int pixel, WaveformPoint const& point)
{
   fPoints.set(pixel*fChannelCount + channel, point);
}

WaveformPoint WaveformPointArray::GetPoint(int channel, int pixel) const
{
   return fPoints[pixel*fChannelCount + channel];
}

#endif




WaveformComponent::WaveformComponent(UiStyle* style, LoopProcessor* loop, const String& name)
:  StyledComponent(style, name)
,  fLoop(nullptr)
,  fThumb(fLoop)
//,  fPixels(2)
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
   //this->setBufferedToImage(true);
   if (loop)
   {
      this->ConnectToLoop(loop);
   }

   this->UpdateStyle();
}

WaveformComponent::~WaveformComponent()
{
   this->ConnectToLoop(nullptr);
}

void WaveformComponent::UpdateStyle()
{
   fBg = fStyle->GetColor(palette::kLoopBg);
   fFg = fStyle->GetColor(palette::kLoopFg);
   fNowLine = fStyle->GetColor(palette::kLoopNow);
   fMonoWave = fStyle->GetColor(palette::kLoopMonoWave);
   fLeftWave = fStyle->GetColor(palette::kLoopLeftWave);
   fRightWave = fStyle->GetColor(palette::kLoopRightWave);
}

void WaveformComponent::ConnectToLoop(LoopProcessor* loop)
{
   if (loop != fLoop)
   {
      if (fLoop)
      {
         std::cout << "  waveform component " << this << " disconnecting from loop " << fLoop << std::endl;
         fLoop->removeChangeListener(this);
      }
      fLoop = loop;
      if (fLoop)
      {
         std::cout << "  waveform component " << this << " connecting to loop " << fLoop << std::endl;
         fLoop->addChangeListener(this);
         fThumb.ConnectToLoop(fLoop);
         //!!!this->CalculateSamplesPerPixel();
         this->Clear();
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
         else if (info.fWasReset)
         {
            // no the loop size didn't really change, but calling
            // LoopSizeChanged() does everything that we want it to do w/r/t
            // resetting the now pointer to the beginning of the buffer.
            this->LoopSizeChanged();
         }
         else if ((0 == info.fLoopSample) && (0 != fLoopInfo.fLoopSample) &&
            (0 == info.fLoopCount) && (0 != fLoopInfo.fLoopCount))
         {
#ifdef qLogPaint
            Logger::outputDebugString("LOOP RESET");
#endif   
            // loop was just reset & cleared..
            fThumb.Reset();
            this->Clear();
         }

         else if (info.fLoopSample != fLoopInfo.fLoopSample)
         {
            // the loop has new samples in it that we need to deal with.
            fThumb.Update();
         }
         // save this data for the next update.
         fLoopInfo = info;
      }
   }
   else if (source == fStyle)
   {
      this->UpdateStyle();
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
   int height = this->getHeight();
   fCenterYPos = height / 2.0;
   fThumb.Resize(width, height);
   // if either the component size or loop length changed, we need to 
   // also recalculate where the ticks should be drawn.
   LoopProcessor::LoopInfo info;
   fLoop->GetLoopInfo(info);
   fTicks.clear();
   for (int i = 0; i < info.fLoopLength; i += info.fSampleRate)
   {
      int pixel = fThumb.PixelForSample(i);
      fTicks.add(pixel);
   }
   fTicks.add(this->getWidth() -1);

   fThumb.Update();
   this->repaint();
}

#if 0
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
#endif

void WaveformComponent::Clear()
{
   // !!! mark the entire display as needing refresh.
#ifdef qLogPaint
   Logger::outputDebugString("CLEAR!");
#endif   
   fThumb.FullRefresh();
   fThumb.Update();

   this->repaint();
}


void WaveformComponent::LoopSizeChanged()
{
#ifdef qLogPaint
   Logger::outputDebugString("LoopSizeChanged!");
#endif   
   this->resized();
}


#if 0
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
   if (1)
   {
      ScopedLock sl(fMutex);
      for (int i = 0; i < fThumbData->fPixelsReturned; ++i, ++pixelIndex)
      {
         // each value in the fPixelData array is absolute value 0..1 of the 
         // max sample in the pertinent chunk. We need to convert that into a pair
         // of pixel y-values that are symmetrical about the vertical center of this component.
         // Left channel first, 
         float pixelVal = fThumbData->GetPixelValue(0, i);
         float deflection  = fFullScaleHeight * pixelVal;
         fPixels.SetPoint(0, pixelIndex, WaveformPoint(fCenterYPos - deflection, fCenterYPos + deflection));
         // ...then right channel.
         pixelVal = fThumbData->GetPixelValue(1, i);
         deflection  = fFullScaleHeight * pixelVal;
         fPixels.SetPoint(1, pixelIndex, WaveformPoint(fCenterYPos - deflection, fCenterYPos + deflection));
      }
   }
   fDirtyPixels += fThumbData->fPixelsReturned;
#ifdef qLogPaint
   s << " is now " <<  fDirtyStart << " fDirtyPixels = " << fDirtyPixels; 
   Logger::outputDebugString(s);
#endif   

}
#endif

void WaveformComponent::paint(Graphics& g)
{
   LogPaint(this, g);
   g.fillAll(fStyle->GetColor(palette::kLoopBg));

   Rectangle<int> clip = g.getClipBounds();
#ifdef qLogPaint
   String c("  PAINT redrawing ");
   c << clip.getX() << ".." << clip.getX() + clip.getWidth();
   Logger::outputDebugString(c); 
#endif   
   g.setColour(fFg);
   g.drawLine(0, fCenterYPos, this->getWidth(), fCenterYPos);

   int nowPixel = jmax(0, fThumb.PixelForSample(fNow));
#ifdef qLogPaint
   String s("  PAINT Now = "); 
   s << nowPixel;
   Logger::outputDebugString(s);
#endif   

   int startIndex = clip.getX();
   int endIndex = startIndex + clip.getWidth();
   int height = this->getHeight();

   // draw tick markers.
   g.setColour(fStyle->GetColor(palette::kLoopTick));
   for (int tick = 0; tick < fTicks.size(); ++tick)
   {
      int tickPixel = fTicks.getUnchecked(tick);
      int tickLeftX = tickPixel - 4;
      int tickRightX = tickPixel + 4;
      if ((tickRightX >= startIndex) && tickLeftX <= endIndex)
      {
         // top tick
         g.drawLine(tickLeftX, 0, tickRightX, 0, 2);
         //g.drawVerticalLine(tickPixel, 0, 8);
         g.drawVerticalLine(tickPixel, 0, height);
         // bottom tick
         g.drawLine(tickLeftX, height, tickRightX, height, 2);
         //g.drawVerticalLine(tickPixel, height-9, height);
      }
   }


   fThumb.Draw(g, fLeftWave, fRightWave);

   // !!! draw NOW line. 
   g.setColour(fNowLine);
   g.fillRect(fThumb.NowPixel(), 0, 3, height);

}