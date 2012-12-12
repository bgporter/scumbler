// Copyright (c) 2012 Bg Porter

#include "Track.h"


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_Track.cpp"
#endif


Track::Track(Scumbler* owner)
:  fOwner(owner)
,  fPreLoopPlugin(kInvalidNode)
,  fLoop(kInvalidNode)
,  fPostLoopPlugin(kInvalidNode)
{

}

Track::~Track()
{
   
}