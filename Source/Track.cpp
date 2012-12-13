// Copyright (c) 2012 Bg Porter

#include "Track.h"





Track::Track(Scumbler* owner)
:  fOwner(owner)
,  fPreLoopPlugin(Scumbler::kInvalidNode)
,  fLoop(Scumbler::kInvalidNode)
,  fPostLoopPlugin(Scumbler::kInvalidNode)
{

}

Track::~Track()
{
   
}

/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif