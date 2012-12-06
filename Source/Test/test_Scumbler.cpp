// Copyright (c) 2012 Bg Porter

// NOTE that this file shouldn't be included in a project from the 
// compiler's point of view; when we're compiled with unit tests 
// enabled, this file should be #included into the associated file 
// in the directory above this one. We're only breaking this out into 
// a separate file to avoid cluttering our file under test.


#include "../Processors/Passthrough.h"


class ScumblerTest : public UnitTest
{
public:
   ScumblerTest() : UnitTest("Scumbler unit tests")
                  , fScumbler(nullptr)
   {

   };

   void initialise()
   {
      fScumbler = Scumbler::GetInstance();
      fScumbler->Reset();
      PassthroughProcessor* pp = new PassthroughProcessor(1, 1);
      delete pp;

   };

   void shutdown()
   {
      fScumbler->Reset();
      fScumbler = nullptr;

   };

   void runTest()
   {
      this->beginTest("Initial State");
      // better not be playing
      this->expect(!fScumbler->IsPlaying());
      // ...and after a reset, there should only be the input and output nodes.
      this->expect(2 == fScumbler->fGraph.getNumNodes());

      this->beginTest("Transport");
      fScumbler->Play();
      this->expect(fScumbler->IsPlaying());
      fScumbler->Pause();
      this->expect(!fScumbler->IsPlaying());

      this->beginTest("Connections");
      this->expect(2 == fScumbler->fGraph.getNumNodes());
      // test some bogus states first
      this->expect(Scumbler::kAlreadyConnected == \
         fScumbler->Connect(fScumbler->fInputNode, fScumbler->fOutputNode)); 
      this->expect(Scumbler::kIllegalConnection == \
         fScumbler->Connect(fScumbler->fOutputNode, fScumbler->fInputNode)); 

      this->expect(Scumbler::kNoSourceNode == \
         fScumbler->Connect(0xffffffff, fScumbler->fOutputNode)); 


      this->expect(Scumbler::kNoDestNode == \
         fScumbler->Connect(fScumbler->fInputNode, 0xFFFF)); 

   };

private:
   Scumbler* fScumbler;

};

static ScumblerTest test;


