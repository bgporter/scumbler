// Copyright (c) 2013 Bg Porter

#include "WaveformComponent.h"

#include "Track.h"

#define kPixelsPerRedraw 1

// turn on/off log statements in the paint() loop.
//#define qLogPaint


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
   fTickLine = fStyle->GetColor(palette::kLoopTick);
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


void WaveformComponent::paint(Graphics& g)
{

   int height = this->getHeight();
   int width = this->getWidth();
   LogPaint(this, g);
   g.fillAll(fBg);

   g.setColour(fFg);
   g.drawLine(0, fCenterYPos, width, fCenterYPos);



   // draw tick markers.
   g.setColour(fTickLine);
   for (int tick = 0; tick < fTicks.size(); ++tick)
   {
      int tickPixel = fTicks.getUnchecked(tick);
      g.drawVerticalLine(tickPixel, 0, height);
   }


   fThumb.Draw(g, fLeftWave, fRightWave);

   g.setColour(fFg);
   g.drawRect(0, 0, width, height, 3.0);

   // !!! draw NOW line. 
   g.setColour(fNowLine);
   g.fillRect(fThumb.NowPixel(), 0, 3, height);

}