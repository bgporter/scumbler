// Copyright (c) 2013 Brett g Porter

#include "PluginBlock.h"

PluginBlock::PluginBlock(PluginConnector* scumbler, NodeId input, NodeId output, size_t size)
:  fScumbler(scumbler)
,  fInput(input)
,  fOutput(output)
{
   fPluginNodes.resize(size);
   // just in case someday the value of invalid node is something other than zero.
   for (size_t i = 0; i < size; ++i)
   {
      fPluginNodes.set(i, tk::kInvalidNode);
   }

   // we don't check the return. If they're already connected, that's cool. 
   // We should really be checking this, though, and throwing an exception
   // if there's a more serious problem.
   fScumbler->Connect(input, output);
}



PluginBlock::~PluginBlock()
{
   for (int i = 0; i < fPluginNodes.size(); ++i)
   {
      NodeId currentNode = fPluginNodes[i];
      if (tk::kInvalidNode != currentNode)
      {
         NodeId before = this->FindNodeBeforeIndex(i);
         NodeId after = this->FindNodeAfterIndex(i);
         // remove the current node and delete it. We can probably rewrite this in 
         // terms of our own DeleteNode() method, yet to be written.
         fScumbler->RemoveBetween(before, currentNode, after, true);
      }
   }
}

int PluginBlock::Size() const
{
   return fPluginNodes.size();
}

tk::Result PluginBlock::InsertNodeAtIndex(NodeId node, int index)
{
   tk::Result retval = tk::kIndexOutOfRange;
   if (index >= 0 && index < this->Size())
   {
      if (tk::kInvalidNode == this->NodeInSlot(index))
      {
         // the index is in range, and the slot is empty.
         fPluginNodes.set(index, node); 
         // Now, connect everything together:
         NodeId before = this->FindNodeBeforeIndex(index);
         NodeId after = this->FindNodeAfterIndex(index);
         if (tk::kInvalidNode != before && tk::kInvalidNode != after)
         {
            retval = fScumbler->InsertBetween(before, node, after);
            this->sendChangeMessage();
         }
         else
         {
            // unknown weird failure.
            retval = tk::kFailure;
         }
      }
      else
      {
         retval = tk::kSlotFull;
      }
   }
   return retval;
}

tk::Result PluginBlock::RemoveNodeAtIndex(int index, bool deleteNode)
{
   tk::Result retval = tk::kIndexOutOfRange;
   if (index >= 0 && index < this->Size())
   {
      NodeId node = this->NodeInSlot(index);
      if (tk::kInvalidNode != node)
      {
         NodeId before = this->FindNodeBeforeIndex(index);
         NodeId after = this->FindNodeAfterIndex(index);
         if (tk::kInvalidNode != before && tk::kInvalidNode != after)
         {
            retval = fScumbler->RemoveBetween(before, node, after, deleteNode);
            if (tk::kSuccess == retval)
            {
               fPluginNodes.set(index, tk::kInvalidNode);
            }
            this->sendChangeMessage();
         }
         else
         {
            // unknown weird failure.
            retval = tk::kFailure;
         }
      }
      else
      {
         // this slot was empty.
         retval = tk::kNoTargetNode;
      }
   }  
   return retval; 
}

NodeId PluginBlock::NodeInSlot(int index) const
{
   NodeId retval = tk::kIndexOutOfRange;
   if (index >= 0 && index < this->Size())
   {
      // We'll either return a real node if there's one in place, otherwise invalid node.
      // 
      retval = fPluginNodes[index];
   }
   return retval;
}

NodeId PluginBlock::LoadPlugin(const PluginDescription& description, String& errorMessage)
{
   return fScumbler->LoadPlugin(description, errorMessage);
}


tk::Result PluginBlock::LoadPluginAtIndex(int index, const PluginDescription& desc, String& msg)
{
   tk::Result retval = tk::kSlotFull;
   if (tk::kInvalidNode == this->NodeInSlot(index))
   {
      retval = tk::kPluginLoadError;
      NodeId newNode = this->LoadPlugin(desc, msg);
      if (tk::kInvalidNode != newNode)
      {
         retval = this->InsertNodeAtIndex(newNode, index);
         // that may have failed for weird reasons. If so, we need to delete the plugin 
         // that we just loaded.
         if (tk::kSuccess != retval)
         {
            // we actually ignore the result here; we need to report failure, so 
            // reporting success here would be useless. If this fails, we have other problems.
            fScumbler->DeleteNode(newNode);
         }
      }
   }
   return retval;
}


NodeId PluginBlock::FindNodeBeforeIndex(int i)
{
   NodeId retval = fInput;
   if (i > 0)
   {
      for (int j = i-1; j > -1; --j)
      {
         if (fPluginNodes[j] != tk::kInvalidNode)
         {
            retval = fPluginNodes[j];
            break;
         }
      }
   }
   // either we found a valid node & have set retval or the only thing before this 
   // is the input. Return it.
   return retval;
}

NodeId PluginBlock::FindNodeAfterIndex(int i)
{
   NodeId retval = fOutput;
   int size = fPluginNodes.size();
   if (i < (size-1))
   {
      for (int j = i+1; j < size; ++j)
      {
         if (fPluginNodes[j] != tk::kInvalidNode)
         {
            retval = fPluginNodes[j];
            break;
         }
      }   
   }
   return retval;
}


/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_PluginBlock.cpp"
#endif
