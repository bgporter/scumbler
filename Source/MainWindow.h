/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#ifndef __MAINWINDOW_H_BDBA903E__
#define __MAINWINDOW_H_BDBA903E__

#include "../JuceLibraryCode/JuceHeader.h"

#include "Scumbler.h"


//==============================================================================
class MainAppWindow   : public DocumentWindow
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();

    void closeButtonPressed();


    /* Note: Be careful when overriding DocumentWindow methods - the base class
       uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in you content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       implementation calls the superclass's method.
    */

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)

    /**
     * The Audio devices are owned by the application window. We pass a reference 
     * to this object to the Scumbler object that needs to use it. This lets us 
     * keep concerns like configuring & peristing state of the underlying system
     * out of the model for this app.
     */
    AudioDeviceManager  fDeviceManager;
    Scumbler            fScumbler;
};


#endif  // __MAINWINDOW_H_BDBA903E__
