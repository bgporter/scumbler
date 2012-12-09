/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  8 Dec 2012 7:47:02pm

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

#include "ScumblerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ScumblerComponent::ScumblerComponent (Scumbler* scumbler)
    : Component ("The Scumbler"),
      fScumbler(scumbler),
      fNewTrackButton (0)
{
    addAndMakeVisible (fNewTrackButton = new TextButton ("new track"));
    fNewTrackButton->setButtonText ("+");
    fNewTrackButton->addListener (this);
    fNewTrackButton->setColour (TextButton::buttonColourId, Colour (0xffcccce2));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ScumblerComponent::~ScumblerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (fNewTrackButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ScumblerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3f3f3f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ScumblerComponent::resized()
{
    fNewTrackButton->setBounds (32, 456, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ScumblerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == fNewTrackButton)
    {
        //[UserButtonCode_fNewTrackButton] -- add your button handler code here..
        //[/UserButtonCode_fNewTrackButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ScumblerComponent" componentName="The Scumbler"
                 parentClasses="public Component" constructorParams="Scumbler* scumbler"
                 variableInitialisers="fScumbler(scumbler)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330000013" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff3f3f3f"/>
  <TEXTBUTTON name="new track" id="afec3388486bebff" memberName="fNewTrackButton"
              virtualName="" explicitFocusOrder="0" pos="32 456 24 24" bgColOff="ffcccce2"
              buttonText="+" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
