// Copyright (c) 2014 Bg Porter

#include "LoopThumbnail.h"

#define kFullScale 0.9f

ThumbnailPoint::ThumbnailPoint(float top_,  float bottom_)
:  top(top_)
,  bottom(bottom_)
{
 this->SetNonZero();
}

ThumbnailPoint::~ThumbnailPoint()
{
  // empty.
}

ThumbnailPoint::ThumbnailPoint(const ThumbnailPoint& other)
:  top(other.top)
,  bottom(other.bottom)
,  nonZero(other.nonZero)
{
  // empty.  
}


void ThumbnailPoint::swap(ThumbnailPoint& first, ThumbnailPoint& second)
{
    using std::swap;
    
    swap(first.top, second.top);
    swap(first.bottom, second.bottom);
    swap(first.nonZero, second.nonZero);
}

/**
* See http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
* for an explanation of why we take this parm by val and not by ref.
*/
ThumbnailPoint& ThumbnailPoint::operator=(ThumbnailPoint rhs)
{
  swap(*this, rhs);
  return *this;
}

 void ThumbnailPoint::Set(float top_, float bottom_)
 {
    top = top_;
    bottom = bottom_;
    this->SetNonZero();
 }

class WavePath
{
public:
   WavePath(int width, float height)
   :  fAxis(height/2.0)
   ,  fWidth(width-1)
   ,  fInWave(false)
   {

   }

   ~WavePath()
   {

   }

   void AddPoint(int xPos, const ThumbnailPoint& p)
   {
      if (!fInWave)
      {
         // are we starting to draw a waveform?
         if (p.nonZero)
         {
            fTop.startNewSubPath(xPos, fAxis);
            fBottom.startNewSubPath(xPos, fAxis);
            fInWave = true;
         }
      }
      if (fInWave)
      {
         fTop.lineTo(xPos, p.top);
         fBottom.lineTo(xPos, p.bottom);

         if (! p.nonZero)
         {
            // we've gone back to zero, so close this subpath out.
            fTop.lineTo(xPos, fAxis);
            fBottom.lineTo(xPos, fAxis);
            fTop.closeSubPath();
            fBottom.closeSubPath();
            fInWave = false;
         }
      }
   }

   void ClosePath()
   {
      if (fInWave)
      {
         fTop.lineTo(fWidth, fAxis);
         fBottom.lineTo(fWidth, fAxis);
         fTop.closeSubPath();
         fBottom.closeSubPath();
         fInWave = false;
      }
   }

   void Draw(Graphics& g)
   {
      g.fillPath(fTop);
      g.fillPath(fBottom);
   }

private:
   Path fTop;
   Path fBottom;

   float fAxis;
   int fWidth;

   bool fInWave;

};



////////////////////////////////

LoopThumbnail::LoopThumbnail(LoopProcessor* loop)
:  fLoop(nullptr)
,  fStart(0.f)
,  fSamplesPerPixel(1.0f)
,  fChannelCount(2)
,  fFullRefresh(true)
,  fWidth(0)
,  fHeight(0)
{
    this->ConnectToLoop(loop);
}

LoopThumbnail::~LoopThumbnail()
{

}

void LoopThumbnail::ConnectToLoop(LoopProcessor* loop)
{
   fLoop = loop;
   if (fLoop)
   {
      fChannelCount = fLoop->GetOutputChannelCount();
      this->Resize(fWidth, fHeight);   
   }

}

void LoopThumbnail::Reset()
{

}

void LoopThumbnail::Resize(int width, int height)
{
   if (width > 0 && height > 0)
   {
     fWidth = width;
     fHeight = height;
     fYAxis = fHeight / 2.0f;
     fFullScaleHeight = fYAxis * kFullScale;

     fPoints.resize(fChannelCount * fWidth);

     if (fLoop)
     {
        LoopProcessor::LoopInfo info;
        fLoop->GetLoopInfo(info);
        fSamplesPerPixel = 1.0f * info.fLoopLength / fWidth;
     }

     this->FullRefresh();
     this->Update();
   }

}

int LoopThumbnail::PixelForSample(int sampleNum) const
{
   return static_cast<int>(sampleNum / fSamplesPerPixel);
}


int LoopThumbnail::NowPixel() const
{
   return this->PixelForSample(fNowSample);
}

void LoopThumbnail::SetPixel(int channel, int index, float value)
{
   float scaledValue = value * fFullScaleHeight;
   ThumbnailPoint pt(fYAxis - scaledValue, fYAxis + scaledValue);
   fPoints.set(index*fChannelCount+channel, pt);
}


ThumbnailPoint LoopThumbnail::GetPoint(int channel, int index) const
{
   return fPoints[index * fChannelCount + channel];
}

void LoopThumbnail::FullRefresh()
{
  fStart = 0;
  fFullRefresh = true;
}

void LoopThumbnail::Update()
{
   float accum = fStart;
   int startSample = static_cast<int>(accum);
   LoopProcessor::LoopInfo info;
   fLoop->GetLoopInfo(info);   
   // the last sample we can look at depending on whether we're getting the whole 
   // buffer or just what's arrived since the last time we updated...
   int endSample = fFullRefresh ? info.fLoopLength : info.fLoopSample;
   int pixelsAvailable = static_cast<int>((endSample - startSample) / fSamplesPerPixel);
   int startPixel = this->PixelForSample(startSample);
   // we need at least a single pixel's worth of samples befor we do anything.
   if (pixelsAvailable >= 1)
   {
      float pixelVal;
      for (int pixelNum = 0; pixelNum < pixelsAvailable; ++pixelNum)
      {
         accum += fSamplesPerPixel;
         int lastSample = static_cast<int>(accum);
         lastSample = mMin(lastSample, info.fLoopLength);
         for (int channel = 0; channel < fChannelCount; ++channel)
         {
            pixelVal = fLoop->GetThumbnailPoint(channel, startSample, lastSample);
            this->SetPixel(channel, startPixel+pixelNum, pixelVal);

         }
         // move along to the enxt block of data...
         startSample = lastSample;
      }
      // ..and save our position for the next update.
      fStart = accum;
   }
   // if we hit the end of the loop, wrap for the next time!
   if (fStart + 0.5 > info.fLoopLength)
   {
      fStart = 0.f;
   }
   fFullRefresh = false;
   fNowSample = info.fLoopSample;
}

void LoopThumbnail::Draw(Graphics& g, Colour leftColor, Colour rightColor)
{
   WavePath left(fWidth, fHeight);
   WavePath right(fWidth, fHeight);

   for (int x = 0; x < fWidth; ++x)
   {
      left.AddPoint(x, this->GetPoint(0, x));
      right.AddPoint(x, this->GetPoint(1, x));
   }

   g.setColour(leftColor);
   left.Draw(g);

   g.setColour(rightColor);
   right.Draw(g);

}