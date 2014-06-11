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

   tk::Result InsertBetween(NodeId before, NodeId newNode, NodeId after, bool disconnect)
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

   tk::Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode, bool reconnect)
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


   tk::Result DeleteNode(NodeId node)
   {
      return tk::kSuccess;
   }

   NodeId LoadPlugin(const PluginDescription& description, String& errorMessage)
   {
      NodeId retval = tk::kInvalidNode;
      errorMessage = String("Failure to load (unit test)");

      if (String("FAIL") != description.name)
      {
         retval = this->AddProcessor(nullptr);
      }
      return retval;
   }


   AudioProcessorEditor* GetEditorForNode(NodeId node, bool useGeneric)
   {
      return nullptr;
   }

   /**
    * Fill a memory block with the current state of the requested node.
    * @param  node id of the node we're interested in
    * @param  m    Memory block to fill
    * @return      success/fail.
    */
   tk::Result GetStateInformationForNode(NodeId node, MemoryBlock& m)
   {
      return tk::kFailure;
   }

    tk::Result SetStateInformationForNode(NodeId node, MemoryBlock& m)
    {
        return tk::kFailure;
    }
    
    
   /**
    * Fill in a PluginDescription object for the specified node. We use this when 
    * saving a Scumbler to disk.
    * @param  node The NodeId of the plugin we're interested in.
    * @param  desc Empty PluginDescription object to be filled in.   
    * @return      Success or Failure
    */
   tk::Result GetPluginDescriptionForNode(NodeId node, PluginDescription& desc)
   {
      return tk::kFailure;
   }



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
      pb->fPlugins.set(1, PluginInfo(11));
      expect(fInput == pb->FindPluginBeforeIndex(1).id);
      expect(fOutput == pb->FindPluginAfterIndex(1).id);
      expect(fInput == pb->FindPluginBeforeIndex(0).id);
      expect(11 == pb->FindPluginAfterIndex(0).id);
      pb->fPlugins.set(3, PluginInfo(15));
      expect(11 == pb->FindPluginBeforeIndex(3).id);
      expect(15 == pb->FindPluginAfterIndex(1).id);
      expect(fOutput == pb->FindPluginAfterIndex(3).id);
      expect(11 == pb->FindPluginBeforeIndex(2).id);
      expect(15 == pb->FindPluginAfterIndex(2).id);

      beginTest("test resetting the block");
      expect(tk::kInvalidNode == pb->PluginInSlot(0).id);
      expect(11 == pb->PluginInSlot(1).id);
      expect(tk::kInvalidNode == pb->PluginInSlot(2).id);
      expect(15 == pb->PluginInSlot(3).id);

      // clear out for the next test
      pb = this->Reset(4);
      for (int i = 0; i < pb->Size(); ++i)
      {
         expect(tk::kInvalidNode == pb->PluginInSlot(i).id);

      }

      beginTest("Insert tests");
      // create 4 node IDs that we can use in tests.
      NodeId a = fGraph->AddProcessor(nullptr);
      NodeId b = fGraph->AddProcessor(nullptr);
      NodeId c = fGraph->AddProcessor(nullptr);
      NodeId d = fGraph->AddProcessor(nullptr);

      // should start with input & output connected.
      expect(fGraph->AreConnected(fInput, fOutput));
      expect(tk::kSuccess == pb->InsertPluginAtIndex(a, 0));
      expect(fGraph->AreConnected(fInput, a));
      expect(fGraph->AreConnected(a, fOutput));
      expect(!fGraph->AreConnected(a, b));
      expect(!fGraph->AreConnected(fInput, fOutput));

      expect(tk::kSuccess == pb->InsertPluginAtIndex(b, 1));
      expect(tk::kSlotFull == pb->InsertPluginAtIndex(b, 1));
      expect(tk::kSuccess == pb->InsertPluginAtIndex(c, 2));
      expect(tk::kSuccess == pb->InsertPluginAtIndex(d, 3));
      expect(fGraph->AreConnected(fInput, a));
      expect(!fGraph->AreConnected(a, fOutput));
      expect(fGraph->AreConnected(a, b));
      expect(fGraph->AreConnected(b, c));
      expect(fGraph->AreConnected(c, d));
      expect(fGraph->AreConnected(d, fOutput));
      expect(a == pb->PluginInSlot(0).id);
      expect(b == pb->PluginInSlot(1).id);
      expect(c == pb->PluginInSlot(2).id);
      expect(d == pb->PluginInSlot(3).id);

      beginTest("Remove/reinsert tests");
      // now start switching things around.
      // pull out 'b' 
      expect(tk::kSuccess == pb->RemovePluginAtIndex(1));
      expect(tk::kNoTargetNode == pb->RemovePluginAtIndex(1));
      expect(fGraph->AreConnected(a, c));
      // pull out 'c'
      expect(tk::kSuccess == pb->RemovePluginAtIndex(2));
      // so now a should be connected to d
      expect(fGraph->AreConnected(a, d));
      expect(tk::kInvalidNode == pb->PluginInSlot(1).id);
      expect(tk::kInvalidNode == pb->PluginInSlot(2).id);
      // swap the old positions of b and c
      expect(tk::kSuccess == pb->InsertPluginAtIndex(b, 2));
      expect(fGraph->AreConnected(a, b));
      expect(fGraph->AreConnected(b, d));
      expect(tk::kSuccess == pb->InsertPluginAtIndex(c, 1));
      expect(fGraph->AreConnected(a, c));
      expect(fGraph->AreConnected(c, b));
      expect(a == pb->PluginInSlot(0).id);
      expect(c == pb->PluginInSlot(1).id);
      expect(b == pb->PluginInSlot(2).id);
      expect(d == pb->PluginInSlot(3).id);

      beginTest("Loading & inserting 'plugins'.");
      pb = this->Reset(4);
      PluginDescription desc;
      String msg = "";

      expect(tk::kSuccess == pb->LoadPluginAtIndex(0, desc, msg));
      expect(tk::kSuccess == pb->LoadPluginAtIndex(1, desc, msg));
      expect(tk::kSlotFull == pb->LoadPluginAtIndex(1, desc, msg));
      // force a failed plugin load.
      desc.name = "FAIL";
      expect(tk::kPluginLoadError == pb->LoadPluginAtIndex(2, desc, msg));


   };

private:
   FakeConnector* fGraph;
   NodeId fInput;
   NodeId fOutput;

};


static PluginBlockTest test;