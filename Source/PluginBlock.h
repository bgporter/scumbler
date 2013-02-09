// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginBlock
#define h_PluginBlock

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginConnector.h"


/**
  * @struct PluginInfo
  * We store both the nodeId for a plugin (so we can manipulate it inside the Scumbler),
  * and also the name of it so we can display it in the UI. The PluginBlock seems
  * to be the appropriate place to link these two things. This struct is small and 
  * light enough that we can pass by value without being concerned.
  */
struct PluginInfo
{
public:
   PluginInfo(NodeId id_=tk::kInvalidNode, String name_=String::empty)
   : id(id_), name(name_){};
   NodeId id;
   String name;

};


/**
 * @class PluginBlock
 *
 * This class handles the connecting plugins in a chain using the simple linear 
 * track-like topology that we support. Given a pair of (source, dest) Nodes that 
 * pass audio to us, we can insert new plugins into any open 'slot' in this block, 
 * remove plugins, re-order them, delete them, etc, and be assured that everything 
 * ends up connected together as you'd want it to be.
 */


class PluginBlock : public ChangeBroadcaster
{
public:
#ifdef qUnitTests
   // the class that tests us can have access to our private parts.
   friend class PluginBlockTest;
#endif


   /**
    * Create a block of plugin nodes that know how to connect between themselves 
    * and to the specified input and output nodes. On creation, it's empty and 
    * just connects its input and output nodes together. **NOTE** that the 
    * 'input' and 'output' parameters here aren't (necessarily) either the overall input 
    * or output nodes, just the I/O for this block of plugins.
    * 
    * @param scumbler The scumbler object that owns the filter graph
    * @param input ID of the node that provides input to this block. 
    * @param output ID of the node we need to send output from our last plugin
    * @param size Number of nodes that this block can hold. 
    */
   PluginBlock(PluginConnector* scumbler, NodeId input, NodeId output, size_t size);

   /**
    * Destroy the plugin block, disconnecting and removing all plugins from the 
    * scumbler object first.
    */
   ~PluginBlock();

   /**
    * How many slots does this block have?
    * @return Integer, number of total slots (>= 1)
    */
   int Size() const;

   /**
    * Find out about the node that's living in s specified slot in this block
    * @param  index slot index.    
    * @return PluginInfo. `tk::kInvalidNode` indicates an empty (or out of range index) slot.
    */
   PluginInfo PluginInSlot(int index) const;

   /**
    * After an audio processor has been added to the graph, this places it at
    * the sepcified index in the block and sets the connections in place.
    * @param  node  Existing node to add. 
    * @param  index index (0..this->Size()) to insert the node at.
    * @return       Result of the operation. may fail if the node isn't valid, 
    *               if the index is out of range, or if that index is already occupied.
    */
   tk::Result InsertPluginAtIndex(PluginInfo node, int index);

   /**
    * Remove the processor node at the specified index, optionally deleting the plugin instance.
    * @param  index      Index of the node (0 <= index < this->Size())     
    * @param  deleteNode If true, delete the processor instance as well.
    * @return            Result of the operation.
    */
   tk::Result RemovePluginAtIndex(int index, bool deleteNode=false);

   /**
    * Load the specified plugin into the Scumbler (but don't yet connect it to anything.) 
    * On success, returns a new NodeId to the caller, which can then call InsertNodeAtIndex()
    * to put it where it wants it. Obviously, it's a good idea to call NodeInSlot() 
    * first to make sure that the slot you want to use is currently empty.
    * @param  description  A populated PluginDescription object, probably gotten from a menu
    *                      selection (or being restored from disk)   
    * @param  errorMessage If this fails, JUCE will put an error string in here for display .
    * @return              The NodeID of the new plug-in. If we fail, this is kInvalidNode.
    */
   PluginInfo LoadPlugin(const PluginDescription& description, String& errorMessage);

   /**
    * Convenience function. Does these things:
    *   - checks that the slot at index is free. If so:
    *   - tells the Scumbler to load the plugin. If that succeeds:
    *   - inserts the resulting node at the requested index, returns Success.
    * @param  index Node to insert a new plugin at. Must be empty.
    * @param  desc  description of the plugin to load.
    * @param  msg   Error message string, filled in on error.
    * @return       Result value
    */
   tk::Result LoadPluginAtIndex(int index, const PluginDescription& desc, String& msg);


private:
   /**
    * Returns the ID of the previous plugin in the block (which may be the input node.)
    * Because this is private, we're assuming that 'i' is in a valid range for this block.        
    * @param  i Index of the node we're working with
    * @return   NodeId.
    */
   PluginInfo FindPluginBeforeIndex(int i);

   /**
    * Returns the id of the next plugin after this one (which may be output)
    * Because this is private, we're assuming that 'i' is in a valid range for this block.        
    * @param  i Index of the node we're working with.
    * @return   NodeId
    */
   PluginInfo FindPluginAfterIndex(int i);


private:   
   JUCE_DECLARE_NON_COPYABLE(PluginBlock);

   /**
    * The scumbler object owning the filter graph.
    */
   PluginConnector* fScumbler;

   /**
    * Where our inupt comes from.
    */
   NodeId fInput;

   /**
    * Where our output goes to.
    */
   NodeId fOutput;

   /**
    * How many plugins can we hold?
    */
   size_t fSize;

   /**
    * Array holding the nodes of our plugins. NOTE that because the 
    * Array<T> type is resizable and we don't want to resize, make sure 
    * that the only operation we perform on the array is set().
    */
   Array<PluginInfo> fPlugins;
};

#endif // this MUST be the last line in this file.
