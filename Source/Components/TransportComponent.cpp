// Copyright (c) 2013 Bg Porter

#include "TransportComponent.h"

#include "ComponentDefs.h"
#include "Scumbler.h"

// Buttons
#include "Buttons/PlayButton.h"
#include "Buttons/ResetButton.h"


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

   fResetButton = new DrawableButton("reset", DrawableButton::ImageStretched);
   fResetButton->setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);
   fResetButton->addListener(this);

   fResetButtonImages = new SvgButton( kResetButton, // normal
                                       kResetButton, // hover
                                       kResetButton, // down
                                       kResetButton  //disabled
                                     );

   // create the button images & get the palette data set
   fResetButtonImages->SetPaletteEntry(SvgButton::kNormal, "bg-stroke", palette::kTransportResetUpBorder);
   fResetButtonImages->SetPaletteEntry(SvgButton::kNormal, "bg-fill", palette::kTransportResetUpFill);
   fResetButtonImages->SetPaletteEntry(SvgButton::kNormal, "fg-stroke", palette::kTransportResetUpFg);
   fResetButtonImages->SetPaletteEntry(SvgButton::kNormal, "fg-fill", palette::kTransportResetUpFgFill);

   fResetButtonImages->SetPaletteEntry(SvgButton::kHover, "bg-stroke", palette::kTransportResetHoverBorder);
   fResetButtonImages->SetPaletteEntry(SvgButton::kHover, "bg-fill", palette::kTransportResetHoverFill);
   fResetButtonImages->SetPaletteEntry(SvgButton::kHover,"fg-stroke", palette::kTransportResetHoverFg);
   fResetButtonImages->SetPaletteEntry(SvgButton::kHover,"fg-fill", palette::kTransportResetHoverFgFill);

   fResetButtonImages->SetPaletteEntry(SvgButton::kDown, "bg-stroke", palette::kTransportResetDownBorder);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDown, "bg-fill", palette::kTransportResetDownFill);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDown, "fg-stroke", palette::kTransportResetDownFg);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDown, "fg-fill", palette::kTransportResetDownFgFill);

   fResetButtonImages->SetPaletteEntry(SvgButton::kDisabled, "bg-stroke", palette::kTransportResetDisabledBorder);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDisabled, "bg-fill", palette::kTransportResetDisabledFill);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDisabled, "fg-stroke", palette::kTransportResetDisabledFg);
   fResetButtonImages->SetPaletteEntry(SvgButton::kDisabled, "fg-fill", palette::kTransportResetDisabledFgFill);

   this->addAndMakeVisible(fResetButton);

   fPlayButton = new DrawableButton("play", DrawableButton::ImageStretched);
   fPlayButton->setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);
   fPlayButton->addListener(this);


   fPlayButtonImages = new SvgButton(  kPlayButton,   // kNormal
                                       kPlayButton,   // hover
                                       kPlayButton,   // down
                                       String::empty, // disabled
                                       kPauseButton,  // normal-on
                                       kPauseButton,  // hover-on
                                       kPauseButton,  // down-on
                                       String::empty  // disabled-on
                                    );

   // create the button images & get the palette data set
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormal, "bg-stroke", palette::kTransportPlayUpBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormal, "bg-fill", palette::kTransportPlayUpFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormal, "fg-stroke", palette::kTransportPlayUpFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormal, "fg-fill", palette::kTransportPlayUpFgFill);

   fPlayButtonImages->SetPaletteEntry(SvgButton::kHover, "bg-stroke", palette::kTransportPlayHoverBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHover, "bg-fill", palette::kTransportPlayHoverFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHover,"fg-stroke", palette::kTransportPlayHoverFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHover,"fg-fill", palette::kTransportPlayHoverFgFill);

   fPlayButtonImages->SetPaletteEntry(SvgButton::kDown, "bg-stroke", palette::kTransportPlayDownBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDown, "bg-fill", palette::kTransportPlayDownFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDown, "fg-stroke", palette::kTransportPlayDownFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDown, "fg-fill", palette::kTransportPlayDownFgFill);

   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormalOn, "bg-stroke", palette::kTransportPlayUpBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormalOn, "bg-fill", palette::kTransportPlayUpFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormalOn, "fg-stroke", palette::kTransportPlayUpFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kNormalOn, "fg-fill", palette::kTransportPlayUpFgFill);

   fPlayButtonImages->SetPaletteEntry(SvgButton::kHoverOn, "bg-stroke", palette::kTransportPlayHoverBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHoverOn, "bg-fill", palette::kTransportPlayHoverFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHoverOn, "fg-stroke", palette::kTransportPlayHoverFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kHoverOn, "fg-fill", palette::kTransportPlayHoverFgFill);

   fPlayButtonImages->SetPaletteEntry(SvgButton::kDownOn, "bg-stroke", palette::kTransportPlayDownBorder);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDownOn, "bg-fill", palette::kTransportPlayDownFill);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDownOn, "fg-stroke", palette::kTransportPlayDownFg);
   fPlayButtonImages->SetPaletteEntry(SvgButton::kDownOn, "fg-fill", palette::kTransportPlayDownFgFill);


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

   fPlayButtonImages->SetButtonImages(fPlayButton, fStyle);
   fResetButtonImages->SetButtonImages(fResetButton, fStyle);


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
   int yPos = (height - kKnobHeight) / 2;

   fAddTrackButton->setBounds (40, yPos, 24, 24);
   fResetButton->setBounds (176, yPos, 32, 32);
   fPlayButton->setBounds (296, yPos, 32, 32);
   fPlayTime->setBounds (480, yPos, 150, 24);
   fOutputVolume->setBounds (width-60, yPos, 32, 24);
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

