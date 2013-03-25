// Copyright (c) 2013 Bg Porter

// !!! License

#ifndef h_WaveformComponent
#define h_WaveformComponent

#include "../JuceLibraryCode/JuceHeader.h"


#include "Loop.h"

class Track;


/**
 * @struct WaveformPoint
 * The Waveform component keeps an array of these structures to cache the 
 * high/low points (y-values) to be drawn at that pixel location, so our draw loop
 * will look something like:
 * for xPos in range(0, width):
 *    drawLine(xPos, wavePoints[xPos].top, xPos, wavePoints[xPos].bottom) 
 *
 * ...obviously there's a lot more logic in there for invalidating regions, 
 * calculating the values, etc. We use the compiler's copy ctor, op=, etc.
 *
 * Note that the values here are yValues relative to the component, and should be 
 * symmetrical w/r/t the center line of the component. We should do those calculations
 * as rarely as possible. 
 */
struct WaveformPoint
{
public:
   WaveformPoint(float top_ = 0, float bottom_ = 0)
   :  top(top_)
   ,  bottom(bottom_)
   {
     // empty.
   }

   ~WaveformPoint()
   {
      // empty.
   }

   WaveformPoint(const WaveformPoint& other)
   :  top(other.top)
   ,  bottom(other.bottom)
   {
      // empty.  
   }


   /**
    * See http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    * for an explanation of why we take this parm by val and not by ref.
    */
   WaveformPoint& operator=(WaveformPoint rhs)
   {
      swap(*this, rhs);
      return *this;
   }



   friend void swap(WaveformPoint& first, WaveformPoint& second)
   {
      using std::swap;
      
      swap(first.top, second.top);
      swap(first.bottom, second.bottom); 
   }


   void Set(float top_, float bottom_)
   {
      top = top_;
      bottom = bottom_;
   }

public:
  float top;
  float bottom;

};

class WaveformComponent :  public Component
                        ,  public ChangeListener   
{
public:
   WaveformComponent(LoopProcessor* loop);

   ~WaveformComponent();

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

   /**
    * Whenever the width of this component changes or the loop duration changes, 
    * we need to recalculate the number of samples represented by a single pixel. 
    */
   void CalculateSamplesPerPixel();

   /**
    * Clear the component -- nuke any cached sample values and redraw the component as empty.
    */
   void Clear();

   /**
    * Called when the size of the loop changes.
    */
   void LoopSizeChanged();

   /**
    * Call into the LoopProcessor to get any updated waveform data that we 
    * need for display; 
    * do any post-processing to prep for display, and get the component 
    * repainted appropriately.
    */
   void GetThumbnailData();

   /**
    * Called by JUCE when we need to paint ourself.
    * @param g Graphics object to use to draw.
    */
   void paint(Graphics& g);


private:
   LoopProcessor* fLoop;
   LoopProcessor::LoopInfo  fLoopInfo;
   ScopedPointer<LoopProcessor::ThumbnailData> fThumbData;
   // for the moment, a single array; not one per channel.
   Array<WaveformPoint> fPixels;

   float fFullScaleHeight;  /**< scale factor for a wave val == 1.0 */
   float fCenterYPos; 

   int fPendingSamples;
   int fRedrawAfterSampleCount;


};

#endif