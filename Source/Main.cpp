/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "ScumblerDefs.h"


ApplicationCommandManager* gCommandManager = nullptr;
ApplicationProperties* gAppProperties = nullptr;
KnownPluginList gKnownPlugins;

//==============================================================================
class ScumblerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    ScumblerApplication()
    {
    }

    ~ScumblerApplication()
    {
    }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        // Do your application's initialisation code here..

        PropertiesFile::Options options;
        options.applicationName     = "Scumbler";
        options.filenameSuffix      = "settings";
        options.osxLibrarySubFolder = "Preferences";

        gAppProperties = new ApplicationProperties();
        gAppProperties->setStorageParameters (options);

        gCommandManager = new ApplicationCommandManager();

        mainWindow = new MainAppWindow();
        gCommandManager->registerAllCommandsForTarget(this);
        gCommandManager->registerAllCommandsForTarget(mainWindow);

        mainWindow->menuItemsChanged();

    }

    void shutdown()
    {
        // Do your application's shutdown code here..
        mainWindow = 0;

        gAppProperties->closeFiles();
        deleteAndZero(gCommandManager);
        deleteAndZero(gAppProperties);
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        quit();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return "Scumbler";
    }

    const String getApplicationVersion()
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed()
    {
        return false;
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        
    }

private:
    ScopedPointer <MainAppWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(ScumblerApplication)
