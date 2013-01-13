// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginBlock
#define h_PluginBlock

#include "../JuceLibraryCode/JuceHeader.h"

#include "Scumbler.h"

class PluginBlock
{
public:
#ifdef qUnitTests
   // the class that tests us can have access to our private parts.
   friend class PluginBlockTest;
#endif

   /**
    * Create a block of plugin nodes that know how to connect between themselves 
    * and to the specified input and output nodes. On creation, it's empty and 
    * just connects its input and output nodes together. NOTE that the 
    * 'input' and 'output' parameters here aren't (necessarily) either the overall input 
    * or output nodes, just the I/O for this block of plugins.
    * 
    * @param scumbler The scumbler object that owns the filter graph
    * @param input ID of the node that provides input to this block. 
    * @param output ID of the node we need to send output from our last plugin
    * @param size Number of nodes that this block can hold. 
    */
   PluginBlock(Scumbler* scumbler, NodeId input, NodeId output, size_t size);

   /**
    * Destroy the plugin block, disconnecting and removing all plugins from the 
    * scumbler object first.
    */
   ~PluginBlock();



private:
   /**
    * Returns the ID of the previous plugin in the block (which may be the input node.)
    * Because this is private, we're assuming that 'i' is in a valid range for this block.        
    * @param  i Index of the node we're working with
    * @return   NodeId.
    */
   NodeId FindNodeBeforeIndex(int i);

   /**
    * Returns the id of the next plugin after this one (which may be output)
    * Because this is private, we're assuming that 'i' is in a valid range for this block.        
    * @param  i Index of the node we're working with.
    * @return   NodeId
    */
   NodeId FindNodeAfterIndex(int i);


private:   
   JUCE_DECLARE_NON_COPYABLE(PluginBlock);

   /**
    * The scumbler object owning the filter graph.
    */
   Scumbler* fScumbler;

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
   Array<NodeId> fPluginNodes;
};

#endif // this MUST be the last line in this file.
