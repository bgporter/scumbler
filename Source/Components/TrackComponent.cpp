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
#include "PluginBlockComponent.h"
#include "LoopComponent.h"
#include "Buttons/PlainButton.h"
#include "ComponentDefs.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TrackComponent::TrackComponent (UiStyle* style, Track* track)
    : StyledComponent(style)
    , fTrack(nullptr)
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
    fPreEffects = new PluginBlockComponent(fPluginColors[kPreBlock], pre);
    fPostEffects = new PluginBlockComponent(fPluginColors[kPostBlock], post);
    fLoop = new LoopComponent(style, loop);
    this->addAndMakeVisible(fPreEffects);
    this->addAndMakeVisible(fPostEffects);
    this->addAndMakeVisible(fLoop);


   this->ConnectToTrack(track);


   addAndMakeVisible (fInputGain = new Slider ("Input gain"));
   fInputGain->setTooltip ("Input gain");
   fInputGain->setRange (-96.0, 0.0, 0);
   fInputGain->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fInputGain->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
/* !!! */
 Colour fg = Colours::red;
 
 fInputGain->setColour(Slider::thumbColourId, fg);
 fInputGain->setColour(Slider::rotarySliderFillColourId, fg);
/* !!! */
    
    
   fInputGain->setPopupDisplayEnabled(true, this);
   fInputGain->setTextValueSuffix(" dB");
   fInputGain->addListener(this);  



   addAndMakeVisible(fPan = new Slider("Pan"));
   fPan->setTooltip("Pan");
   fPan->setRange(0.f, 1.0f, 0);
   fPan->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
   fPan->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

   /*
   fPan->setPopupDisplayEnabled(true, this);
   fPan->setTextValueSuffix("");
   */
   fPan->addListener(this);  

   addAndMakeVisible(fLeftEnabled = new ToggleButton("Left"));
   fLeftEnabled->setButtonText("L");
   fLeftEnabled->addListener(this);

   addAndMakeVisible(fRightEnabled = new ToggleButton("Right"));
   fRightEnabled->setButtonText("R");
   fRightEnabled->addListener(this);



   addAndMakeVisible (fOutputVolume = new Slider ("Volume"));
   fOutputVolume->setTooltip ("Track volume");
   fOutputVolume->setRange (-96.0, 0.0, 0);
   fOutputVolume->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
   fOutputVolume->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);

   fOutputVolume->setPopupDisplayEnabled(true, this);
   fOutputVolume->setTextValueSuffix(" dB");
   fOutputVolume->addListener(this);   

   fActive = new SvgButton("Active", kPlainButton, fStyle);
   fActive->setTooltip("Activate track");
   fActive->addListener(this);
   fActive->setClickingTogglesState(true);

   fActive->SetContext("Track", "Active");
   fActive->AddButtonImage(SvgButton::kHover, kPlainButton);
   fActive->AddButtonImage(SvgButton::kDown, kPlainButton);
   fActive->AddButtonImage(SvgButton::kNormalOn, kPlainButton);
   fActive->AddButtonImage(SvgButton::kHoverOn, kPlainButton);
   fActive->AddButtonImage(SvgButton::kDownOn, kPlainButton);

   this->addAndMakeVisible(fActive);

   bool active = false;
   if (fTrack)
   {
      active = fTrack->IsActive();
   }
   fActive->setToggleState(active, NotificationType::dontSendNotification); 
   

   fMute = new TextButton("Mute");
   fMute->setTooltip("Mute track");
   fMute->setButtonText("m");
   fMute->addListener(this);

   fMute->setClickingTogglesState(true);
   this->addAndMakeVisible(fMute);

   fSolo = new TextButton("Solo");
   fSolo->setTooltip("Solo track");
   fSolo->setButtonText("s");
   fSolo->addListener(this);

   fSolo->setClickingTogglesState(true);
   this->addAndMakeVisible(fSolo);

   fDelete = new TextButton("Delete");
   fDelete->setTooltip("Delete track");
   fDelete->setButtonText("x");
   fDelete->addListener(this);
   // !!!TODO: Set button colors.
   this->addAndMakeVisible(fDelete);

   this->UpdateStyle();


    //[/Constructor]
}

TrackComponent::~TrackComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    // Preven us (and our children!) from receiving any new change notifications
    //this->ConnectToTrack(nullptr);
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    this->deleteAllChildren();
    //[/Destructor]
}


