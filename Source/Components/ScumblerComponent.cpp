/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  16 Dec 2012 10:55:55am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Cla/AddTrss Extensions"
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
    , fTransport(nullptr)
{


    //[UserPreSize]
    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
        // adding track(s)
    for (int i = 0; i < fScumbler->GetNumTracks(); ++i)
    {
      TrackComponent* tc = new TrackComponent(fScumbler->GetTrack(i));
      fTracks.add(tc);
      this->addAndMakeVisible(tc);
    }

    fTransport = new TransportComponent(fScumbler);
    this->addAndMakeVisible(fTransport);

    
    setSize (600, 400);


    // subscribe to change notifications coming from the scumbler object.
    fScumbler->addChangeListener(this);

    this->setWantsKeyboardFocus(true);
    this->grabKeyboardFocus();
    //[/Constructor]
}

ScumblerComponent::~ScumblerComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero(fTransport);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ScumblerComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ScumblerComponent::resized()
{
    //[UserResized] Add your own custom resize handling here..
    int trackCount = fTracks.size();
    for (int i = 0; i < trackCount; ++i)
    {
      TrackComponent* tc = fTracks.getUnchecked(i);
      this->SetTrackBounds(i, tc);
    }

    fTransport->setBounds(0, this->getHeight() - 50, this->getWidth(), 50);
    //[/UserResized]
}

void ScumblerComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

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

bool ScumblerComponent::keyPressed(const KeyPress& key)
{
   bool retval = false;
   if (key.isKeyCode(KeyPress::tabKey))
   {
      ModifierKeys mod = key.getModifiers();
      if (mod.isShiftDown())
      {
         fScumbler->ActivatePreviousTrack();
      }
      else
      {
         fScumbler->ActivateNextTrack();
      }
      retval = true;
   } 
   else
   {
      retval = false;
   }
   return retval;
}

void ScumblerComponent::SetTrackBounds(int index, TrackComponent* tc)
{
  int trackCount = fTracks.size();

  int height = this->getHeight() - fTransport->getHeight();
  int width = this->getWidth();
  int trackHeight = jmin(height/6, height/trackCount);
  int trackBlockHeight = trackHeight * trackCount;
  int trackBlockStartingY = (height/2) - (trackBlockHeight / 2);
  int trackLeft = 5;
  int trackWidth = width - (2*trackLeft);

  tc->setBounds(trackLeft, trackBlockStartingY + index * trackHeight, trackWidth, trackHeight);
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
