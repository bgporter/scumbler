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
  fScumbler = new Scumbler(fDeviceManager, fPluginManager);
  //fScumbler->Reset();

  // create the scumbler component that owns & operates our user interface.
  ScumblerComponent* c = new ScumblerComponent(fScumbler);
  gCommandManager->registerAllCommandsForTarget(c);
  // set that component as this window's content (and take ownership of the pointer)
  this->setContentOwned(c, true);
  this->setResizable(true, true);
  this->centreWithSize (1024, 768);
  this->setVisible (true);


  #if JUCE_MAC
    // !!! TODO: Add About & Preferences items to the apple menu
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
  // @TODO: !!!
}



ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
    return this->findFirstTargetParentComponent();
}

void MainAppWindow::getAllCommands(Array<CommandID>& commands)
{
  const CommandID ids[] = {
    //CommandIds::kNew,
    //CommandIds::kOpen,
    //CommandIds::kSave,
    //CommandIds::kSaveAs,
    CommandIds::kConfigAudio,
    //CommandIds::kPlay,
    //CommandIds::kPause,
    //CommandIds::kRewind,
    //CommandIds::kToggleRecord
   
  };
  commands.addArray(ids, numElementsInArray(ids));

}

void MainAppWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
  const String category("General");


  switch (commandID)
  {
    case CommandIds::kConfigAudio:
    {
      result.setInfo("Configure Audio...", String::empty, category, 0);
    }
    break;



    
  }
}

bool MainAppWindow::perform(const InvocationInfo& info)
{
  bool retval = true;
  switch (info.commandID)
  {

    case CommandIds::kConfigAudio:
    {
      this->ConfigureAudio();
    }
    break;


          

  }
  return retval;
}

