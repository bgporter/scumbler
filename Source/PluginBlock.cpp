// Copyright (c) 2013 Brett g Porter

#include "PluginBlock.h"

PluginBlock::PluginBlock(PluginConnector* scumbler, NodeId input, NodeId output, size_t size)
:  fScumbler(scumbler)
,  fInput(input)
,  fOutput(output)
{
   fPlugins.resize(size);
   // just in case someday the value of invalid node is something other than zero.
   for (size_t i = 0; i < size; ++i)
   {
      fPlugins.set(i, tk::kInvalidNode);
   }

   // we don't check the return. If they're already connected, that's cool. 
   // We should really be checking this, though, and throwing an exception
   // if there's a more serious problem.
   fScumbler->Connect(input, output);
}



PluginBlock::~PluginBlock()
{
   for (int i = 0; i < fPlugins.size(); ++i)
   {
      PluginInfo currentNode = fPlugins[i];
      if (tk::kInvalidNode != currentNode.id)
      {
         PluginInfo before = this->FindPluginBeforeIndex(i);
         PluginInfo after = this->FindPluginAfterIndex(i);
         // remove the current node and delete it. We can probably rewrite this in 
         // terms of our own DeleteNode() method, yet to be written.
         fScumbler->RemoveBetween(before.id, currentNode.id, after.id, true);
      }
   }
}

void PluginBlock::LoadXml(XmlElement* e, StringArray& errors, int formatVersion)
{
   XmlElement* slots = e->getChildByName("slots");
   if (slots)
   {
      int slotIndex = 0;
      forEachXmlChildElement(*slots, slot)
      {
         XmlElement* plugin = slot->getChildByName("PLUGIN");
         if (plugin)
         {
            PluginDescription pd;
            if (pd.loadFromXml(*plugin))
            {
               String msg;
               tk::Result result = this->LoadPluginAtIndex(slotIndex, pd, msg);
               if (tk::kSuccess == result)
               {
                  // the plugin is loaded in the right place; now we need to restore its 
                  // state as we last left it.
                  PluginInfo info = this->PluginInSlot(slotIndex);
                  XmlElement* state = slot->getChildByName("STATE");
                  if (state)
                  {
                     MemoryBlock m;
                     m.fromBase64Encoding(state->getAllSubText());
                     result = fScumbler->SetStateInformationForNode(info.id, m);
                     if (tk::kSuccess != result)
                     {
                        // report error.
                     }

                  }
                  else
                  {
                     // !!! report error.
                  }


               }
               else
               {
                  // !!! report error msg
               }

            }
            else
            {
               // !!! report the error
            }

         }
         ++slotIndex;
      }

   }
   else 
   {
      // !!! report missing slots element.
   }


}


XmlElement* PluginBlock::DumpXml(int formatVersion) const
{
   XmlElement* node = new XmlElement("slots");
   for (int i = 0; i < this->Size(); ++i)
   {
      XmlElement* slotNode = node->createNewChildElement("slot");
      PluginInfo slotInfo = this->PluginInSlot(i);
      if (tk::kInvalidNode != slotInfo.id)
      {
         // there's a real plugin here -- store its description and state.
         PluginDescription pd;
         tk::Result result = fScumbler->GetPluginDescriptionForNode(slotInfo.id, pd);
         if (tk::kSuccess == result)
         {
            slotNode->addChildElement(pd.createXml());
         }

         MemoryBlock m;
         result = fScumbler->GetStateInformationForNode(slotInfo.id, m);
         if (tk::kSuccess == result)
         {
            XmlElement* state = slotNode->createNewChildElement("state");
            state->addTextElement(m.toBase64Encoding());
         }
      }
   }

   return node;
}

int PluginBlock::Size() const
{
   return fPlugins.size();
}

