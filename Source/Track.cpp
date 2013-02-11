// Copyright (c) 2012 Bg Porter

#include "Track.h"

// TODO: replace with the real loop processor.
#include "Processors/Passthrough.h"
#include "Processors/Loop.h"



Track::Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name)
:  fScumbler(owner)
,  fName(name)
,  fPreEffectCount(preFxCount)
,  fPreEffects(nullptr)
,  fLoop(tk::kInvalidNode)
,  fPostEffectCount(postFxCount)
,  fPostEffects(nullptr)
{
   // we need the input and output nodes that the Scumbler controls.
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);

   // create & insert the loop processor
   // (FOR NOW this is just the passthrough processor.)
   AudioProcessor* loop = new LoopProcessor();
   fLoop = fScumbler->AddProcessor(loop);
   fScumbler->InsertBetween(input, fLoop, output);

   // create the plugin blocks and hook them in.
   fPreEffects = new PluginBlock(fScumbler, input, fLoop, fPreEffectCount);
   fPostEffects = new PluginBlock(fScumbler, fLoop, output, fPostEffectCount);


}

Track::~Track()
{
   // delete the plugin blocks, and then remove the loop processor from the graph, deleting
   // the loop processor as we exit.
   fPreEffects = nullptr;
   fPostEffects = nullptr;
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);
   fScumbler->RemoveBetween(input, fLoop, output, true);
}


void Track::SetName(const String& name)
{
   fName = name;
}

String Track::GetName() const
{
   return fName;
}


void Track::UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener)
{
   ChangeBroadcaster* sender;
   switch (target)
   {
      case kPreFx: sender = fPreEffects; break;

      case kTrack: sender = this; break;

      case kPostFx: sender = fPostEffects; break;

      // assert on an error.
      default: jassert(false); break;
   }

   if (add)
   {
      sender->addChangeListener(listener);
   }
   else
   {
      sender->removeChangeListener(listener);
   }
}


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif