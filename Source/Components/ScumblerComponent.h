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

#ifndef __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_C58AE459__
#define __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_C58AE459__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "../Scumbler.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ScumblerComponent  : public Component,
                           public ButtonListener
{
public:
    //==============================================================================
    ScumblerComponent (Scumbler* scumbler);
    ~ScumblerComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    /**
     * Add a new track to the scumbler object. 
     * @return result code indicating success or reason for failure.
     */
    Scumbler::Result AddTrack();

    
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Scumbler* fScumbler;
    //[/UserVariables]

    //==============================================================================
    TextButton* fNewTrackButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScumblerComponent);
};


#endif   // __JUCER_HEADER_SCUMBLERCOMPONENT_SCUMBLERCOMPONENT_C58AE459__
