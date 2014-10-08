 /*
  ==============================================================================

    This file was auto-generated!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#include "MainWindow.h"

#include "Commands.h"
#include "Components/ScumblerComponent.h"
#include "Components/UiStyle.h"
#include "Components/Palette.h"
#include "PluginListWindow.h"


namespace
{
   const String kFileExtension(".scumbler");

};

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName() , Colours::lightgrey
      , DocumentWindow::allButtons)
    , fScumbler(nullptr)
    , fFilePath(String::empty)
{

  this->InitializeStyle();

  // make sure that the plugin format manager knows to look for AU/VST formats.
  fPluginManager.addDefaultFormats();


  // connect our menu bar model to the command manager -- anything changed 
  // there will be reflected back here in our menu.
  this->setApplicationCommandManagerToWatch(gCommandManager);

  // restore anything we saved in previous instantiations.
  PropertiesFile* userSettings = gAppProperties->getUserSettings();
  ScopedPointer<XmlElement> savedAudioState(userSettings->getXmlValue("audioDeviceState"));

  fDeviceManager.initialise(2, 2, // max 2 input and output channels.
    savedAudioState,              // pass in the last known configuration state
    true                          // select the default device if restoring the last config fails.
     );

  // restore the saved plugin list from the preferences file. NOTE that the weird 
  // code structure below around subscribing to changes in the known plugin 
  // list is intentional (two separate `if()` statements instead of if/else). 
  // We want to be sure to subscribe to those changes after we restore from XML 
  // (if we can), but before we do our first scan of plugins (assuming that's 
  // what happens.)
  ScopedPointer<XmlElement> pluginList(userSettings->getXmlValue("pluginList"));
  if (nullptr != pluginList)
  {
    gKnownPlugins.recreateFromXml(*pluginList);
  }

  gKnownPlugins.addChangeListener(this);
  
  if (nullptr == pluginList)
  {
    this->ViewPlugins();
  }

#if 0
  // create and reset the scumbler object now that the audio system is configured.
  fScumbler = new Scumbler(fDeviceManager, fPluginManager);
  //fScumbler->Reset();
  
  // Pass any saved configuration parameters down to the scumbler object.
  int sortOrder = userSettings->getIntValue("pluginSortOrder", (int) KnownPluginList::defaultOrder);
  fScumbler->SetPluginSortOrder((KnownPluginList::SortMethod) sortOrder);

  // create the scumbler component that owns & operates our user interface.NOTE 
  // that because we're about to call 'setContentOwned()' we don't need to retain a pointer 
  // to the component here. This window object will take care of its lifespan.
  ScumblerComponent* c = new ScumblerComponent(fScumbler);
  gCommandManager->registerAllCommandsForTarget(c);
  // set that component as this window's content (and take ownership of the pointer)
  this->setContentOwned(c, true);
#else
    this->CreateNewScumblerAndComponent(true);
#endif
    
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

   // if the list of plugins is being displayed, make sure it gets cleaned up
   gKnownPlugins.removeChangeListener(this);
   this->ViewPlugins(false);
   // destroy the UI components
   this->clearContentComponent();
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

void MainAppWindow::changeListenerCallback(ChangeBroadcaster* source)
{
  this->menuItemsChanged();
  if (source == &gKnownPlugins)
  {
    // re-save the plug-in list as it changes so we have at least a partial inventory
    // in the event of a crash.
    ScopedPointer<XmlElement> savedPluginList(gKnownPlugins.createXml());

    if (nullptr != savedPluginList)
    {
      PropertiesFile* userSettings = gAppProperties->getUserSettings();
      userSettings->setValue("pluginList", savedPluginList);
      gAppProperties->saveIfNeeded();
    }
  } 
  else if (source == fScumbler)
  {
     String title = fScumbler->GetTitle();
     if (fScumbler->IsDirty())
     {
        title += "*";
     }    
     this->setName(title);
  }
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

  // User closed the dialog -- persist the settings for our next run.
  ScopedPointer<XmlElement> audioState(fDeviceManager.createStateXml());
  PropertiesFile* settings = gAppProperties->getUserSettings();
  settings->setValue("audioDeviceState", audioState);
  settings->saveIfNeeded();

  // !!! notify the scumbler object that the settings have changed

}


void MainAppWindow::InitializeStyle()
{    
    const char* const fontNames[] = {"Helvetica Neue", "Helvetica", "Arial", nullptr};
    //const char* const fontNames[] = {"foo", "banana", "whelp", nullptr};
    StringArray desiredFonts(fontNames);
    StringArray availableFonts(Font::findAllTypefaceNames());

    String uiFont("");
    
    // loop through the desired font names and find the first one that is present on the system.
    for (int i = 0; i < desiredFonts.size(); ++i)
    {
        String fontName = desiredFonts[i];
        if (availableFonts.contains(fontName))
        {
           uiFont = fontName;
           break;
        }
    }
    if (String("") == uiFont)
    {
        // none of the requested fonts are available on this machine -- fall back to 
        // whatever the sans serif default is.
        uiFont = Font::getDefaultSansSerifFontName();
    }

    // create a new palette with the default (light) palette. Someday we should
    // write the last palette info into the app's preferences and restore accordingly.
    Palette* palette = new Palette(Palette::kLightPalette);

    fStyle = new UiStyle(uiFont, palette);

}

void MainAppWindow::ViewPlugins(bool display)
{
  if (display)
  {
    // don't create a second instance!
    if (nullptr == fPluginWindow)
    {
      fPluginWindow = new PluginListWindow(this, fPluginManager);
      fPluginWindow->toFront(true);
    }
  }
  else
  {
    // make the window go away.
    fPluginWindow = nullptr;
  }

}


void MainAppWindow::New()
{
   // !!! Check existing scumbler dirty state first!
   if (fScumbler->IsDirty())
   {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon,
        "Save current configuration?",
        "Save changes to the Scumbler before creating a new one?",
        "Save", "Discard", "Cancel", 
        nullptr, nullptr);
      if (0 == result)
      {
          //cancel
          return;  
      }
      else if (1 == result)
      {
          // save first
          this->Save();
      }
      // else it's 2 and we don't do anything to discard except keep going.

   }
   this->CreateNewScumblerAndComponent(true);
   // reset the file path..
   fFilePath = String::empty;
}

void MainAppWindow::Open()
{
   if (fScumbler->IsDirty())
   {
      int result = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon,
        "Save current configuration?",
        "Save changes to the Scumbler before opening another one?",
        "Save", "Discard", "Cancel", 
        nullptr, nullptr);
      if (0 == result)
      {
          //cancel
          return;  
      }
      else if (1 == result)
      {
          // save first
          this->Save();
      }
      // else it's 2 and we don't do anything to discard except keep going     
   }
   FileChooser fc("Open a scumbler file", 
     File::getSpecialLocation(File::userDocumentsDirectory), "*" + kFileExtension);
   if (fc.browseForFileToOpen())
   {
     File xmlFile(fc.getResult());
     XmlDocument doc(xmlFile);
     fFilePath = xmlFile.getFullPathName();

     ScopedPointer<XmlElement> xml(doc.getDocumentElement());

     if (xml)
     {
        this->CreateNewScumblerAndComponent(false);
        StringArray errors;
        fScumbler->LoadXml(xml, errors, 0);
        // !!! display any errors that occurred.
     }
   }
}

void MainAppWindow::Save()
{
   if (!fFilePath.isEmpty())
   {
     ScopedPointer<XmlElement> contents(fScumbler->DumpXml(0));
     File temp(fFilePath);
     contents->writeToFile(temp, String::empty);
     fScumbler->SetDirty(false);
   }
   else
   {
      // there's no saved file yet, so execute SaveAs...
      this->SaveAs();  
   }
}

void MainAppWindow::SaveAs()
{
   FileChooser fc("Specify path & file to save to",
      File::getSpecialLocation(File::userDocumentsDirectory),
      kFileExtension, true);
   if (fc.browseForFileToSave(true))
   {
       File chosenFile = fc.getResult();
       fFilePath = chosenFile.getFullPathName();
       if (! fFilePath.endsWith(kFileExtension))
       {
          fFilePath += kFileExtension; 
       }
       this->Save(); 
   }  

}

bool MainAppWindow::IsDirty() const
{
   return fScumbler->IsDirty();  
}

void MainAppWindow::CreateNewScumblerAndComponent(bool addFirstTrack)
{
   if (fScumbler)
   {
      // if there's already a scumbler object in existence, tell it to stop processing
      // so we can get the new one we're about to create connected correctly.
      fScumbler->StopProcessing();
      fScumbler->removeChangeListener(this);
   }  
   fScumbler = new Scumbler(fDeviceManager, fPluginManager);
   // Pass any saved configuration parameters down to the scumbler object.
   PropertiesFile* userSettings = gAppProperties->getUserSettings();
   int sortOrder = userSettings->getIntValue("pluginSortOrder", (int) KnownPluginList::defaultOrder);
   fScumbler->SetPluginSortOrder((KnownPluginList::SortMethod) sortOrder);

   if (addFirstTrack)
   {
      // Add a track to the scumbler and make it active.
      fScumbler->AddTrack();    
      fScumbler->ActivateTrack(0);
   }

   fScumbler->addChangeListener(this);
   fScumbler->SetDirty(false);

   // create the scumbler component that owns & operates our user interface.NOTE 
   // that because we're about to call 'setContentOwned()' we don't need to retain a pointer 
   // to the component here. This window object will take care of its lifespan.
   ScumblerComponent* c = new ScumblerComponent(fStyle, fScumbler);
   gCommandManager->registerAllCommandsForTarget(c);
   // set that component as this window's content (and take ownership of the pointer)
   this->setContentOwned(c, true);
   int w = this->getWidth();
   int h = this->getHeight();
   // why 128? When JUCE is creating the main window component, it starts out 
   // for some reason at 128x128 dimensions. If we've already been sized explicitly, 
   // restore those dimensions after replacing the ScumblerComponent. !!! At 
   // some point, a more deeply considered solution to this problem should be developed.
   if ((w != 128) && (h != 128))
   {
      this->setSize(w, h);
   }

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
      menu.addCommandItem(gCommandManager, CommandIds::kViewPlugins);
      // add the submenu to control how we sort the plugins...
      PopupMenu sortMenu;
      KnownPluginList::SortMethod order = fScumbler->GetPluginSortOrder();
      sortMenu.addItem(kDefaultOrder, "List plugins in default order", true, 
        order == KnownPluginList::defaultOrder);
      sortMenu.addItem(kAlphaOrder, "List plugins in alphabetical order", true, 
        order == KnownPluginList::sortAlphabetically);
      sortMenu.addItem(kCategoryOrder, "List plugins by category", true, 
        order == KnownPluginList::sortByCategory);
      sortMenu.addItem(kMfrOrder, "List plugins by manufacturer", true, 
        order == KnownPluginList::sortByManufacturer);
      sortMenu.addItem(kDefaultOrder, "List plugins by directory structure", true, 
        order == KnownPluginList::sortByFileSystemLocation);

      menu.addSubMenu("Plugin Sorting", sortMenu);

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
   if (menuItemID >= kDefaultOrder && menuItemID <= kDirectoryOrder)
   {
      KnownPluginList::SortMethod sortOrder; 

      switch (menuItemID)
      {
         case kAlphaOrder:
            sortOrder = KnownPluginList::sortAlphabetically;
            break;

         case kCategoryOrder:
            sortOrder = KnownPluginList::sortByCategory;
            break;

         case kMfrOrder:
            sortOrder = KnownPluginList::sortByManufacturer;
            break;

         case kDirectoryOrder:
            sortOrder = KnownPluginList::sortByFileSystemLocation;
            break;

         case kDefaultOrder:
         default:
            sortOrder = KnownPluginList::defaultOrder;
            break;
      }
      fScumbler->SetPluginSortOrder(sortOrder);
      // save this for next time...
      PropertiesFile* settings = gAppProperties->getUserSettings();
      settings->setValue("pluginSortOrder", (int) sortOrder);
      settings->saveIfNeeded();

      this->menuItemsChanged();


   }
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
    CommandIds::kViewPlugins,
    //CommandIds::kPlay,
    //CommandIds::kPause,
    //CommandIds::kRewind,
    //CommandIds::kToggleRecord
   
  };
  commands.addArray(ids, numElementsInArray(ids));

}

void MainAppWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
  const String category("Audio");
  const String fileCategory("File");


  switch (commandID)
  {
    case CommandIds::kNew:
    {
      result.setInfo("New",
        "Create a new (empty) Scumbler file", category, 0);
      result.defaultKeypresses.add(KeyPress('n', ModifierKeys::commandModifier, 0));
      result.setActive(!fScumbler->IsPlaying());

    }
    break;

    case CommandIds::kOpen:
    {
      result.setInfo("Open...",
        "Open a Scumbler file",
        category, 0);
      result.defaultKeypresses.add (KeyPress('o', ModifierKeys::commandModifier, 0));
      result.setActive(!fScumbler->IsPlaying());
    }
    break;

    case CommandIds::kSave:
    {
      result.setInfo("Save",
        "Save the current Scumbler setup to a file",
        category, 0);
      result.defaultKeypresses.add(KeyPress('s', ModifierKeys::commandModifier, 0));
      result.setActive(!fScumbler->IsPlaying());
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


    case CommandIds::kViewPlugins:
    {
      result.setInfo("View plugins...", String::empty, category, 0);
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
       this->New();
    }
    break;

    case CommandIds::kOpen:
    {
      this->Open();
    }
    break;

    case CommandIds::kSave:
    {
      this->Save();
    }
    break;

    case CommandIds::kSaveAs:
    {
      this->SaveAs();
    }
    break;

    case CommandIds::kConfigAudio:
    {
      this->ConfigureAudio();
    }
    break;

    case CommandIds::kViewPlugins:
    {
      this->ViewPlugins();

    }
    break;


          

  }
  return retval;
}