tk::Result PluginBlock::InsertPluginAtIndex(PluginInfo plugin, int index)
{
   tk::Result retval = tk::kIndexOutOfRange;
   if (index >= 0 && index < this->Size())
   {
      if (tk::kInvalidNode == this->PluginInSlot(index).id)
      {
         // the index is in range, and the slot is empty.
         // Now, connect everything together:
         PluginInfo before = this->FindPluginBeforeIndex(index);
         PluginInfo after = this->FindPluginAfterIndex(index);
         if (tk::kInvalidNode != before.id && tk::kInvalidNode != after.id)
         {
            retval = fScumbler->InsertBetween(before.id, plugin.id, after.id);
            if (tk::kSuccess == retval)
            {
               fPlugins.set(index, plugin); 
   std::cout << "PluginBlock::InsertPluginAtIndex->sendChangeMessage" << std::endl;
               this->sendChangeMessage();
            }
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

tk::Result PluginBlock::RemovePluginAtIndex(int index, bool deleteNode)
{
   tk::Result retval = tk::kIndexOutOfRange;
   if (index >= 0 && index < this->Size())
   {
      PluginInfo plugin = this->PluginInSlot(index);
      if (tk::kInvalidNode != plugin.id)
      {
         PluginInfo before = this->FindPluginBeforeIndex(index);
         PluginInfo after = this->FindPluginAfterIndex(index);
         if (tk::kInvalidNode != before.id && tk::kInvalidNode != after.id)
         {
            retval = fScumbler->RemoveBetween(before.id, plugin.id, after.id, deleteNode);
            if (tk::kSuccess == retval)
            {
               fPlugins.set(index, PluginInfo());
            }
   std::cout << "PLuginBlock::RemovePluginAtIndex->sendChangeMessage" << std::endl;
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

PluginInfo PluginBlock::PluginInSlot(int index) const
{
   PluginInfo retval;
   if (index >= 0 && index < this->Size())
   {
      // We'll either return a real node if there's one in place, otherwise invalid node.
      // 
      retval = fPlugins[index];
   }
   return retval;
}

PluginInfo PluginBlock::LoadPlugin(const PluginDescription& description, String& errorMessage)
{
   NodeId newNode = fScumbler->LoadPlugin(description, errorMessage);
   return PluginInfo(newNode, description.name);
}


tk::Result PluginBlock::LoadPluginAtIndex(int index, const PluginDescription& desc, String& msg)
{
   tk::Result retval = tk::kSlotFull;
   if (tk::kInvalidNode == this->PluginInSlot(index).id)
   {
      retval = tk::kPluginLoadError;
      PluginInfo newPlugin = this->LoadPlugin(desc, msg);
      if (tk::kInvalidNode != newPlugin.id)
      {
         retval = this->InsertPluginAtIndex(newPlugin, index);
         // that may have failed for weird reasons. If so, we need to delete the plugin 
         // that we just loaded.
         if (tk::kSuccess != retval)
         {
            // we actually ignore the result here; we need to report failure, so 
            // reporting success here would be useless. If this fails, we have other problems.
            fScumbler->DeleteNode(newPlugin.id);
         }
      }
   }
   return retval;
}

AudioProcessorEditor* PluginBlock::GetEditorForIndex(int index, bool useGeneric)
{
   AudioProcessorEditor* retval = nullptr;

   PluginInfo plugin = this->PluginInSlot(index);
   if (tk::kInvalidNode != plugin.id)
   {
      retval = fScumbler->GetEditorForNode(plugin.id, useGeneric);
   }
   return retval;
}

PluginInfo PluginBlock::FindPluginBeforeIndex(int i)
{
   PluginInfo retval = PluginInfo(fInput, "Input");
   if (i > 0)
   {
      for (int j = i-1; j > -1; --j)
      {
         if (fPlugins[j].id != tk::kInvalidNode)
         {
            retval = fPlugins[j];
            break;
         }
      }
   }
   // either we found a valid node & have set retval or the only thing before this 
   // is the input. Return it.
   return retval;
}

PluginInfo PluginBlock::FindPluginAfterIndex(int i)
{
   PluginInfo retval = PluginInfo(fOutput, "Output");
   int size = fPlugins.size();
   if (i < (size-1))
   {
      for (int j = i+1; j < size; ++j)
      {
         if (fPlugins[j].id != tk::kInvalidNode)
         {
            retval = fPlugins[j];
            break;
         }
      }   
   }
   return retval;
}

KnownPluginList::SortMethod PluginBlock::GetPluginSortOrder() const
{
   return fScumbler->GetPluginSortOrder();
}

/// KEEP THIS SECTION AT THE END OF THE FILE.
#ifdef qUnitTests
#include "Test/test_PluginBlock.cpp"
#endif
