// Copyright (c) 2013 Bg Porter

#include "TransportComponent.h"

#include "Scumbler.h"


TransportComponent::TransportComponent(Scumbler* scumbler)
:  Component("TransportComponent")
,  fScumbler(scumbler)
,  fAddTrackButton(nullptr)
,  fStopButton(nullptr)
,  fPlayButton(nullptr)
,  fPlayTime(nullptr)
,  fOutputVolume(nullptr)
,  fResetButton(nullptr)
{
   fAddTrackButton = new TextButton("Add track button");
   fAddTrackButton->setButtonText("+");
   fAddTrackButton->addListener(this);
   fAddTrackButton->setColour(TextButton::buttonColourId, Colours::white);
   fAddTrackButton->setEnabled(false);
   this->addAndMakeVisible(fAddTrackButton);

   fResetButton = new TextButton("reset button");
   fResetButton->setButtonText("reset");
   fResetButton->addListener(this);
   fResetButton->setColour(TextButton::buttonColourId, Colours::white);
   this->addAndMakeVisible(fResetButton);

   fStopButton = new TextButton("stop button");
   fStopButton->setButtonText("stop");
   fStopButton->addListener(this);
   fStopButton->setColour(TextButton::buttonColourId, Colours::white);
   this->addAndMakeVisible(fStopButton);

   fPlayButton = new TextButton("play button");
   fPlayButton->setButtonText("play");
   fPlayButton->addListener(this);
   fPlayButton->setColour(TextButton::buttonColourId, Colours::white);
   this->addAndMakeVisible(fPlayButton);

   addAndMakeVisible (fPlayTime = new Label ("new label",
                                           "00:00:00"));
   fPlayTime->setFont (Font ("Helvetica", 26.0000f, Font::plain));
   fPlayTime->setJustificationType (Justification::centred);
   fPlayTime->setEditable (false, false, false);
   fPlayTime->setColour(TextEditor::textColourId, Colours::black);
   fPlayTime->setColour(TextEditor::backgroundColourId, Colour (0x0));

   addAndMakeVisible (fOutputVolume = new Slider ("Volume"));
   fOutputVolume->setTooltip ("Output volume");
   fOutputVolume->setRange (-96.0, 0.0, 0);
   fOutputVolume->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fOutputVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
   fOutputVolume->setColour (Slider::thumbColourId, Colours::black);
   fOutputVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
   fOutputVolume->setPopupDisplayEnabled(true, this);
   fOutputVolume->setTextValueSuffix("dB");
   fOutputVolume->addListener(this);   

   this->setSize(600, 50);

}

TransportComponent::~TransportComponent()
{
   this->deleteAllChildren();
}

void TransportComponent::paint (Graphics& g)
{
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif  
   bool playing = fScumbler->IsPlaying();
   fPlayButton->setEnabled(!playing);
   fResetButton->setEnabled(!playing);
   fStopButton->setEnabled(playing);
   fOutputVolume->setValue(fScumbler->GetOutputVolume());
}

void TransportComponent::resized()
{

   fAddTrackButton->setBounds (40, 16, 24, 24);
   fStopButton->setBounds (240, 16, 47, 24);
   fPlayButton->setBounds (296, 16, 47, 24);
   fPlayTime->setBounds (480, 16, 150, 24);
   fOutputVolume->setBounds (920, 16, 32, 24);
   fResetButton->setBounds (176, 16, 47, 24);
}

void TransportComponent::buttonClicked (Button* buttonThatWasClicked)
{
   if (fAddTrackButton == buttonThatWasClicked)
   {
      fScumbler->AddTrack();
   }
   else if (fStopButton == buttonThatWasClicked)
   {
      if (fScumbler->IsPlaying())
      {
         fScumbler->TogglePlay();
      }
   }
   else if (fPlayButton == buttonThatWasClicked)
   {
      if (!fScumbler->IsPlaying())
      {
         fScumbler->TogglePlay();
      }
   }
   else if (fResetButton == buttonThatWasClicked)
   {
      // !!! todo.
   }
}

void TransportComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
   if (fOutputVolume == sliderThatWasMoved)
   {
      fScumbler->SetOutputVolume(fOutputVolume->getValue());
   }

}

void TransportComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (fScumbler == source)
   {

      this->repaint();
   }

}