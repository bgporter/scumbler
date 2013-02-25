// Copyright (c) 2013 Bg Porter

#include "LoopComponent.h"

#include "Scumbler.h"


LoopComponent::LoopComponent(Track* track)
:  fTrack(track)
,  fLoop(track->GetLoop())
,  fFeedback(nullptr)
,  fWaveform(nullptr)
{

   fWaveform = new WaveformComponent(track);
   this->addAndMakeVisible(fWaveform);

   fDuration = new TextButton("Duration");
   fDuration->setTooltip("Loop duration");
   fDuration->setButtonText("D");
   fDuration->addListener(this);
   fDuration->setColour(TextButton::buttonColourId, Colours::white);
   this->addAndMakeVisible(fDuration);
   

   fFeedback = new Slider("feedback");
   fFeedback->setTooltip ("Loop feedback");
   fFeedback->setRange (-96.0, 0.0, 0);
   fFeedback->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fFeedback->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
   fFeedback->setColour (Slider::thumbColourId, Colours::black);
   fFeedback->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
   fFeedback->setPopupDisplayEnabled(true, this);
   fFeedback->setTextValueSuffix("dB");
   fFeedback->addListener(this);     
   this->addAndMakeVisible(fFeedback);



}

LoopComponent::~LoopComponent()
{
   this->deleteAllChildren();
   
}

void LoopComponent::ConnectToLoop(LoopProcessor* loop)
{
   if (loop != fLoop)
   {
      if (fLoop)
      {
         fLoop->removeChangeListener(this);
      }
      fLoop = loop;
      if (fLoop)
      {
         fLoop->addChangeListener(this);
      }
   }
   fWaveform->ConnectToLoop(loop);
} 

void LoopComponent::changeListenerCallback(ChangeBroadcaster* source)
{
   if (source == fLoop)
   {
      this->repaint();
   }
}

void LoopComponent::buttonClicked (Button* buttonThatWasClicked)
{
   if (fDuration == buttonThatWasClicked)
   {

   }
}

void LoopComponent::sliderValueChanged(Slider* slider)
{
   if (fFeedback == slider)
   {
      fLoop->SetFeedback(DbToFloat(slider->getValue()));
   }
   
}

void LoopComponent::resized()
{
   const int controlHeight = 24;
   const int controlRowHeight = 30;
   const int waveformHeight = this->getHeight() - controlRowHeight;
   const int controlTop = waveformHeight + (controlRowHeight - controlHeight) / 2;

   fWaveform->setBounds(0, 0, this->getWidth(), waveformHeight);

   int xPos = this->getWidth() * 0.1;
   fDuration->setBounds(xPos, controlTop, 24, 24);

   fFeedback->setBounds(this->getWidth() * 0.9, controlTop, 32, 24);
}

void LoopComponent::paint(Graphics& g)
{
#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif   

   float feedback = fLoop->GetFeedback();
   fFeedback->setValue(GainToDb(feedback));

   fDuration->setEnabled(!fTrack->IsPlaying());
}