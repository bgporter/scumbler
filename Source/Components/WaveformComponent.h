// Copyright (c) 2013 Bg Porter

// !!! License

#ifndef h_WaveformComponent
#define h_WaveformComponent

#include "../JuceLibraryCode/JuceHeader.h"


#include "Loop.h"
#include "LoopThumbnail.h"
#include "StyledComponent.h"
#include "TrackComponent.h"

class Track;


#if 0
class WaveformPointArray
{
public:
   /**
    * Create an empty array of points.
    */
   WaveformPointArray(int channels);

   ~WaveformPointArray();

   /**
    * Resize the array to hold (fChannels * width) WaveformPoints.
    * @param width Number of points to hold.
    */
   void Resize(int width);

   /**
    * Set a new value for the point at (channel, pixel)
    * @param channel Channel num
    * @param pixel   Pixel index
    * @param point   Ref to a waveform point.
    */
   void SetPoint(int channel, int pixel, WaveformPoint const& point);

   /**
    * Get the waveform point data for the specified (channel, pixel)
    * @param  channel hannel num
    * @param  pixel   pixel index
    * @return         WaveFormPoint object.
    */
   WaveformPoint GetPoint(int channel, int pixel) const;

   /**
    * Get the number of channels to draw.
    * @return the number of channels (0..1)
    */
   int ChannelCount() const { return fChannelCount;};


private:
   int fChannelCount;
   Array<WaveformPoint> fPoints;

};

#endif

class WaveformComponent :  public StyledComponent
{
public:
   WaveformComponent(UiStyle* style, LoopProcessor* loop, const String& name=String::empty);

   ~WaveformComponent();

   void UpdateStyle();

   /**
    * Connect at runtime to a specific loop processor. This is useful because 
    * a particular TrackComponent (and its children) may be connected to several different 
    * Track objects over its lifetime.
    * @param loop Pointer to the LoopProcessor object that holds the samples we 
    * need to display. If nullptr, we disconnect.
    */
   void ConnectToLoop(LoopProcessor* loop);

   /**
    * Called by the loop processor when we need to know that something has changed.
    * @param source [description]
    */
   void changeListenerCallback(ChangeBroadcaster* source);

   /**
    * Called by JUCE when our size changes.
    */
   void resized();

#if 0
   /**
    * Whenever the width of this component changes or the loop duration changes, 
    * we need to recalculate the number of samples represented by a single pixel. 
    */
   void CalculateSamplesPerPixel();
#endif

   /**
    * Refresh the pixel buffer from the loop and redraw the entire component.
    */
   void RedrawAll();

   /**
    * Clear the component -- nuke any cached sample values and redraw the component as empty.
    */
   void Clear();

   /**
    * Called when the size of the loop changes.
    */
   void LoopSizeChanged();

#if 0
   /**
    * Call into the LoopProcessor to get any updated waveform data that we 
    * need for display; 
    * do any post-processing to prep for display, and get the component 
    * repainted appropriately.
    */
   void GetThumbnailData();
#endif
   
   /**
    * Called by JUCE when we need to paint ourself.
    * @param g Graphics object to use to draw.
    */
   void paint(Graphics& g);

private:
   LoopProcessor* fLoop;
   LoopProcessor::LoopInfo  fLoopInfo;
   //ScopedPointer<LoopProcessor::ThumbnailData> fThumbData;
   //WaveformPointArray fPixels;
   LoopThumbnail fThumb;

   float fFullScaleHeight;  /**< scale factor for a wave val == 1.0 */
   float fCenterYPos; 

   /**
    * We're probably going to be notified of new samples more frequently than
    * we want to redraw the screen. When we draw, we set this to zero. On each 
    * change notification from the loop when there are new samples, we add the number
    * of newly arrived samples to this accumulator. When 
    * fPendingSamples > fRedrawAfterSampleCount, we get thumbnail data for all the 
    * unprocessed samples and draw the resulting waveform.
    */
   int fPendingSamples;
   int fRedrawAfterSampleCount;



   /**
    * Index of the first pixel in our buffer that needs to be updated on the screen. 
    * When we can, we only invalidate the rect with new data to paint more quickly. 
    */
   int fDirtyStart;
   int fDirtyPixels; 

   /**
    * current 'now' sample
    */
   int fNow;

   /**
    * array of pixels where we should draw a tick mark to indicate time on the screen
    */
   Array<int> fTicks;

   /**
    * Let's try caching the colors that we use. maybe the dict lookups are costly when we do them all the time...
    */
   Colour fBg; 
   Colour fFg;
   Colour fNowLine;
   Colour fMonoWave;
   Colour fLeftWave;
   Colour fRightWave;


   /**
    * Protect the thumbnail data from read/write collisions.
    */
   CriticalSection fMutex;

};

#endif