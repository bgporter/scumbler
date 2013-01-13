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
}

PluginBlock::~PluginBlock()
{

}


NodeId PluginBlock::FindNodeBeforeIndex(int i)
{
   NodeId retval = fInput;
   if (i > 0)
   {
      for (int j = i-1; i > -1; --i)
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
