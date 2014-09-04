// Copyright (c) 2013 Bg Porter

// !!! License 

#ifndef h_LoopProcessor
#define h_LoopProcessor

#include "Passthrough.h"

class Track;

class LoopProcessor : public PassthroughProcessor
                    , public ChangeBroadcaster
{
public:

   enum
   {

      kMinDuration = 100, /**< a resonable minimum duration */
      kMaxDuration = 20000, /**< an arbitrary maximum duration */

   };

   /**
    * @struct LoopInfo
    *
    * UI components can use this structure to get useful information about 
    * the current state of this processor.
    */
   struct LoopInfo
   {
      double  fSampleRate;  //**< current sample rate
      int     fLoopSample;  //**< index of the *next* sample that will be written to 
      int     fLoopLength;  //**< number of samples in the loop
      int     fLoopCount;   //**< how many times have we played through the loop?
      bool    fIsPlaying;   //**< are we currently playing?
      bool    fWasReset;    //**< was the buffer reset since the last time we looked at it?

      /**
       * We need to provide our own op= -- this is used in the waveform component to 
       * decide if anything about the display needs to be updated.
       */
      bool operator==(const LoopInfo& rhs) const
      {
        return ((fSampleRate == rhs.fSampleRate) &&
               (fLoopSample == rhs.fLoopSample) &&
               (fLoopLength == rhs.fLoopLength) &&
               (fLoopCount == rhs.fLoopCount) &&
               (fIsPlaying == rhs.fIsPlaying) &&
               (fWasReset == rhs.fWasReset));
      };
      bool operator!=(const LoopInfo& rhs) const { return !operator==(rhs); };
   };

   /**
    * @struct ThumbnailData -- used to retrieve sample data from the loop processor that
    * can be used for waveform display. 
    */
   struct ThumbnailData
   {

      /**
       * [in] how many channels are we prepared to look at?
       */
      int fChannelCount;     
      /**
       * [in/out] floating point accumulator used to track the index of the next sample
       * to look at -- by keeping this as a float, we keep precision between calls.
       * 
       * on input, index of the first sample to look at.
       * After return, index of the *next* sample to look at on the next call.
       */
      float fStart;
      
      /**
       * [in] How many thumbnail values should be returned at most.
       */
      int fMaxThumbnailValues;
      
      /**
       * [in] number of samples that should be looked at to generate a single thumbnail 
       * pixel. (# of loop buffer samples / # of waveform pixels being displayed)
       */
      float fSamplesPerPixel; 

      /**
       * [out] The number of valid pixels contained in the fPixelData buffer. 
       * 0 <= fPixelsReturned <= fMaxThumbnailValues
       */
      int fPixelsReturned;

      /**
       * [out] an array of floats per channel. On return, each will be filled with 
       * `fPixelsReturned` floats each from 0..1. We're assuming that the waveform is 
       * symmetrical. Any values after fPixelsReturned should be ignored. 
       */
      Array<float> fPixelData; 

      /**
       * Create the ThumbnailData object.
       */
      ThumbnailData(int channelCount);

      /**
       * Resize the pixel arrays to a new size (called when the component width changes)
       * Affects all channels at the same time.
       * @param newCapacity Number of new pixels to make space for.
       */
      void Resize(int newCapacity);

      /**
       * Store the new pixel value at 
       * @param channel  Channel of the new pixel (0..fChannels-1)
       * @param index    0-based index to store the new value at.
       * @param val      value to store (0.0 <= val <= 1.0)
       */
      void SetPixelValue(int channel, int pixelNum, float val);

      /**
       * Return the pixel value for the specified (channel, pixelNum) tuple.
       * @param  channel  Channel# (0..channelCount-1)
       * @param  pixelNum pixel num being addressed (0..currentWidth)
       * @return          value from 0..1
       */
      float GetPixelValue(int channel, int pixelNum);



   };

   /**
    * Create the loop processor.
    * @param track non-owning pointer to the track that owns us. 
    * @param channelCount number of input and output channels for this loop.
    * 
    */
   LoopProcessor(Track* track, int channelCount = 1);

   ~LoopProcessor();

   /**
    * Set the duration of this loop. This may require the reallocation of loop 
    * buffers, and should probably only be permissable if we're not playing.
    * @param milliseconds Duration of the loop in ms.
    */
   tk::Result SetLoopDuration(int milliseconds);

   /**
    * Get the current duration of the loop 
    * @return integer, duration in ms.
    */
   int GetLoopDuration() const;

   /**
    * Change the feedback applied to the loop.
    * @param gain 0..1
    */
   void SetFeedback(float gain);  

   /**
    * Get the current feedback being applied to the loop.
    * @return [description]
    */
   float GetFeedback() const;

   /**
    * Is this loop playing? (true if the track is playing..   .)
    * @return bool.
    */
   bool IsPlaying() const;


   /**
    * Fill the passed-in ThumbnailData structure with information about sample peaks
    * that can be used elsewhere to display a thumbnail of the waveform
    * @param  data Reference to a ThumbnailData structure
    */
   void GetThumbnailData(ThumbnailData* data);


   /**
    * Clear and reset the loop, setting all read/write positions back to zero.
    */
   void Reset();

   /**
    * Set the loop pointer to the specified sample position inside the loop. 
    * If loopPos < 0 or > loopLength, we clamp the new position to those legal bounds.
    * @param loopPos sample # (0..loopLength);
    */
   void SeekAbsolute(int loopPos = 0);
   

   /**
    * Retrieve useful and current data about the state of our looping so we can 
    * update the UI correctly.
    * @param info LoopInfo struct. Filled on output.
    */
   void GetLoopInfo(LoopInfo& info);
   /**
     * @name required overrides of pure virtual functions.
     */
   ///@{
   
   /**
    * get the name of this processor.
    * @return String filled with the name.
    */
   const String getName() const;
 
   /**
    * Called before playback starts to get things ready.
    * @param sampleRate               samples per second.
    * @param estimatedSamplesPerBlock 
    */
   void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

   void releaseResources();

   void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

   bool silenceInProducesSilenceOut() const;

   // AudioProcessorEditor* createEditor();

   // bool hasEditor() const;

   // int getNumParameters();

   // const String getParameterName(int parameterIndex);

   // float getParameter(int parameterIndex);

   // const String getParameterText(int parameterIndex);

   // void setParameter(int parameterIndex, float newValue);

   // int getNumPrograms();

   // int getCurrentProgram();

   // void setCurrentProgram(int index);

   // const String getProgramName(int index);

   // void changeProgramName(int index, const String& newName);

   // void getStateInformation(juce::MemoryBlock& destData);

   // void setStateInformation(const void* data, int sizeInBytes);


   ///@}



private:

   /**
    * Track that we belong to.
    */
   Track* fTrack;

   /**
    * number of samples per second.
    */
   double fSampleRate;


   /**
    * Loop duration in ms.
    */
   int fLoopDuration;

   /**
    * feedback gain -- we default to 0.9f unity gain. 
    */
   float fFeedback;

   /**
    * buffer to hold our loop data.
    */
   ScopedPointer<AudioSampleBuffer> fLoopBuffer;

   /**
    * Index of the next sample inside the buffer for us to deal with; updated 
    * on each call to processBlock()
    */
   int fLoopPosition;

   /**
    * Number of times we've looped. Updated each time we wrap around.
    */
   int fLoopCount;

   /**
    * Were we fully reset since the last time the display code looked at us?
    */
   bool fWasReset;

   /**
    * Make access to variables threadsafe.
    */
   CriticalSection fMutex;

};


#endif
