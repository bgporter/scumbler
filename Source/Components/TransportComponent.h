// Copyright (c) 2013 Bg Porter

// !!! license

#ifndef h_TransportComponent
#define h_TransportComponent

#include "JuceHeader.h"

class Scumbler;


class TransportComponent   : public Component 
                           , public ButtonListener
                           , public SliderListener
                           , public ChangeListener
{
public:
   TransportComponent(Scumbler* scumbler);

   ~TransportComponent();

   void paint (Graphics& g);
   void resized();
   void buttonClicked (Button* buttonThatWasClicked);
   void sliderValueChanged (Slider* sliderThatWasMoved);   

   void changeListenerCallback(ChangeBroadcaster* source);

private:

   Scumbler* fScumbler;

   TextButton* fAddTrackButton;
   TextButton* fStopButton;
   TextButton* fPlayButton;
   Label* fPlayTime;
   Slider* fOutputVolume;
   TextButton* fResetButton;   

   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent);   
};                         


#endif