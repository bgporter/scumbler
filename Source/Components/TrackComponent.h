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

#ifndef __JUCER_HEADER_TRACKCOMPONENT_TRACKCOMPONENT_88FA4414__
#define __JUCER_HEADER_TRACKCOMPONENT_TRACKCOMPONENT_88FA4414__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include "../Scumbler.h"
#include "PluginBlockComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TrackComponent  : public Component
{
public:
    //==============================================================================
    TrackComponent (Scumbler* owner);
    ~TrackComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void moved();
    void parentSizeChanged();
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void focusGained (FocusChangeType cause);
    void focusLost (FocusChangeType cause);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Scumbler* fScumbler;
    PluginBlockComponent* fPreEffects;
    PluginBlockComponent* fPostEffects;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent);
};


#endif   // __JUCER_HEADER_TRACKCOMPONENT_TRACKCOMPONENT_88FA4414__
