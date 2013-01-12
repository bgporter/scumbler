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


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TrackComponent::TrackComponent (Scumbler* owner)
    : fScumbler(owner)
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 80);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

TrackComponent::~TrackComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TrackComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffb6b6b6));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void TrackComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
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
