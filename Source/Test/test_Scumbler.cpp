// Copyright (c) 2012 Bg Porter

// NOTE that this file shouldn't be included in a project from the 
// compiler's point of view; when we're compiled with unit tests 
// enabled, this file should be #included into the associated file 
// in the directory above this one. We're only breaking this out into 
// a separate file to avoid cluttering our file under test.


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

   };

   void shutdown()
   {
      fScumbler->Reset();
      fScumbler = nullptr;
   };

   void runTest()
   {
      this->beginTest("Transport");
      this->expect(!fScumbler->IsPlaying());
      fScumbler->Play();
      this->expect(fScumbler->IsPlaying());
      fScumbler->Pause();
      this->expect(!fScumbler->IsPlaying());


   };

private:
   Scumbler* fScumbler;

};

static ScumblerTest test;


