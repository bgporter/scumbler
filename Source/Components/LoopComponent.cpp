// Copyright (c) 2013 Bg Porter

#include "LoopComponent.h"

#include "Scumbler.h"


void DragLabel::mouseUp(const MouseEvent& e)
{
   int x = 9;
   //this->showEditor();
   Label::mouseUp(e);
}

LoopComponent::LoopComponent(LoopProcessor* loop)
:  fLoop(nullptr)
,  fFeedback(nullptr)
,  fWaveform(nullptr)
{

   fWaveform = new WaveformComponent(nullptr);
   this->addAndMakeVisible(fWaveform);
    
   this->ConnectToLoop(loop);

   fDuration = new Label("Duration");
   fDuration->setTooltip("Loop duration");
   fDuration->setJustificationType(Justification::centredRight);
   fDuration->setEditable(true, false, false);
   fDuration->setBorderSize(1, 1);
   //fDuration->setColour(Label::outlineColourId, Colours::green);
   fDuration->setText(String(fLoop->GetLoopDuration()), false);
   fDuration->addListener(this);
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
      fDuration->setText(String(fLoop->GetLoopDuration()), false);
      //this->repaint();
   }
}

void LoopComponent::buttonClicked (Button* buttonThatWasClicked)
{
}


void LoopComponent::labelTextChanged(Label* label)
{
   if (label == fDuration)
   {
      Value val = fDuration->getTextValue();
      int newDuration = static_cast<int>(val.getValue());
      fLoop->SetLoopDuration(newDuration);
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
   fDuration->setBounds(5, waveformHeight + 2, 64, 16);

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

}