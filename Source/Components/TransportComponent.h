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

   Scumbler* fScumbler;

   DrawableButton* fAddTrackButton;
   DrawableButton* fPlayButton;
   Label* fPlayTime;
   Slider* fOutputVolume;
   DrawableButton* fResetButton;   

   ScopedPointer<SvgButton>   fPlayButtonImages;
   ScopedPointer<SvgButton>   fPlusButtonImages;
   ScopedPointer<SvgButton>   fResetButtonImages;

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent);   
};                         


#endif