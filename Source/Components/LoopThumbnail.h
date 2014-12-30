// Copyright (c) 2014 Bg Porter

// !!! License 

#ifndef h_LoopThumbnail
#define h_LoopThumbnail

#include "JuceHeader.h"
#include "Loop.h"



class ThumbnailPoint
{
public:
   ThumbnailPoint(float top_ = 0, float bottom_ = 0);

   ~ThumbnailPoint();

   ThumbnailPoint(const ThumbnailPoint& other);


   inline void SetNonZero()
   {
      nonZero = ((bottom - top) >= 2.0);
   }

   void swap(ThumbnailPoint& first, ThumbnailPoint& second);
    
   /**
    * See http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    * for an explanation of why we take this parm by val and not by ref.
    */
   ThumbnailPoint& operator=(ThumbnailPoint rhs);

   void Set(float top_, float bottom_);

public:
  float top;
  float bottom;
  bool  nonZero;  // true if this is a point worth drawing

};


class LoopThumbnail
{
public:
   LoopThumbnail(LoopProcessor* loop=nullptr);

   ~LoopThumbnail();

   /**
    * Connect this thumbnail object to a new/different loop processor. Note 
    * that we don't need to do anything to *disconnect* from a loop if we're already 
    * connected to one.
    *
    *  @param loop the Processor that has our samples.
    *
    */
   void ConnectToLoop(LoopProcessor* loop);

   /**
    * Completely reset the loop back to zero position and clear the cached wave data,
    * also recalculating things like samples per pixel..
    */
   void Reset();

   /**
    * Change the size of the thumbnail data that we'll be calculating.      
    * @param width  New width of the thumbnail (changes the # of samples displayed with a single
    *               thumbnail pixel)
    * @param height New height of the thumbnail (changes our vertical scaling)
    *
    */
   void Resize(int width, int height);

   /**
    * Set a new value for a specified (channel, pixel index) location.
    * @param channel Channel, should be 0 (left) / 1(right).
    * @param index   Pixel index (0.. (width-1))
    * @param value   Sample value (0..1)
    */
   void SetPixel(int channel, int index, float value);


   /**
    * Get a copy of the Thumbnail point at the specified (channel, index) location.
    * @param  channel Channel, 0/1
    * @param  index   pixel index, 0..width-1
    * @return         ThumbnailPoint object.
    */
   ThumbnailPoint GetPoint(int channel, int index) const;


   /**
    * Mark the thumbnail as needing to be completely refreshed instead of just getting 
    * sample data since the last time we were updated.
    */
   void FullRefresh();

   /**
    * Update thumbnail data from the loop processor. Depending on state, this may either 
    * update the entire thumbnail, or just samples that have been received since the last time we
    * updated.
    */
   void Update();

   /**
    * Given the current loop length and display width, calculate which pixel index should
    * be used to display a given sample number.
    * @param  sampleNum Sample index (0.. (loop length - 1))
    * @return           Pixel index, 0..(width-1)
    */
   int PixelForSample(int sampleNum) const;

   /**
    * Get the pixel index (0..width-1) showing where 'now' is. 
    * @return pixel index.
    */
   int NowPixel() const;


   /**
    * Draw the current waveform thumbnails into the given graphics context, setting colors
    * as specified here.
    * @param g     Graphics context.
    * @param left  Color to use for channel 0
    * @param right Color to use for channel 1.
    */
   void Draw(Graphics& g, Colour left, Colour right);

private:
   LoopProcessor* fLoop;

   Array<ThumbnailPoint>  fPoints;

   /**
    * We need our starting position in the loop buffer to be calculated as a float
    * so we can correctly handle non-integer numbers of samples per pixels. We calculate 
    * here in float, but truncate to integer when we're actually accessing sample data.
    */
   float fStart;

   /**
    * How many samples' worth of data are represented with each pixel on screen?
    */
   float fSamplesPerPixel;

   /**
    * Number of channels; always matches the output channel count of the loop that we're
    * displaying.
    */
   int fChannelCount;

   /**
    * When fValid is false, we refresh the entire loop, not just current samples.
    */
   bool fFullRefresh;

   int fWidth;
   int fHeight;
   float fYAxis;
   float fFullScaleHeight;
   int fNowSample;


   CriticalSection fMutex;
};


#endif // this must be the last line in this file.