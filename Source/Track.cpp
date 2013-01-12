// Copyright (c) 2012 Bg Porter

#include "Track.h"





Track::Track(Scumbler* owner, const String& name)
:  fOwner(owner)
,  fName(name)
,  fPreLoopPlugin(tk::kInvalidNode)
,  fLoop(tk::kInvalidNode)
,  fPostLoopPlugin(tk::kInvalidNode)
{

}

Track::~Track()
{
   
}


void Track::SetName(const String& name)
{
   fName = name;
}

String Track::GetName() const
{
   return fName;
}

/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif