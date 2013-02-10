// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Scumbler
#define h_Scumbler

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginConnector.h"

class Track;



/**
 * \class Scumbler
 * \brief The 'model' class for our MVC architecture in the Scumbler app
 *
 * This class owns/manages all of
 * - the audio IO and the filter graph
 * - the track objects
 * - links together the plugins owned by each track and the filter graph
 * - writing output to file, etc.
 */
class Scumbler  : public ChangeBroadcaster
                , public PluginConnector
{
public:
#ifdef qUnitTests
  friend class ScumblerTest;
#endif    
  // The Track class needs to be able to access the input and output nodes.
  friend class Track;




  /**
    * Constructor
    * \param deviceManager A reference to an AudioDeviceManager object that's
    *     owned elsewhere in the application. 
    */
  Scumbler(AudioDeviceManager& deviceManager, AudioPluginFormatManager& pluginManager);

   /**
    * dtor.
    */
   ~Scumbler();


   /**
    * \name TogglePlay
    * \brief Switches the scumbler object between the play and pause states.
    */
    void TogglePlay();
   /**
    * \name IsPlaying
    * \brief returns bool indicating whether the Scumbler is processing 
    *   audio right now.
    */
   bool IsPlaying() const;

   /**
    * \name Reset
    * \brief Do a complete reset on the processor graph. 
    *
    * After removing (and therefore deleting) all of the processor nodes that 
    * were added to the graph, creates new input & output processors and adds them.
    * **NOTE** that the Scumbler object should be reset after creation, after the 
    * Audio device manager is initialized.
    */
    void Reset();

    /**
     * @name PluginConnector operations
     */
    ///@{

   /**
     * \name Connect
     * \brief Connect a source node to a destination node in the graph.
     * @return tk::Result code indicating success or the reason for failure.
     */
    virtual tk::Result Connect(NodeId source, NodeId dest);


    /**
     * Remove a connection between two nodes in the graph. Because we're not 
     * allowing any complex topologies now, we follow the same logic that was 
     * used to connect 
     * @param  source The node that's providing audio 
     * @param  dest   The node that's receiving audio.
     * @return tk::Result code indicating success or the reason for failure.
     */
    tk::Result Disconnect(NodeId source, NodeId dest);

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
    tk::Result InsertBetween(NodeId before, NodeId newNode, NodeId after);

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
    tk::Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode=false);
    /**
     * Insert the provided AudioProcessor object into the Scumbler's filter 
     * graph.  The Scumbler takes ownership of the object, and it should 
     * **not** be deleted elsewhere.
     * @param  p The audio processor.
     * @return the identifier of the node.
     */
    NodeId  AddProcessor(AudioProcessor* p);


   /**
     * Remove a node from the graph. This should rarely be needed outside the class 
     * that implements it. An obvious use is when we load a plugin but aren't able to
     * insert it into the graph. This *shouldn't* happen. But just in case.
     * @param  node Id of the node to delete.
     * @return      Success or Fail.
     */

    tk::Result DeleteNode(NodeId node);

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
   NodeId LoadPlugin(const PluginDescription& description, String& errorMessage);


   /**
    * Return a pointer to an editor component for the requested Node in the graph.
    * @param  node       ID of the node we're interested in.
    * @param  useGeneric If true, use JUCE's generic editor component for this.
    * @return            Pointer to the editor component. Calling code is responsible
    *                    for cleaning this up.
    */
   AudioProcessorEditor* GetEditorForNode(NodeId node, bool useGeneric);

   /**
    * Fill in a PluginDescription object for the specified node. We use this when 
    * saving a Scumbler to disk.
    * @param  node The NodeId of the plugin we're interested in.
    * @param  desc Empty PluginDescription object to be filled in.   
    * @return      Success or Failure
    */
   tk::Result GetPluginDescriptionForNode(NodeId node, PluginDescription& desc);  

    ///@}
 
    /**
     * @name Track operations
     */
    ///@{
    
    /**
     * Returns the number of tracks that are currently in the Scumbler. 
     * @return [description]
     */
    int GetNumTracks() const;

    /**
     * Add a new (empty) track to the end of the array of track objects. 
     * @return kSuccess, ideally. If we are capping the number of possible tracks, 
     * we'll need to define a new tk::Result value(s) to reflect the 
     * reason for failure.
     */
    tk::Result AddTrack(const String& name = String::empty);

    /**
     * Remove a track from the scumbler and clean everything up.
     * @param  index index of the track to remove
     * @return       Success/failure.
     */
    tk::Result DeleteTrack(int index);

    /**
     * Move an existing track to a different index in the array. 
     * @param  fromIndex The current index of the track that we want to move
     * @param  toIndex   The index we want the track to occupy (using an index less 
     * than zero or greater than the highest index will move the track to the last position.)
     * @return           Success/failure.
     */
    tk::Result MoveTrack(int fromIndex, int toIndex);
    /**
     * Get a pointer to a specific track object that's owned by the Scumbler.
     * @param  index 0-based index of the track to retrieve
     * @return       pointer, nullptr if there's not a Track at that index.
     */
    Track* GetTrack(int index) const;

    ///@}
