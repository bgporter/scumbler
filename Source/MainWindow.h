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

namespace CommandIds
{
    static const CommandID kNew     = 0x2FFF;
    static const CommandID kOpen    = 0x3000;
    static const CommandID kSave    = 0x3001;
    static const CommandID kSaveAs  = 0x3002;
    static const CommandID kConfigAudio = 0x3003;
    static const CommandID kAboutBox = 0x3004;
    static const CommandID kPlay    = 0x3005;
    static const CommandID kPause   = 0x3006;
    static const CommandID kRewind  = 0x3007;
    static const CommandID kToggleRecord = 0x3008;      
}

extern ApplicationCommandManager* gCommandManager;
extern ApplicationProperties* gAppProperties;

//==============================================================================
class MainAppWindow   : public DocumentWindow
                      , public MenuBarModel
                      , public ApplicationCommandTarget
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();

    void closeButtonPressed();

    /**
     * \name ConfigureAudio
     * \brief Display the built-in JUCE audio device selector.
     */
    
    void ConfigureAudio();

    /**
     * @name MenuBarModel overrides.
     */
    ///@{
 
    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
 
    ///@}
    
    /**
     * @name ApplicationCommandTarget overrides.
     */
    
    ///@{
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands(Array<CommandID>& commands);
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
    bool perform(const InvocationInfo& info);
    ///@}


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
