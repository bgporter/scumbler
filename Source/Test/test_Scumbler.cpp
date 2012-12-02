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
   {
      fScumbler = Scumbler::GetInstance();

   };

   void initialise()
   {

   };

   void shutdown()
   {

   };

   void runTest()
   {

   };

private:
   Scumbler* fScumbler;

};

static ScumblerTest test;


