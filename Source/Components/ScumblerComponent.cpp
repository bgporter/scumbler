/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  16 Dec 2012 10:55:55am

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
    : Component ("The Scumbler")
    , fScumbler(scumbler)
    , fNewTrackButton (0)
{
    addAndMakeVisible (fNewTrackButton = new TextButton ("new track"));
    fNewTrackButton->setButtonText ("+");
    fNewTrackButton->addListener (this);
    fNewTrackButton->setColour (TextButton::buttonColourId, Colour (0xffcccce2));

    //addAndMakeVisible (fTrackComponent = new TrackComponent(nullptr));
    //fTrackComponent->setName ("base track");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    // subscribe to change notifications coming from the scumbler object.
    fScumbler->addChangeListener(this);
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
    int trackCount = fTracks.size();
    for (int i = 0; i < trackCount; ++i)
    {
      TrackComponent* tc = fTracks.getUnchecked(i);
      this->SetTrackBounds(i, tc);
    }

    //[/UserResized]
}

void ScumblerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == fNewTrackButton)
    {
        //[UserButtonCode_fNewTrackButton] -- add your button handler code here..
        fScumbler->AddTrack();
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
    CommandIds::kAddTrack,
#ifdef qUnitTests
    CommandIds::kRunUnitTests
#endif
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

    case CommandIds::kAddTrack:
    {
      result.setInfo("Add Track", "Add a new audio track to the scumbler",
        "Audio", 0);
      result.defaultKeypresses.add(KeyPress('a', ModifierKeys::commandModifier, 0));

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
    case CommandIds::kRunUnitTests:
    {
#ifdef qUnitTests
      // Unsubscribe this component from change notifications so we don't go
      // crazy when the tests are running
      fScumbler->removeChangeListener(this);
      UnitTestRunner runner;
      // run all tests even if there are failures.
      runner.setAssertOnFailure(false);
      runner.runAllTests();
      // hook us back up to the notifications.
      fScumbler->addChangeListener(this);
#endif
    }
    break;


  }
  return retval;
}

void ScumblerComponent::changeListenerCallback(ChangeBroadcaster* source)
{
  if (source == fScumbler)
  {
    // if the number of tracks has changed, we need to refresh things.
    int trackCount = fScumbler->GetNumTracks();
    int trackDelta = trackCount - fTracks.size();
    if (trackDelta != 0)
    {
      // obviously, if the delta is > 0, we've added a track, if < 0, 
      // a track has been deleted.
      if (trackDelta > 0)
      {
        // adding track(s)
        for (int i = 0; i < trackDelta; ++i)
        {
          TrackComponent* tc = new TrackComponent();
          fTracks.add(tc);
          this->addAndMakeVisible(tc);
        }

      }
      else
      {
        // deleting track(s)
        trackDelta *= -1;
        for (int i = 0; i < trackDelta; ++i)
        {
          // remove the last trackComponent from the list and delete it.
          fTracks.removeLast();
        }

      }

    }
    // Note that it's also possible that we just changed the order of tracks. That's
    // why we don't connect TrackComponents to tracks until after the numbers match, 
    // because we'll re-connect the two things here after every change callback, just
    // in case.
    

    for (int i = 0; i < trackCount; ++i)
    {
      TrackComponent* tc = fTracks.getUnchecked(i);
      Track* track = fScumbler->GetTrack(i);
      tc->ConnectToTrack(track);
      this->SetTrackBounds(i, tc);
    }

    this->repaint();
  }

}


void ScumblerComponent::SetTrackBounds(int index, TrackComponent* tc)
{
  int trackCount = fTracks.size();

  int height = this->getHeight();
  int width = this->getWidth();
  int trackHeight = jmin(height/6, height/trackCount);
  int trackLeft = 5;
  int trackWidth = width - (2*trackLeft);

  tc->setBounds(trackLeft, index * trackHeight, trackWidth, trackHeight);
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
  <GENERICCOMPONENT name="base track" id="b7e382f5ea847640" memberName="fTrackComponent"
                    virtualName="TrackComponent" explicitFocusOrder="0" pos="0 50 872 100"
                    class="TrackComponent" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
