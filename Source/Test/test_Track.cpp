// Copyright (c) 2012 Bg Porter

// NOTE that this file shouldn't be included in a project from the 
// compiler's point of view; when we're compiled with unit tests 
// enabled, this file should be #included into the associated file 
// in the directory above this one. We're only breaking this out into 
// a separate file to avoid cluttering our file under test.

class TrackTest : public UnitTest
{
public:
   TrackTest() : UnitTest("Track unit tests")
   {

   }


   void initialise()
   {
   
   };

   void shutdown()
   {
   
   };

   void runTest()
   {
      Scumbler* scumbler = Scumbler::GetInstance();
      this->beginTest("Add/Delete");
      expect(0 == scumbler->GetNumTracks());
      expect(Scumbler::kSuccess == scumbler->AddTrack());
      expect(1 == scumbler->GetNumTracks());
      expect(Scumbler::kSuccess == scumbler->AddTrack());
      expect(2 == scumbler->GetNumTracks());
      scumbler->Reset();
      //  make sure that when we do a reset that the tracks also go away.
      expect(0 == scumbler->GetNumTracks());
      expect(Scumbler::kSuccess == scumbler->AddTrack());
      expect(Scumbler::kSuccess == scumbler->SetTrackName(0, "First"));
      expect(Scumbler::kSuccess == scumbler->AddTrack());
      expect(Scumbler::kSuccess == scumbler->SetTrackName(1, "Second"));
      this->beginTest("Names");
      String name;
      expect(Scumbler::kSuccess == scumbler->GetTrackName(0, name));
      expect(String("First") == name);
      expect(Scumbler::kSuccess == scumbler->GetTrackName(1, name));
      expect(String("Second") == name);

   }
private:

};

static TrackTest trackTest;