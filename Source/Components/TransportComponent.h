// Copyright (c) 2013 Bg Porter

// !!! license

#ifndef h_TransportComponent
#define h_TransportComponent

#include "JuceHeader.h"
#include "StyledComponent.h"
#include "SvgButton.h"


class Scumbler;


class TransportComponent   : public StyledComponent 
                           , public ButtonListener
                           , public SliderListener
{
public:
   TransportComponent(UiStyle* style, Scumbler* scumbler);

   void UpdateStyle();

   ~TransportComponent();

   void paint (Graphics& g);
   void resized();
   void buttonClicked (Button* buttonThatWasClicked);
   void sliderValueChanged (Slider* sliderThatWasMoved);   

   void changeListenerCallback(ChangeBroadcaster* source);

private:
   /**
    * Load the correct play/pause graphics into the play button depending on the 
    * current state of the Scumbler object. 
    */
   void SetPlayButton();

private:

   Scumbler* fScumbler;

   TextButton* fAddTrackButton;
   DrawableButton* fPlayButton;
   Label* fPlayTime;
   Slider* fOutputVolume;
   TextButton* fResetButton;   

   // Button image data
   ScopedPointer<SvgButton> fPlayUp;
   ScopedPointer<SvgButton> fPlayHover;
   ScopedPointer<SvgButton> fPlayDown;
   ScopedPointer<SvgButton> fPauseUp;
   ScopedPointer<SvgButton> fPauseHover;
   ScopedPointer<SvgButton> fPauseDown;


   // Drawable data -- since we need to be able to swap play/pause images
   // when the playback state changes, we keep those pointers around
   OwnedArray<Drawable> fPlayDrawables;
   OwnedArray<Drawable> fPauseDrawables;


   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent);   
};                         


#endif