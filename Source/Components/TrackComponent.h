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

#include "../Track.h"
#include "PluginBlockComponent.h"
#include "LoopComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TrackComponent  : public Component
                      , public ChangeListener
{
public:
    //==============================================================================
    TrackComponent (Track* owner=nullptr);
    ~TrackComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    /**
     * Set the Track object inside the Scumbler that this component will be representing 
     * on screen. 
     * @param track A pointer to the track object we should use. Passing in a null pointer 
     *              has the effect of disconnecting us from the track we are already connected
     *              to.
     */
    void ConnectToTrack(Track* track);    

    /**
     * Returns a pointer to the track object within the Scumbler that's being represented 
     * on screen by this component (and its children). Note that this may change at runtime,
     * so it's important for child components to *not* cache this pointer.       
     * @return pointer to a Track object.
     */
    Track* GetTrack() const;


    /**
     * Called when something we're watching calls us back with a notification.
     */
    void changeListenerCallback(ChangeBroadcaster* source);

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
    Track*  fTrack;
    PluginBlockComponent* fPreEffects;
    PluginBlockComponent* fPostEffects;
    LoopComponent*  fLoop;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent);
};


#endif   // __JUCER_HEADER_TRACKCOMPONENT_TRACKCOMPONENT_88FA4414__
