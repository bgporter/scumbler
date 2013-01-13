// Copyright (c) 2013 Bg Porter

// NOTE that this file shouldn't be included in a project from the 
// compiler's point of view; when we're compiled with unit tests 
// enabled, this file should be #included into the associated file 
// in the directory above this one. We're only breaking this out into 
// a separate file to avoid cluttering our file under test.



class FakeConnector: public PluginConnector
{
public:

   FakeConnector()
   {
      fNextNodeId = tk::kInvalidNode + 1;
   };

   tk::Result Connect(NodeId source, NodeId dest)
   {
      tk::Result retval = tk::kAlreadyConnected;
      if (! this->AreConnected(source, dest))
      {
         fConnections.add(Connection(source, dest));
         retval = tk::kSuccess;
      }
      return retval;
   };


   tk::Result Disconnect(NodeId source, NodeId dest)
   {
      tk::Result retval = tk::kNotConnected;
      if (this->AreConnected(source, dest))
      {
         int index = this->FindConnection(source, dest);
         fConnections.remove(index);
         retval = tk::kSuccess;
      }
      return retval;
   };

   tk::Result InsertBetween(NodeId before, NodeId newNode, NodeId after)
   {
      tk::Result retval = tk::kFailure;

      retval = this->Disconnect(before, after);
      if (tk::kSuccess == retval)
      {
         retval = this->Connect(before, newNode);
         if (tk::kSuccess == retval)
         {
            retval = this->Connect(newNode, after);
         }
      }
      return retval;
   };

   tk::Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode)
   {
      tk::Result retval = this->Disconnect(before, nodeToRemove);
      if (tk::kSuccess == retval)
      {
         retval = this->Disconnect(nodeToRemove, after);
         if (tk::kSuccess == retval)
         {
            retval = this->Connect(before, after);
         }
      }
      return retval;
   };
  
   NodeId  AddProcessor(AudioProcessor* p)
   {
      return fNextNodeId++;
   };

   bool AreConnected(NodeId source, NodeId dest)
   {
      return this->FindConnection(source, dest) != -1;
   };

   int FindConnection(NodeId source, NodeId dest)
   {
      for (int i = 0; i < fConnections.size(); ++i)
      {
         if (fConnections[i].source == source && fConnections[i].dest == dest)
         {
            return i;
         }
      }
      return -1;
   };

   void Reset()
   {
      fConnections.clear(); 
   };

private:
   struct Connection
   {
      Connection(NodeId s=tk::kInvalidNode, NodeId d=tk::kInvalidNode)
      : source(s), dest(d)
      { /* blank */ };
      NodeId source;
      NodeId dest;
   };

   Array<Connection> fConnections;

   NodeId fNextNodeId;

};


class PluginBlockTest : public UnitTest
{
public:
   PluginBlockTest() : UnitTest("PluginBlock unit tests")
   {

   }


   void initialise()
   {
      fGraph = new FakeConnector();
      fInput = fGraph->AddProcessor(nullptr);
      fOutput = fGraph->AddProcessor(nullptr);
   
   };

   PluginBlock* Reset(int length)
   {
      fGraph->Reset();
      return new PluginBlock(fGraph, fInput, fOutput, length);
   }

   void shutdown()
   {
      delete fGraph;
      fGraph = nullptr;
   };

   void runTest()
   {
      beginTest("Finding preceding/following nodes.");
      ScopedPointer<PluginBlock> pb(this->Reset(4));
      // just checking the before/after logic. We're not actually adding anything
      // to the fake graph. 
      pb->fPluginNodes.set(1, 11);
      expect(fInput == pb->FindNodeBeforeIndex(1));
      expect(fOutput == pb->FindNodeAfterIndex(1));
      expect(fInput == pb->FindNodeBeforeIndex(0));
      expect(11 == pb->FindNodeAfterIndex(0));
      pb->fPluginNodes.set(3, 15);
      expect(11 == pb->FindNodeBeforeIndex(3));
      expect(15 == pb->FindNodeAfterIndex(1));
      expect(fOutput == pb->FindNodeAfterIndex(3));
      expect(11 == pb->FindNodeBeforeIndex(2));
      expect(15 == pb->FindNodeAfterIndex(2));

      // clear out for the next test
      pb = this->Reset(4);




   };

private:
   FakeConnector* fGraph;
   NodeId fInput;
   NodeId fOutput;

};


static PluginBlockTest test;