void TrackComponent::UpdateStyle()
{
   Colour fg = fStyle->GetColor(palette::kAppFg);
   
   fInputGain->setColour(Slider::thumbColourId, fg);
   fInputGain->setColour(Slider::rotarySliderFillColourId, fg);

   fPan->setColour(Slider::thumbColourId, fg);
   fPan->setColour(Slider::rotarySliderFillColourId, fg);

   fOutputVolume->setColour(Slider::thumbColourId, fg);
   fOutputVolume->setColour(Slider::rotarySliderFillColourId, fg);

   fActive->UpdateStyle();


#if 0
   fActiveButtonImages->SetButtonImages(fActive, fStyle);
 #endif

   fMute->setColour(TextButton::buttonColourId, fStyle->GetColor(palette::kTrackMuteOff));
   fMute->setColour(TextButton::buttonOnColourId, fStyle->GetColor(palette::kTrackMuteOn));

   fSolo->setColour(TextButton::buttonColourId, fStyle->GetColor(palette::kTrackSoloOff));
   fSolo->setColour(TextButton::buttonOnColourId, fStyle->GetColor(palette::kTrackSoloOn));

   this->repaint();

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
    

    float halfWidth = this->getWidth() / 2.0;

    // first, draw the signal line underneath the pre-effects:
    Colour preColor = fPluginColors[kPreBlock].fg;
    Colour postColor = fPluginColors[kPostBlock].fg;
    g.setColour(preColor);
    g.fillRect(fCenterLineStartX, fCenterLineYPos-1.0, halfWidth, 3.0);
    // draw the post line...
    g.setColour(postColor);
    g.fillRect(halfWidth, fCenterLineYPos-1.0, halfWidth, 3.0);


    // update the pan setting/color
    fPan->setValue(fTrack->GetInputPan());
    fPan->setColour(Slider::thumbColourId, postColor);
    fPan->setColour(Slider::rotarySliderFillColourId, postColor);   
 
    // update the channel selection buttons
    tk::ChannelEnable chan = fTrack->GetEnabledChannels();
    fLeftEnabled->setToggleState(tk::kLeftChannel & chan, dontSendNotification);
    fRightEnabled->setToggleState(tk::kRightChannel & chan, dontSendNotification);

    // draw a bounding circle around the input/output volume knobs
    // and update their values/colors

    this->OutlineKnob(g, fInputGain);
    fInputGain->setValue(fTrack->GetInputGain());
    fInputGain->setColour(Slider::thumbColourId, postColor);
    fInputGain->setColour(Slider::rotarySliderFillColourId, postColor);   

    this->OutlineKnob(g, fOutputVolume);
    fOutputVolume->setValue(fTrack->GetOutputVolume());
    fOutputVolume->setColour(Slider::thumbColourId, postColor);
    fOutputVolume->setColour(Slider::rotarySliderFillColourId, postColor);   

    if (fTrack->CanBeDeleted())
    {
        fDelete->setVisible(true);
        bool deleteEnabled = ! fTrack->IsPlaying();
        fDelete->setEnabled(deleteEnabled);
    }
    else
    {
        fDelete->setVisible(false);
    }

    //g.drawRect(fSolo->getBounds());
    //g.drawRect(fMute->getBounds());


    //[/UserPaint]
}


void TrackComponent::OutlineKnob(Graphics& g, Slider* knob)
{
    Rectangle<int> box = knob->getBounds();
    box.expand(2, 2);
    Rectangle<float> floatBox(box.getX(), box.getY(), box.getWidth(), box.getHeight());
    g.setColour(fPluginColors[kPreBlock].bg);
    g.fillRoundedRectangle(floatBox, floatBox.getWidth()/2.0);
    g.setColour(fPluginColors[kPostBlock].fg);
    g.drawRoundedRectangle(floatBox, floatBox.getWidth()/2.0, 3.0);  
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

    // center the gain between the left edge and the start of the pre-effect block.
    int availableGainWidth = fPreEffects->getX();
    int gainXPos = (availableGainWidth - kKnobWidth) / 2;

    // center the volume between the right edge of the post effects and the edge of the component.
    int availableVolumeWidth = (this->getWidth() - fPostEffects->getRight());
    const int kOutputXPos = fPostEffects->getRight() + (availableVolumeWidth - kKnobWidth) / 2;
    const int kMargin = 5;
    int yPos = fCenterLineYPos - (kKnobHeight/2);

    Rectangle<int> inputBounds(gainXPos, yPos, kKnobHeight, kKnobHeight);
    fInputGain->setBounds(inputBounds);

    Rectangle<int> outputBounds(kOutputXPos, yPos, kKnobHeight, kKnobHeight);
    fOutputVolume->setBounds(outputBounds);
    fCenterLineStopX = outputBounds.getCentreX();

    // temp until we have a control for the input gain/pan
    fCenterLineStartX = 0.f;

    // The activate track button is directly underneath the pre plugin block.
    Rectangle<int> activeBounds(fPreEffects->getX(), fPreEffects->getBottom()+5, 
      kKnobHeight, kKnobHeight);
    fActive->setBounds(activeBounds);
    activeBounds.translate(kKnobHeight * 2, 0);
    fPan->setBounds(activeBounds);
    activeBounds.translate(kKnobHeight * 2, 0);
    activeBounds.setHeight(kKnobHeight / 2);
    fLeftEnabled->setBounds(activeBounds);
    activeBounds.translate(0, kKnobHeight / 2);
    fRightEnabled->setBounds(activeBounds);


    // The mute and solo controls are underneath the block of post plugins, 
    // with the right edge of the solo button aligned with the right edge of the
    // post effect block.
    Rectangle<int> buttonBounds(fPostEffects->getRight()-kKnobHeight, fPostEffects->getBottom()+5,
      kKnobHeight, kKnobHeight);
    fMute->setBounds(buttonBounds);
    Rectangle<int> soloBounds = buttonBounds.translated(-30, 0);
    fSolo->setBounds(soloBounds);

    // the delete button is at the upper right of each track.
    Rectangle<int> deleteBounds(0, 0, kKnobHeight/2, kKnobHeight/2);
    fDelete->setBounds(deleteBounds);



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


TrackComponent::PluginColors::PluginColors()
{
  bg = Colours::white;
  fg = Colours::black;
  text = Colours::white;
  fullSlotBg = Colours::grey;
  fullSlotFg = Colours::white;
  mouseOver = Colours::red;
}



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
      this->repaint();
    }
  }

}


