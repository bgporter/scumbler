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
                  , fProc(nullptr)
   {

   };

   void initialise()
   {
      fScumbler = Scumbler::GetInstance();
      fScumbler->Reset();

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

      this->beginTest("Basic Connections");
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
      // test disconnections.
      this->expect(fScumbler->fGraph.isConnected(fScumbler->fInputNode, 
         fScumbler->fOutputNode));
      this->expect(Scumbler::kSuccess == fScumbler->Disconnect(
         fScumbler->fInputNode, fScumbler->fOutputNode));
      this->expect(!fScumbler->fGraph.isConnected(fScumbler->fInputNode, 
         fScumbler->fOutputNode));
      this->expect(Scumbler::kSuccess == fScumbler->Connect(
         fScumbler->fInputNode, fScumbler->fOutputNode));
      this->expect(fScumbler->fGraph.isConnected(fScumbler->fInputNode, 
         fScumbler->fOutputNode));

      this->beginTest("Complex connections");
      // create the processors that we'll be passing audio through.
      PassthroughProcessor* proc = new PassthroughProcessor(1, 1);
      this->expect(1 == proc->getNumInputChannels());
      this->expect(1 == proc->getNumOutputChannels());
      PassthroughProcessor* proc2 = new PassthroughProcessor(2,2);
      this->expect(2 == proc2->getNumInputChannels());
      this->expect(2 == proc2->getNumOutputChannels());

      //  add the 1-in 1-out node to the graph & connect it in the middle.
      this->expect(2 == fScumbler->fGraph.getNumNodes());
      uint32 node = fScumbler->AddProcessor(proc);
      Scumbler::Result r = fScumbler->InsertBetween(
         Scumbler::kInput, node, Scumbler::kOutput);
      this->expect(Scumbler::kSuccess == r);
      this->expect(3 == fScumbler->fGraph.getNumNodes());

      uint32 node2 = fScumbler->AddProcessor(proc2);
      r = fScumbler->InsertBetween(
         Scumbler::kInput, node2, Scumbler::kOutput);
      this->expect(Scumbler::kNotConnected == r);
      r = fScumbler->InsertBetween(node, node2, Scumbler::kOutput);
      this->expect(Scumbler::kSuccess == r);
      this->expect(4 == fScumbler->fGraph.getNumNodes());

      this->beginTest("Disconncections");
      r = fScumbler->RemoveBetween(Scumbler::kInput, node2, Scumbler::kOutput);
      this->expect(Scumbler::kNotConnected == r);

      r = fScumbler->RemoveBetween(Scumbler::kInput, node, node2);
      this->expect(Scumbler::kSuccess == r);
      this->expect(4 == fScumbler->fGraph.getNumNodes());

      r = fScumbler->RemoveBetween(Scumbler::kInput, node2, Scumbler::kOutput, true);
      this->expect(Scumbler::kSuccess == r);
      this->expect(3 == fScumbler->fGraph.getNumNodes());
   };

private:
   Scumbler* fScumbler;
};

static ScumblerTest test;


