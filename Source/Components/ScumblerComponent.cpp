/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  10 Dec 2012 10:15:28pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ScumblerComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
extern ApplicationCommandManager* gCommandManager;  
//[/MiscUserDefs]

//==============================================================================
ScumblerComponent::ScumblerComponent (Scumbler* scumbler)
    : Component ("The Scumbler"),
      fScumbler(scumbler),
      fNewTrackButton (0)
{
    addAndMakeVisible (fNewTrackButton = new TextButton ("new track"));
    fNewTrackButton->setButtonText ("+");
    fNewTrackButton->addListener (this);
    fNewTrackButton->setColour (TextButton::buttonColourId, Colour (0xffcccce2));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ScumblerComponent::~ScumblerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (fNewTrackButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ScumblerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3f3f3f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ScumblerComponent::resized()
{
    fNewTrackButton->setBounds (32, 456, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ScumblerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == fNewTrackButton)
    {
        //[UserButtonCode_fNewTrackButton] -- add your button handler code here..
        //[/UserButtonCode_fNewTrackButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

ApplicationCommandTarget* ScumblerComponent::getNextCommandTarget()
{
  return this->findFirstTargetParentComponent();

}

void ScumblerComponent::getAllCommands(Array<CommandID>& commands)
{
  const CommandID ids[] = {
    CommandIds::kNew,
    CommandIds::kOpen,
    CommandIds::kSave,
    CommandIds::kSaveAs,
    CommandIds::kPlay,
    //CommandIds::kPause,
    //CommandIds::kRewind,
    //CommandIds::kToggleRecord,
    //CommandIds::kAddTrack
  };

  commands.addArray(ids, numElementsInArray(ids));

}

void ScumblerComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
  String category = "General";
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
  }
}

bool ScumblerComponent::perform(const InvocationInfo& info)
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

    case CommandIds::kPlay:
    {
      fScumbler->TogglePlay();
      // tell the command manager something has changed. This will make it 
      // re-query us with getCommandInfo() and set the menu text to display either 
      // 'Play' or 'Pause'
      gCommandManager->commandStatusChanged();
    }
    break;
  }
  return retval;
}

void ScumblerComponent::changeListenerCallback(ChangeBroadcaster* source)
{

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ScumblerComponent" componentName="The Scumbler"
                 parentClasses="public Component, public ApplicationCommandTarget, public ChangeListener"
                 constructorParams="Scumbler* scumbler" variableInitialisers="fScumbler(scumbler)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff3f3f3f"/>
  <TEXTBUTTON name="new track" id="afec3388486bebff" memberName="fNewTrackButton"
              virtualName="" explicitFocusOrder="0" pos="32 456 24 24" bgColOff="ffcccce2"
              buttonText="+" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
