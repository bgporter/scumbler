/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"


//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons),
      fScumbler(fDeviceManager)
{
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

StringArray MainAppWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Options", "Help", nullptr };

    return StringArray (names);  
}

PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
  PopupMenu menu;
  // TODO: !!!
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
    CommandIds::kConfigAudio
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
        result.defaultKeypresses.add (KeyPress('s', ModifierKeys::commandModifier, 0));
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
      result.setInfo("Change the audio device settings",
        String::empty, category, 0);
    }
    break;
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

    }
    break;
  }
  return retval;
}