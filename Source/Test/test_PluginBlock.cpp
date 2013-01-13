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
   
   };

   void shutdown()
   {
   
   };

   void runTest()
   {
   };
};


static PluginBlockTest test;