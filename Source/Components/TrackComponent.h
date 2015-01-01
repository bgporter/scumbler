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

#include "StyledComponent.h"
#include "SvgButton.h"
#include "FlatButton.h"  
#include "../Track.h"


// forward defs...
class PluginBlockComponent;
class LoopComponent;





//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TrackComponent  : public StyledComponent
                      //, public ChangeListener
                      , public ButtonListener
                      , public LabelListener
                      , public Slider::Listener
{
public:
    //==============================================================================
    TrackComponent (UiStyle* style, Track* owner=nullptr);
    ~TrackComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.


    /**
     * \struct PluginColors
     * The TrackComponent maintains an instance of this struct for each of
     * the plugin blocks it owns. This lets us hide the color selection 
     * logic in a single place when e.g. the track solo/mute/etc. state
     * changes (or when we get loadable palettes...)
     */
    struct PluginColors
    {
        /**
         * A ctor to make sure we have defaults set somewhat sensibly.
         */
        PluginColors();

        Colour bg;
        Colour fg;
        Colour fullSlotBg;
        Colour fullSlotFg;
        Colour mouseOver;
        Colour text;

    };


    /**
     * Respond to new style settings.
     */
    void UpdateStyle();

    /**
     * Set the Track object inside the Scumbler that this component will be representing 
     * on screen. 
     * @param track A pointer to the track object we should use. Passing in a null pointer 
     *              has the effect of disconnecting us from the track we are already connected
     *              to.
     */
    void ConnectToTrack(Track* track);    


    /**
     * Tell this track component what its track number is for display purposes. We 
     * reset this every time tracks are added or deleted from the scumbler.
     * @param number Track number (1-based)
     * @param count  Total number of tracks.
     */
    void SetTrackNumber(int number, int count);
    /**
     * Returns a pointer to the track object within the Scumbler that's being represented 
     * on screen by this component (and its children). Note that this may change at runtime,
     * so it's important for child components to *not* cache this pointer.       
     * @return pointer to a Track object.
     */
    Track* GetTrack() const;


    /**
     * REturn a track name for display for this track. **Q:** Why not just use 
     * `fTrack->GetName()`? **A:** when a track is untitled, we display things differently
     * than if the track has been given an explicit name.
     * @return String to display.
     *
     */
    String GetTrackName() const;
    /**
     * Update the colors used to draw this track component (and its children)
     */
    void UpdateColors();


    /**
     * Called when something we're watching calls us back with a notification.
     */
    void changeListenerCallback(ChangeBroadcaster* source);

    /**
     * The user edited the title of this track.
     * @param source [description]
     */
    void labelTextChanged(Label* source);

    /**
     * Called when the volume knob changes.
     * @param sliderThatWasMoved [description]
     */
    void sliderValueChanged (Slider* sliderThatWasMoved);  

    /**
     * Called when a button is clicked.
     * @param buttonThatWasClicked Pointer to a button object.
     */
    void buttonClicked (Button* buttonThatWasClicked);

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
    /**
     * Draw a filled circle underneath a rotary knob in the right Colours
     * @param knob pointer to the Slider object we're outlining.
     */
    void OutlineKnob(Graphics& g, Slider* knob);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Track*  fTrack;
    
    // color structs
    enum PluginBlockIndex
    {
        kPreBlock = 0,
        kPostBlock = 1,
        kPluginBlockCount
    };
    PluginColors  fPluginColors[kPluginBlockCount];


    //Child components (complex)
    PluginBlockComponent* fPreEffects;
    PluginBlockComponent* fPostEffects;
    LoopComponent*  fLoop;
    
    // Child components (buttons, etc.)
    Label* fNumLabel;
    Label* fTitle;
    Slider* fInputGain;
    Slider* fOutputVolume;
    Slider* fPan;
    ToggleButton* fLeftEnabled;
    ToggleButton* fRightEnabled;
    FlatButton* fMute;
    FlatButton* fSolo; 
    SvgButton* fDelete;
    SvgButton* fActive;

    
    float fCenterLineYPos;
    float fCenterLineStartX;
    int fCenterLineStopX;

    int fTrackNum;
    int fTrackCount;

    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackComponent);
};


#endif   // __JUCER_HEADER_TRACKCOMPONENT_TRACKCOMPONENT_88FA4414__
