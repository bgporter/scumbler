 /*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"

#include "Commands.h"
#include "Components/ScumblerComponent.h"


//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName() , Colours::lightgrey
      , DocumentWindow::allButtons)
    , fScumbler(nullptr)
{

  // connect our menu bar model to the command manager -- anything changed 
  // there will be reflected back here in our menu.
  this->setApplicationCommandManagerToWatch(gCommandManager);

  // restore anything we saved in previous instantiations.
  PropertiesFile* userSettings = gAppProperties->getUserSettings();
  ScopedPointer<XmlElement> savedAudioState(userSettings->getXmlValue("audioDeviceState"));

  fDeviceManager.initialise(2, 2, // max 2 input and output channels.
    savedAudioState, // pass in the last known configuration state
    true              // select the default device if restoring the last config fails.
     );

  // create and reset the scumbler object now that the audio system is configured.
  fScumbler = new Scumbler(fDeviceManager);
  fScumbler->Reset();

  // create the scumbler component that owns & operates our user interface.
  ScumblerComponent* c = new ScumblerComponent(fScumbler);
  this->setContentOwned(c, true);
  this->setResizable(true, true);
  centreWithSize (1024, 768);
  setVisible (true);


  #if JUCE_MAC
    setMacMainMenu (this);
  #else
    setMenuBar (this);
  #endif
}

MainAppWindow::~MainAppWindow()
{
   #if JUCE_MAC
    setMacMainMenu (nullptr);
   #else
    setMenuBar (nullptr);
   #endif
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}



void MainAppWindow::ConfigureAudio()
{
  AudioDeviceSelectorComponent audioConfig(fDeviceManager, 
    1, 2,     // at least one input channel, no more than 2
    1, 2,     // at least one output channel, no more than 2
    true,     // show midi input options
    true,     // show midi output options
    true,     // show channels as stereo pairs.
    false     // don't hide the advanced options.
    );

  audioConfig.setSize(500, 450);

  DialogWindow::LaunchOptions(o);
  o.content.setNonOwned(&audioConfig);
  o.dialogTitle = "Configure Audio";
  o.componentToCentreAround = this;
  o.dialogBackgroundColour = Colours::grey; 
  o.escapeKeyTriggersCloseButton = true;
  o.useNativeTitleBar = true;
  o.resizable = false;

  o.runModal();

  // persist the settings for our next run.
  ScopedPointer<XmlElement> audioState(fDeviceManager.createStateXml());
  PropertiesFile* settings = gAppProperties->getUserSettings();
  settings->setValue("audioDeviceState", audioState);
  settings->saveIfNeeded();

  // !!! notify the scumbler object that the settings have changed

}

StringArray MainAppWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Options", "Help", nullptr };

    return StringArray (names);  
}

PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
  PopupMenu menu;

  switch (topLevelMenuIndex)
  {
    case 0:   //File menu
    {
      menu.addCommandItem(gCommandManager, CommandIds::kNew);
      menu.addCommandItem(gCommandManager, CommandIds::kOpen);
      menu.addCommandItem(gCommandManager, CommandIds::kSave);
      menu.addCommandItem(gCommandManager, CommandIds::kSaveAs);
    }
    break;

    case 1:   // Options menu
    {
      menu.addCommandItem(gCommandManager, CommandIds::kConfigAudio);
      menu.addCommandItem(gCommandManager, CommandIds::kPlay);
#ifdef qUnitTests
      menu.addSeparator();
      menu.addCommandItem(gCommandManager, CommandIds::kRunUnitTests);
#endif    
    }
    break;

    case 2:   //Help menu

    break;

    default:
    // There's shouldn't be a menu at this index! 
    jassertfalse;
    break;

}
  return menu;
}

void MainAppWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
  // TODO: !!!
}



ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
    return this->findFirstTargetParentComponent();
}

void MainAppWindow::getAllCommands(Array<CommandID>& commands)
{
  const CommandID ids[] = {
    CommandIds::kNew,
    CommandIds::kOpen,
    CommandIds::kSave,
    CommandIds::kSaveAs,
    CommandIds::kConfigAudio,
    CommandIds::kPlay,
    //CommandIds::kPause,
    //CommandIds::kRewind,
    //CommandIds::kToggleRecord
#ifdef qUnitTests
    CommandIds::kRunUnitTests
#endif    
  };
  commands.addArray(ids, numElementsInArray(ids));

}

void MainAppWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
  const String category("General");

  switch (commandID)
  {
    case CommandIds::kNew:
    { 
      result.setInfo("New", 
        "Create a new (empty) Scumbler file", category, 0);
      result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));

    }
    break;

    case CommandIds::kOpen:
    {
      result.setInfo("Open...",
        "Open a Scumbler file",
        category, 0);
        result.defaultKeypresses.add (KeyPress('o', ModifierKeys::commandModifier, 0));

    }
    break;

    case CommandIds::kSave:
    {
      result.setInfo("Save",
        "Save the current Scumbler setup to a file",
        category, 0);
        result.defaultKeypresses.add(KeyPress('s', ModifierKeys::commandModifier, 0));
    }
    break;

    case CommandIds::kSaveAs:
    {
      result.setInfo("Save As...",
        "Save a copy of the current Scumbler setup to a file",
        category, 0);
        result.defaultKeypresses.add(KeyPress('s', 
          ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
    }
    break;

    case CommandIds::kConfigAudio:
    {
      result.setInfo("Configure Audio...", String::empty, category, 0);
    }
    break;

    case CommandIds::kPlay:
    { 
      if (fScumbler->IsPlaying())
      {
        result.setInfo("Pause", "Pause audio playback", category, 0);
      }
      else
      {
        result.setInfo("Play", "Start audio playback", category, 0);
      }
      result.defaultKeypresses.add(KeyPress('p', ModifierKeys::commandModifier, 0));
    }
    break;
#ifdef qUnitTests
    case CommandIds::kRunUnitTests:
    {
      result.setInfo("Run unit tests", "Run all unit tests", "Development", 0);
      result.defaultKeypresses.add(KeyPress('t', ModifierKeys::commandModifier, 0));
    }
    break;
#endif
  }

}

bool MainAppWindow::perform(const InvocationInfo& info)
{
  bool retval = true;
  switch (info.commandID)
  {
    case CommandIds::kNew:
    {

    }
    break;

    case CommandIds::kOpen:
    {

    }
    break;
    case CommandIds::kSave:
    {

    }
    break;
    case CommandIds::kSaveAs:
    {

    }
    break;
    case CommandIds::kConfigAudio:
    {
      this->ConfigureAudio();
    }
    break;

    case CommandIds::kPlay:
    {
      fScumbler->TogglePlay();
      // tell the command manager something has changed. This will make it 
      // re-query us with getCommandInfo() and set the menu text to display either 
      // 'Play' or 'Pause'
      gCommandManager->commandStatusChanged();
    }
    break;
          
    case CommandIds::kRunUnitTests:
    {
#ifdef qUnitTests
      UnitTestRunner runner;
      // run all tests even if there are failures.
      runner.setAssertOnFailure(false);
      runner.runAllTests();
#endif    
    }
    break;
  }
  return retval;
}

