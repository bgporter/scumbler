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
      this->beginTest("Add tracks");
      expect(0 == scumbler->GetNumTracks());
      expect(tk::kSuccess == scumbler->AddTrack());
      expect(1 == scumbler->GetNumTracks());
      expect(tk::kSuccess == scumbler->AddTrack());
      expect(2 == scumbler->GetNumTracks());
      scumbler->Reset();
      //  make sure that when we do a reset that the tracks also go away.
      expect(0 == scumbler->GetNumTracks());

      this->beginTest("Deleting tracks");
      expect(tk::kSuccess == scumbler->AddTrack());
      Track* track = scumbler->GetTrack(1);
      expect(nullptr == track);
      track = scumbler->GetTrack(0);
      expect(nullptr != track);
      track->SetName("first");
      expect(String("first") == track->GetName());
      expect(tk::kSuccess == scumbler->AddTrack());
      track = scumbler->GetTrack(1);
      expect(nullptr != track);
      track->SetName("second");
      expect(tk::kFailure == scumbler->DeleteTrack(2));
      expect(tk::kSuccess == scumbler->DeleteTrack(0));
      expect(1 == scumbler->GetNumTracks());
      track = scumbler->GetTrack(0);
      expect(nullptr != track);
      expect(String("second") == track->GetName());

      this->beginTest("Name @ construction");
      scumbler->Reset();
      expect(tk::kSuccess == scumbler->AddTrack("zero"));
      expect(String("zero") == scumbler->GetTrack(0)->GetName());
      expect(tk::kSuccess == scumbler->AddTrack("one"));
      expect(String("one") == scumbler->GetTrack(1)->GetName());
      expect(tk::kSuccess == scumbler->AddTrack("two"));
      expect(String("two") == scumbler->GetTrack(2)->GetName());

      this->beginTest("Moving Tracks");
      scumbler->Reset();
      const char* kNames[]  ={"zero", "one", "two", "three", nullptr};
      Array<String> names(kNames );
      for (int i = 0; i < names.size(); ++i)
      {
         expect(tk::kSuccess == scumbler->AddTrack(names[i]));
      }
      for (int i = 0; i < names.size(); ++i)
      {
         expect(names[i] == scumbler->GetTrack(i)->GetName());
      }
      scumbler->MoveTrack(0, 1);
      expect(String("one") == scumbler->GetTrack(0)->GetName());      
      expect(String("zero") == scumbler->GetTrack(1)->GetName());      
      expect(String("two") == scumbler->GetTrack(2)->GetName());      
      expect(String("three") == scumbler->GetTrack(3)->GetName());      
      scumbler->MoveTrack(0, 1);
      expect(String("zero") == scumbler->GetTrack(0)->GetName());      
      expect(String("one") == scumbler->GetTrack(1)->GetName());      
      expect(String("two") == scumbler->GetTrack(2)->GetName());      
      expect(String("three") == scumbler->GetTrack(3)->GetName());      
      scumbler->MoveTrack(0, -1);
      expect(String("one") == scumbler->GetTrack(0)->GetName());      
      expect(String("two") == scumbler->GetTrack(1)->GetName());      
      expect(String("three") == scumbler->GetTrack(2)->GetName());      
      expect(String("zero") == scumbler->GetTrack(3)->GetName());      

      scumbler->MoveTrack(0, 4);
      expect(String("two") == scumbler->GetTrack(0)->GetName());      
      expect(String("three") == scumbler->GetTrack(1)->GetName());      
      expect(String("zero") == scumbler->GetTrack(2)->GetName());      
      expect(String("one") == scumbler->GetTrack(3)->GetName());      
      scumbler->MoveTrack(3, 0);
      scumbler->MoveTrack(3, 0);
      expect(String("zero") == scumbler->GetTrack(0)->GetName());      
      expect(String("one") == scumbler->GetTrack(1)->GetName());      
      expect(String("two") == scumbler->GetTrack(2)->GetName());      
      expect(String("three") == scumbler->GetTrack(3)->GetName());  

      scumbler->Reset();



   }
private:

};

static TrackTest trackTest;