Track* TrackComponent::GetTrack() const
{
   return fTrack;  
}

void TrackComponent::UpdateColors()
{
   // !!! Respond to the current track settings!
   bool pluginsActive[kPluginBlockCount];
   pluginsActive[kPreBlock] = fTrack->IsActive();
   pluginsActive[kPostBlock] = ! fTrack->IsMuted();

   for (int i = 0; i < kPluginBlockCount; ++i)
   {
     if (pluginsActive[i])
     {
        fPluginColors[i].fg = fStyle->GetColor(palette::kPluginFullActiveFg);
        fPluginColors[i].fullSlotFg = fStyle->GetColor(palette::kPluginFullActiveFg);
        fPluginColors[i].fullSlotBg = fStyle->GetColor(palette::kPluginFullActiveBg);
        fPluginColors[i].mouseOver = fStyle->GetColor(palette::kPluginActiveHover);
        fPluginColors[i].text = fStyle->GetColor(palette::kPluginActiveText);
     }
     else
     {
        fPluginColors[i].fg = fStyle->GetColor(palette::kPluginFullInactiveFg);
        fPluginColors[i].fullSlotFg = fStyle->GetColor(palette::kPluginFullInactiveFg);
        fPluginColors[i].fullSlotBg = fStyle->GetColor(palette::kPluginFullInactiveBg);
        fPluginColors[i].mouseOver = fStyle->GetColor(palette::kPluginInactiveHover);
        fPluginColors[i].text = fStyle->GetColor(palette::kPluginInactiveText);
     }
   }
}
void TrackComponent::changeListenerCallback(ChangeBroadcaster* source)
{
  std::cout << "TrackComponent::changeListenerCallback" << std::endl;
  if (source == fTrack)
  {
     if (fTrack->WantsToBeDeleted())
     {
        // the track is being deleted -- disconnect from it. 
        this->ConnectToTrack(nullptr);
     }
     else
     {
       fActive->setToggleState(fTrack->IsActive(), NotificationType::dontSendNotification); 
       this->UpdateColors();

       this->repaint();
     }
  }
  else if (source == fStyle)
  {
     this->UpdateStyle();
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
   else if (fActive == buttonThatWasClicked)
   {
      fTrack->SetActive(fActive->getToggleState());
   }
   else if ((fLeftEnabled == buttonThatWasClicked) || (fRightEnabled == buttonThatWasClicked))
   {
      // handle these buttons together
      tk::ChannelEnable currentEnable = fTrack->GetEnabledChannels();

      int newEnable = fLeftEnabled->getToggleState() ? tk::kLeftChannel : 0;
      newEnable |= fRightEnabled->getToggleState() ? tk::kRightChannel : 0;

      if (0 == newEnable)
      {
         // we can't have zero channels enabled, so attempting to do that actually
         // just enables the other channel.
         newEnable = ((int) tk::kStereo)^ currentEnable;     
      }
      fTrack->SetEnabledChannels((tk::ChannelEnable) newEnable);
   }
   else if (fDelete == buttonThatWasClicked)
   {
     if (!fTrack->IsPlaying())
     {
        fTrack->AskToBeDeleted();
     }
   }

}

void TrackComponent::sliderValueChanged (Slider* sliderThatWasMoved)
{
  if (fOutputVolume == sliderThatWasMoved)
  {
     fTrack->SetOutputVolume(fOutputVolume->getValue());  
  }
  else if (fInputGain == sliderThatWasMoved)
  {
     fTrack->SetInputGain(fInputGain->getValue());
  }
  else if (fPan == sliderThatWasMoved)
  {
     fTrack->SetInputPan(fPan->getValue());
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
