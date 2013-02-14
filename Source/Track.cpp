// Copyright (c) 2012 Bg Porter

#include "Track.h"




Track::Track(Scumbler* owner, int preFxCount, int postFxCount, const String& name)
:  fScumbler(owner)
,  fName(name)
,  fPlaying(true)
,  fPreEffectCount(preFxCount)
,  fPreEffects(nullptr)
,  fPostEffectCount(postFxCount)
,  fPostEffects(nullptr)
,  fLoop(nullptr)
,  fLoopId(tk::kInvalidNode)
{
   // we need the input and output nodes that the Scumbler controls.
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);

   // create & insert the loop processor
   AudioProcessor* fLoop = new LoopProcessor(this);
   fLoopId = fScumbler->AddProcessor(fLoop);
   fScumbler->InsertBetween(input, fLoopId, output);

   // create the plugin blocks and hook them in.
   fPreEffects = new PluginBlock(fScumbler, input, fLoopId, fPreEffectCount);
   fPostEffects = new PluginBlock(fScumbler, fLoopId, output, fPostEffectCount);


}

Track::~Track()
{
   // delete the plugin blocks, and then remove the loop processor from the graph, deleting
   // the loop processor as we exit.
   fPreEffects = nullptr;
   fPostEffects = nullptr;
   NodeId input = fScumbler->HandleSpecialNode(tk::kInput);
   NodeId output = fScumbler->HandleSpecialNode(tk::kOutput);
   fScumbler->RemoveBetween(input, fLoopId, output, true);
}


void Track::SetName(const String& name)
{
   fName = name;
}

String Track::GetName() const
{
   return fName;
}

bool Track::IsPlaying() const
{
   return fPlaying && fScumbler->IsPlaying();
}

void Track::UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener)
{
   ChangeBroadcaster* sender;
   switch (target)
   {
      case kPreFx: sender = fPreEffects; break;

      case kTrack: sender = this; break;

      case kPostFx: sender = fPostEffects; break;

      case kLoop: sender = fLoop; break;

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