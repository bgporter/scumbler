/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  16 Dec 2012 10:49:41am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "TrackComponent.h"
#include "ComponentDefs.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TrackComponent::TrackComponent (Track* track)
    : fTrack(track)
    , fCenterLineYPos(0)
{

    //[UserPreSize]
    //[/UserPreSize]

    //setSize (600, 80);

    PluginBlock* pre = nullptr;
    PluginBlock* post = nullptr;
    LoopProcessor* loop = nullptr;

    if (nullptr != track)
    {
       pre = track->GetPreEffectBlock();
       post = track->GetPostEffectBlock();  
       loop = track->GetLoop();
    }

    //[Constructor] You can add your own custom stuff here..
    fPreEffects = new PluginBlockComponent(pre);
    fPostEffects = new PluginBlockComponent(post);
    fLoop = new LoopComponent(loop);
    this->addAndMakeVisible(fPreEffects);
    this->addAndMakeVisible(fPostEffects);
    this->addAndMakeVisible(fLoop);

   addAndMakeVisible (fOutputVolume = new Slider ("Volume"));
   fOutputVolume->setTooltip ("Track volume");
   fOutputVolume->setRange (-96.0, 0.0, 0);
   fOutputVolume->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fOutputVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
   fOutputVolume->setColour (Slider::thumbColourId, Colours::black);
   fOutputVolume->setColour (Slider::rotarySliderFillColourId, Colour (0x7f000000));
   fOutputVolume->setPopupDisplayEnabled(true, this);
   fOutputVolume->setTextValueSuffix("dB");
   fOutputVolume->addListener(this);   

   fMute = new TextButton("Mute");
   fMute->setTooltip("Mute track");
   fMute->setButtonText("m");
   fMute->addListener(this);
   fMute->setColour(TextButton::buttonColourId, Colours::white);
   fMute->setClickingTogglesState(true);
   this->addAndMakeVisible(fMute);

   fSolo = new TextButton("Solo");
   fSolo->setTooltip("Solo track");
   fSolo->setButtonText("s");
   fSolo->addListener(this);
   fSolo->setColour(TextButton::buttonColourId, Colours::white);
   fSolo->setClickingTogglesState(true);
   this->addAndMakeVisible(fSolo);



    //[/Constructor]
}

TrackComponent::~TrackComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    // Preven us (and our children!) from receiving any new change notifications
    this->ConnectToTrack(nullptr);
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    this->deleteAllChildren();
    //[/Destructor]
}

//==============================================================================
void TrackComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

#ifdef qSketch
   g.setColour(Colours::lightslategrey);
   g.drawRect(0, 0, this->getWidth(), this->getHeight());
#endif

    //[UserPaint] Add your own custom painting code here..

    // This chunk of code will need revision when we have multiple
    // tracks:
    // - the signal line before the waveform for inactive tracks should be gray
    // - all the black should be gray for muted tracks.

    g.setColour(Colours::black);
    g.fillRect(0.0, fCenterLineYPos-1.0, fCenterLineStopX, 3.0);

    // draw a bounding circle around the output volume knob.
    Rectangle<int> box = fOutputVolume->getBounds();
    box.expand(2, 2);
    Rectangle<float> floatBox(box.getX(), box.getY(), box.getWidth(), box.getHeight());
    g.setColour(Colours::white);
    g.fillRoundedRectangle(floatBox, floatBox.getWidth()/2.0);
    g.setColour(Colours::black);
    g.drawRoundedRectangle(floatBox, floatBox.getWidth()/2.0, 3.0);

    fOutputVolume->setValue(fTrack->GetOutputVolume());
    
    //g.drawRect(fSolo->getBounds());
    //g.drawRect(fMute->getBounds());


    //[/UserPaint]
}

void TrackComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //
    //!!! *NOTE* that all the literal const values here are temp placeholders. These need to be 
    //designed/chosen much more carefully.
    int trackWidth = this->getWidth();
    int trackHeight = this->getHeight();
    int pluginBlockWidth = this->proportionOfWidth(kPluginBlockWidth);
    int pluginBlockHeight = this->proportionOfHeight(kPluginBlockHeight);
    int effectBlockHeight = pluginBlockHeight - kControlRowHeight;
    int preX = kTrackMargin;
    int postX = trackWidth - kTrackMargin - pluginBlockWidth;
    int loopX = preX + pluginBlockWidth;
    int loopWidth = postX - (preX + pluginBlockWidth);
    int effectBlockYPos = trackHeight * 0.1;
    fCenterLineYPos = effectBlockYPos + (effectBlockHeight / 2.0);

    
    fPreEffects->setBounds(preX, effectBlockYPos, pluginBlockWidth, effectBlockHeight);
    fLoop->setBounds(loopX, effectBlockYPos, loopWidth, pluginBlockHeight);
    fPostEffects->setBounds(postX, effectBlockYPos, pluginBlockWidth, effectBlockHeight);

    // center the volume between the right edge of the post effects and the edge of the component.
    int availableVolumeWidth = (this->getWidth() - fPostEffects->getRight());
    const int kXPos = fPostEffects->getRight() + (availableVolumeWidth - kKnobWidth) / 2;
    const int kMargin = 5;
    int yPos = fCenterLineYPos - (kKnobHeight/2);

    Rectangle<int> outputBounds(kXPos, yPos, kKnobHeight, kKnobHeight);
    fOutputVolume->setBounds(outputBounds);
    fCenterLineStopX = outputBounds.getCentreX();

    // The mute and solo controls are underneath the block of post plugins, 
    // with the right edge of the solo button aligned with the right edge of the
    // post effect block.
    Rectangle<int> buttonBounds(fPostEffects->getRight()-kKnobHeight, fPostEffects->getBottom()+5,
      kKnobHeight, kKnobHeight);
    fMute->setBounds(buttonBounds);
    Rectangle<int> soloBounds = buttonBounds.translated(-30, 0);
    fSolo->setBounds(soloBounds);



    //[/UserResized]
}

void TrackComponent::moved()
{
    //[UserCode_moved] -- Add your code here...
    //[/UserCode_moved]
}

void TrackComponent::parentSizeChanged()
{
    //[UserCode_parentSizeChanged] -- Add your code here...
    //[/UserCode_parentSizeChanged]
}

void TrackComponent::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    //[/UserCode_mouseDown]
}

void TrackComponent::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    //[/UserCode_mouseDrag]
}

void TrackComponent::focusGained (FocusChangeType cause)
{
    //[UserCode_focusGained] -- Add your code here...
    //[/UserCode_focusGained]
}

void TrackComponent::focusLost (FocusChangeType cause)
{
    //[UserCode_focusLost] -- Add your code here...
    //[/UserCode_focusLost]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


void TrackComponent::ConnectToTrack(Track* track)
{
  // if we are already listening to this track, there's nothing to do. Carry on.
  if (track != fTrack)
  {
    // disconnect any old track from us and our children..
    if (fTrack)
    {
      fTrack->removeChangeListener(this);
      fPreEffects->ConnectToPluginBlock(nullptr);
      fPostEffects->ConnectToPluginBlock(nullptr);
      fLoop->ConnectToLoop(nullptr);
    }
    fTrack = track;
    if (fTrack)
    {
      fTrack->addChangeListener(this);
      fPreEffects->ConnectToPluginBlock(fTrack->GetPreEffectBlock());
      fPostEffects->ConnectToPluginBlock(fTrack->GetPostEffectBlock());
      fLoop->ConnectToLoop(fTrack->GetLoop());
    }
  }

}


Track* TrackComponent::GetTrack() const
{
   return fTrack;  
}

void TrackComponent::changeListenerCallback(ChangeBroadcaster* source)
{
  if (source == fTrack)
  {
     this->repaint();
  }
}

void TrackComponent::buttonClicked (Button* buttonThatWasClicked)
{
   if (fMute == buttonThatWasClicked)
   {
      fTrack->Mute(fMute->getToggleState());
   }
   else if (fSolo == buttonThatWasClicked)
   {
      fTrack->Solo(fSolo->getToggleState());
   }

}

void TrackComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
  if (fOutputVolume == sliderThatWasMoved)
  {
     fTrack->SetOutputVolume(fOutputVolume->getValue());  
  }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TrackComponent" componentName=""
                 parentClasses="public Component" constructorParams="Scumbler* owner"
                 variableInitialisers="fScumbler(owner)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="600"
                 initialHeight="80">
  <METHODS>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="parentSizeChanged()"/>
    <METHOD name="moved()"/>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="focusGained (FocusChangeType cause)"/>
    <METHOD name="focusLost (FocusChangeType cause)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffb6b6b6"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
