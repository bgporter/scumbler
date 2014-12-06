// Copyright (c) 2013 Bg Porter

#include "TransportComponent.h"

#include "ComponentDefs.h"
#include "Scumbler.h"

// Buttons
#include "Buttons/PlayButton.h"
#include "Buttons/PlusButton.h"
#include "Buttons/ResetButton.h"


TransportComponent::TransportComponent(UiStyle* style, Scumbler* scumbler)
:  StyledComponent(style, "TransportComponent")
,  fScumbler(scumbler)
,  fAddTrackButton(nullptr)
,  fPlayButton(nullptr)
,  fPlayTime(nullptr)
,  fResetButton(nullptr)
,  fOutputVolume(nullptr)
{
   fAddTrackButton = new SvgButton("AddTrack", kPlusButton, fStyle);
   fAddTrackButton->addListener(this);
   fAddTrackButton->setEnabled(true);

   // the AddTrack button reuses the play button palette entries. 
   fAddTrackButton->SetContext("Transport", "Play");
   fAddTrackButton->AddButtonImage(tk::kButtonHover, kPlusButton);
   fAddTrackButton->AddButtonImage(tk::kButtonDown, kPlusButton);
   fAddTrackButton->AddButtonImage(tk::kButtonDisabled, kPlusButton);

   this->addAndMakeVisible(fAddTrackButton);

   fResetButton = new SvgButton("Reset", kResetButton, fStyle);
   fResetButton->addListener(this);
   fResetButton->setEnabled(true);
   fResetButton->SetContext("Transport", "Play");
   fResetButton->AddButtonImage(tk::kButtonHover, kResetButton);
   fResetButton->AddButtonImage(tk::kButtonDown, kResetButton);
   fResetButton->AddButtonImage(tk::kButtonDisabled, kResetButton);
   this->addAndMakeVisible(fResetButton);


   fPlayButton = new SvgButton("Play", kPlayButton, fStyle);
   fPlayButton->addListener(this);
   fPlayButton->setEnabled(true);
   fPlayButton->SetContext("Transport", "Play");
   fPlayButton->AddButtonImage(tk::kButtonHover, kPlayButton);
   fPlayButton->AddButtonImage(tk::kButtonDown, kPlayButton);
   fPlayButton->AddButtonImage(tk::kButtonNormalOn, kPauseButton);
   fPlayButton->AddButtonImage(tk::kButtonHoverOn, kPauseButton);
   fPlayButton->AddButtonImage(tk::kButtonDownOn, kPauseButton);

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

#if 0
   fPlusButtonImages->SetButtonImages(fAddTrackButton, fStyle);
   fPlayButtonImages->SetButtonImages(fPlayButton, fStyle);
   fResetButtonImages->SetButtonImages(fResetButton, fStyle);
#endif

   fAddTrackButton->UpdateStyle();
   fPlayButton->UpdateStyle();
   fResetButton->UpdateStyle();

   fPlayTime->setColour(TextEditor::textColourId, Colours::black);
   fPlayTime->setColour(TextEditor::backgroundColourId, Colour (0x0));
   fOutputVolume->setColour (Slider::thumbColourId, Colours::black);
   fOutputVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
}

void TransportComponent::paint (Graphics& g)
{

   LogPaint(this, g);
   g.fillAll (fStyle->GetColor(palette::kTransportBg));
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif  
   bool playing = fScumbler->IsPlaying();
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
   int height = this->getHeight();
   int width = this->getWidth();
   int buttonSize = height * 0.64;

   int yPos = (height - buttonSize) / 2;

   //std::cout << "transport height = " << height << std::endl;

   fAddTrackButton->setBounds (40, yPos, buttonSize, buttonSize);
   fResetButton->setBounds (176, yPos, buttonSize, buttonSize);
   fPlayButton->setBounds (296, yPos, buttonSize, buttonSize);
   fPlayTime->setBounds (480, yPos, 150, 24);
   fOutputVolume->setBounds (width-(40 + buttonSize), yPos, buttonSize, buttonSize);
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
      fPlayButton->setToggleState(fScumbler->IsPlaying(), NotificationType::dontSendNotification);
      this->repaint();
   }

}