#ifdef qUnitTests
  /**
   * Get a pointer to the one and only scumbler object. Only used for unit tests.
   * @return pointer to the Scumbler.
   */
  static Scumbler* GetInstance();
#endif


protected:
  /**
    * \name Play
    * \brief Start audio through our processor graph.
    */
  void Play();

  /**
   * \name Pause
   * \brief Stop audio playback.
   */
  void Pause();

  /**
   * Typedef for a pointer-to-member-function that's exposed by the 
   * AudioProcessorGraph used to both connect and disconnect a single channel pair
   * of two nodes.
   * @param  uint32 node ID of source node
   * @param  int    source channel
   * @param  uint32 node ID of destination node
   * @param  int    destination channel index
   * @return        success/failure of the operation.
   */
  typedef bool (AudioProcessorGraph::*fnPtr)(uint32, int, uint32, int);
  

  /**
   * Internal function that takes care of either connecting or disconnecting two nodes in an audio processor graph.
   * @param  source     node id of the  source node.
   * @param  dest       node id of the destination node.    
   * @param  connecting If true, we are connecting the nodes (else disconnecting)
   * @return            tk::Result code indicating success or the reason for failure.
   */
  tk::Result HandleConnection(NodeId source, NodeId dest, bool connecting);

  /**
   * Use this to convert one of the special enum values `Scumbler::kInput` or 
   * `Scumbler::kOutput` into the actual node ids that 
   * @param  node value that may either be a real node id (< 0xfffffffe) or one of the
   *              special values for the Scumbler's pre-existing in/out nodes.
   * @return      an actual node id (possibly the same value that was passed in.)
   */
  NodeId HandleSpecialNode(NodeId node);

private:
   JUCE_DECLARE_NON_COPYABLE(Scumbler);

  enum
  {
    kPreEffects = 4,
    kPostEffects = 4
  };

   /**
    * The player object that pushes audio through the filter graph.
    */
   AudioProcessorPlayer fPlayer;

   /**
    * The graph of audio processors that do all the work.
    */
   AudioProcessorGraph  fGraph;

   /**
    * The audio hardware. 
    * In our initial version, we'll just initialize with defaults, and use 
    * the Juce-supplied AudioDeviceSelectorComponent class to let the user tweak 
    * settings.
    */
   AudioDeviceManager&   fDeviceManager;

   AudioPluginFormatManager& fPluginManager;

   /**
    * Are we playing right now?
    */
   bool fPlaying;

   /**
    * node IDs for the input and output processors.
    */
   NodeId fInputNode;
   NodeId fOutputNode;

   /**
    * Pointers to the track objects that we own. See the docs for OwnedArray 
    * (it takes ownership of the objects and deletes them when necessary.)
    */
   OwnedArray<Track>  fTracks; 

};


#endif // this must be the final line in this file.