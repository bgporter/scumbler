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

extern ApplicationCommandManager* gCommandManager;
extern ApplicationProperties* gAppProperties;
extern KnownPluginList  gKnownPlugins;

class PluginListWindow;

//==============================================================================
class MainAppWindow   : public DocumentWindow
                      , public MenuBarModel
                      , public ApplicationCommandTarget
                      , public ChangeListener
{
public:
    //==============================================================================
    MainAppWindow();
    ~MainAppWindow();

    void closeButtonPressed();

    void changeListenerCallback(ChangeBroadcaster*);


    /**
     * \name ConfigureAudio
     * \brief Display the built-in JUCE audio device selector.
     */
    
    void ConfigureAudio();

    /**
     * View (or delete) the plugin window that lets us scan for plugins.
     * @param display Passing `true` will display the plugin window, `false` will delete it. 
     */
    void ViewPlugins(bool display=true);


    /**
     * Clear the scumbler and create a new, empty workspace for us. If the scumbler is
     * currently dirty, we should make sure to give the user an option to save the
     * current contents before nuking them.
     */
    void New();

    /**
     * Open a Scumbler file from disk. If the current scumbler needs to be saved first, 
     * we should make sure that can happen.
     */
    void Open();
    /**
     * Save the current Scumbler to the current file. If there isn't a file open, 
     * do SaveAs() instead.
     */
    void Save();

    /**
     * Save the current Scumbler into a new file.
     */
    void SaveAs();

    /**
     * Create a new Scumbler for us to use. If there was already one in place, this new
     * one will completely replace it and it will be deleted automatically  .
     * @param addFirstTrack If true, creates a new empty track.
     */
    void CreateNewScumblerAndComponent(bool addFirstTrack);

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
    enum MenuId
    {
        kDefaultOrder = 200,
        kAlphaOrder,
        kCategoryOrder,
        kMfrOrder,
        kDirectoryOrder, 

        // keep this one at the end of the list
        kLastMenuId
    };


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)

    /**
     * The Audio devices are owned by the application window. We pass a reference 
     * to this object to the Scumbler object that needs to use it. This lets us 
     * keep concerns like configuring & peristing state of the underlying system
     * out of the model for this app.
     */
    AudioDeviceManager      fDeviceManager;

    /**
     * We also own the single plugin format manager here. It's needed in the UI to
     * get the list of available plugins, and it's needed in the model to actually
     * create instances plugins.
     */
    AudioPluginFormatManager fPluginManager;

    /**
     * We keep this as a ScopedPointer so it's sure to be cleaned up correctly when the 
     * app closes.
     */
    ScopedPointer<Scumbler> fScumbler;

    /**
     * A pointer to the window that lists/scans for plugins.
     */
    ScopedPointer<PluginListWindow>  fPluginWindow;

    /**
     * Path to the currently open file (if any)
     */
    String fFilePath;
};


#endif  // __MAINWINDOW_H_BDBA903E__
