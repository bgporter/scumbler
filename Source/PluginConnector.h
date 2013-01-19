// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginConnector
#define h_PluginConnector

#include "../JuceLibraryCode/JuceHeader.h"

#include "ScumblerDefs.h"

class PluginConnector
{
public:
   /**
    * Do-nothing constructor.
    */
   PluginConnector()
   {
      // this space intentionally left blank.
   };

   /**
    * Do-nothing destructor
    */
   virtual ~PluginConnector()
   {
      // this space intentionally left blank.
   };

   /**
     * \name Connect
     * \brief Connect a source node to a destination node in the graph.
     * @return tk::Result code indicating success or the reason for failure.
     */
    virtual tk::Result Connect(NodeId source, NodeId dest) = 0;


    /**
     * Remove a connection between two nodes in the graph. Because we're not 
     * allowing any complex topologies now, we follow the same logic that was 
     * used to connect 
     * @param  source The node that's providing audio 
     * @param  dest   The node that's receiving audio.
     * @return tk::Result code indicating success or the reason for failure.
     */
    virtual tk::Result Disconnect(NodeId source, NodeId dest) = 0;

    /**
     * Insert 'newNode' in the graph inbetween 'before' and 'after'.
     * @param  before  The node that 'newNode' should be inserted after. The 
     *                 special value Scumbler::kInput can be used to connect 
     *                 newNode to the input node.
     * @param  newNode id of the node to connect in the middle. It should 
     *                 already have been added to the graph with AddProcessor
     * @param  after   The node that 'newNode' should be inserted before. The 
     *                 special value Scumbler::kOutput can be used to connect the 
     *                 newNode to the Scumbler's output.
     * @return         tk::Result
     */
    virtual tk::Result InsertBetween(NodeId before, NodeId newNode, NodeId after) = 0;

    /**
     * Disconnect the node `nodetoRemove` that's connected between `before` and 
     * `after`. The `nodeToRemove` is still owned by the graph and can be reconnected
     * elsewhere, unless the `deleteNode` argument is true.
     * @param  before       The node that must be connected to `nodeToRemove` as its
     *                      input. After this operation, `before` will be connected to 
     *                      `after`.
     * @param  nodeToRemove The node that we're trying to remove.
     * @param  after        The node that must be connected to `nodeToRemove`
     *                      as its output. After this operation, `before` will 
     *                      be connected to `after`.
     * @param  deleteNode   Should the node be deleted from the graph after it's 
     *                      disconnected?
     * @return              tk::Result
     */
    virtual tk::Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode=false) = 0;

    /**
     * Insert the provided AudioProcessor object into the Scumbler's filter 
     * graph.  The Scumbler takes ownership of the object, and it should 
     * **not** be deleted elsewhere.
     * @param  p The audio processor.
     * @return the identifier of the node.
     */
    virtual NodeId  AddProcessor(AudioProcessor* p) = 0;

    /**
     * Remove a node from the graph. This should rarely be needed outside the class 
     * that implements it. An obvious use is when we load a plugin but aren't able to
     * insert it into the graph. This *shouldn't* happen. But just in case.
     * @param  node Id of the node to delete.
     * @return      Success or Fail.
     */

    virtual tk::Result DeleteNode(NodeId node) = 0;


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
   virtual NodeId LoadPlugin(const PluginDescription& description, String& errorMessage) = 0;

};

#endif  // this MUST be the last line in this file.
