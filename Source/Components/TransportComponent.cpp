// Copyright (c) 2013 Bg Porter

#include "TransportComponent.h"

#include "Scumbler.h"


TransportComponent::TransportComponent(UiStyle* style, Scumbler* scumbler)
:  StyledComponent(style)
,  fScumbler(scumbler)
,  fAddTrackButton(nullptr)
,  fPlayButton(nullptr)
,  fPlayTime(nullptr)
,  fOutputVolume(nullptr)
,  fResetButton(nullptr)
{
   fAddTrackButton = new TextButton("Add track button");
   fAddTrackButton->setButtonText("+");
   fAddTrackButton->addListener(this);

   fAddTrackButton->setEnabled(true);
   this->addAndMakeVisible(fAddTrackButton);

   fResetButton = new TextButton("reset button");
   fResetButton->setButtonText("reset");
   fResetButton->addListener(this);

   this->addAndMakeVisible(fResetButton);

   fPlayButton = new TextButton("play button");
   fPlayButton->setButtonText("play");
   fPlayButton->addListener(this);

   this->addAndMakeVisible(fPlayButton);

   addAndMakeVisible (fPlayTime = new Label ("new label",
                                           "00:00:00"));
   fPlayTime->setFont (Font ("Helvetica", 26.0000f, Font::bold));
   fPlayTime->setJustificationType (Justification::centred);
   fPlayTime->setEditable (true, false, false);


   addAndMakeVisible (fOutputVolume = new Slider ("Volume"));
   fOutputVolume->setTooltip ("Output volume");
   fOutputVolume->setRange (-96.0, 0.0, 0);
   fOutputVolume->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fOutputVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

   fOutputVolume->setPopupDisplayEnabled(true, this);
   fOutputVolume->setTextValueSuffix("dB");
   fOutputVolume->addListener(this);   

   this->setSize(600, 50);

   this->UpdateStyle();

   fScumbler->addChangeListener(this);

}

TransportComponent::~TransportComponent()
{
   this->deleteAllChildren();
}


void TransportComponent::UpdateStyle()
{
   fAddTrackButton->setColour(TextButton::buttonColourId, Colours::white);
   fResetButton->setColour(TextButton::buttonColourId, Colours::white);
   fPlayButton->setColour(TextButton::buttonColourId, Colours::white);
   fPlayTime->setColour(TextEditor::textColourId, Colours::black);
   fPlayTime->setColour(TextEditor::backgroundColourId, Colour (0x0));
   fOutputVolume->setColour (Slider::thumbColourId, Colours::black);
   fOutputVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
}

void TransportComponent::paint (Graphics& g)
{


   g.fillAll (fStyle->GetColor(palette::kTransportBg));
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif  
   bool playing = fScumbler->IsPlaying();
   fPlayButton->setButtonText(playing ? "pause" : "play");
   fResetButton->setEnabled(!playing);
   fOutputVolume->setValue(fScumbler->GetOutputVolume());

   uint64 samples = fScumbler->GetSampleCount();
   // !!! This should change to reflect the actual saampling rate !!!
   uint64 seconds = samples / 44100;
   uint64 hours = seconds / (60 * 60);
   seconds -= hours * (60*60);
   uint64 minutes = seconds / 60;
   seconds -= minutes * 60;

   String h(hours);
   String m(minutes);
   String s(seconds);
   String time = h.paddedLeft('0', 2) + ":" + m.paddedLeft('0', 2) + ":" + s.paddedLeft('0', 2);
   fPlayTime->setText(time, NotificationType::dontSendNotification);
}

void TransportComponent::resized()
{
   int parentHeight = this->getHeight();
   fAddTrackButton->setBounds (40, (parentHeight-24)/2, 24, 24);
   fPlayButton->setBounds (296, (parentHeight-24)/2, 47, 24);
   fPlayTime->setBounds (480, (parentHeight-24)/2, 150, 24);
   fOutputVolume->setBounds (920, (parentHeight-24)/2, 32, 24);
   fResetButton->setBounds (176, (parentHeight-24)/2, 47, 24);
}

void TransportComponent::buttonClicked (Button* buttonThatWasClicked)
{
   if (fAddTrackButton == buttonThatWasClicked)
   {
      fScumbler->AddTrack();
   }
   else if (fPlayButton == buttonThatWasClicked)
   {
      fScumbler->TogglePlay();
   }
   else if (fResetButton == buttonThatWasClicked)
   {
      ModifierKeys mk = ModifierKeys::getCurrentModifiers();
      if (mk.isShiftDown())
      {
         fScumbler->ResetAllTracks();
      }
      else
      {
         fScumbler->SeekAllTracksAbsolute(0);
      }
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
   if ((fScumbler == source) || (fStyle == source))
   {
      this->repaint();
   }

}