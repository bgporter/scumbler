// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Scumbler
#define h_Scumbler

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginConnector.h"

class GainProcessor;
class Track;


/**
 * Convert a decibel value into the equivalent floating-point gain.
 * @param  db decibels, probably <= 0 (but not necessarily  ) 
 * @return    the corresponding floating-point amplitude gain value (e.g., -6.0 ~= 0.5)
 */
float DbToFloat(float db);

/**
 * convert a gain value from 0.0 .. 1.0 into the equivalent dB value
 * @param  gain Gain to convert
 * @return      decibel value
 */
float GainToDb(float gain);


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
                , public ChangeListener
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
    * Called when something needs to notify us of a change. Initially, 
    * this is only used when Track objects need to tell us that they
    * want to be deleted.
    * @param source object that's notifying us.
    */
   void changeListenerCallback(ChangeBroadcaster* source);

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
     * Set the scumbler's master output volume. 
     * @param volumeInDb dB, probably -96..0
     */
    void SetOutputVolume(float volumeInDb);

    /**
     * Get the current master output volume in dB
     * @return floating point dB value, probably in the range -96.0 .. 0.0
     */
    float GetOutputVolume() const;


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
     * Insert 'newNode' in the graph in between 'before' and 'after'.
     * @param  before  The node that 'newNode' should be inserted after. The 
     *                 special value Scumbler::kInput can be used to connect 
     *                 newNode to the input node.
     * @param  newNode id of the node to connect in the middle. It should 
     *                 already have been added to the graph with AddProcessor
     * @param  after   The node that 'newNode' should be inserted before. The 
     *                 special value Scumbler::kOutput can be used to connect the 
     *                 newNode to the Scumbler's output.
     * @param disconnect Do we need to disconnect before+after before inserting newNode?
     *                   This is useful when we're adding a track -- the input and output nodes
     *                   are no longer directly connected, so there's no reason to disconnect
     *                   them from each other first. 
     * @return         tk::Result
     */
    tk::Result InsertBetween(NodeId before, NodeId newNode, NodeId after, bool disconnect=true);

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
     * @param  reconnect    Should `before` and `after` be reconnected to each other after 
     *                      we disconnect `nodeToRemove`? (Useful when deleting a track -- we 
     *                      don't want the input and output to be directly connected again.)
     * @return              tk::Result
     */
    tk::Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode, 
      bool reconnect=true);



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
     * Set the track at the specified index as active, deactivating whichever track had
     * been active before that.
     * @param  index Index of the track to activate
     * @return       Success/fail status.
     */
    tk::Result ActivateTrack(int index);


    /**
     * Activate the next track in the list, wrapping around at the end. Doesn't
     * do anything if there aren't at least 2 tracks.
     * @return success/fail status
     */
    tk::Result ActivateNextTrack();

    /**
     * Activate the previous track, wrapping around. 
     * @return success/fail
     */
    tk::Result ActivatePreviousTrack();

    /**
     * Called from inside the Track::SetActive() implementation. The track object that's 
     * being activated lets the Scumbler know that this is happening. The Scumbler 
     * then looks for the current active track and deactivates it, updating the index of 
     * the currently active track
     * @param  trackBeingActivated Pointer to the track that's being activated.
     * @return                     success/fail.
     */
    tk::Result TrackIsActivating(Track* trackBeingActivated);

    /**
     * Return the index of the currently active track, or -1 if there's no
     * track currently active (shouldn't happen)
     * @return zero-based track index.
     */
    int GetActiveTrackIndex() const;


    /**
     * Set a track as being soloed. Pass in nullptr to have no tracks soloed. Individual 
     * tracks can process their output by calling scumbler->GetSoloTrack(). If that 
     * returns nullptr, no tracks are soloed, and they can output normally. If the return 
     * value is anything other than nullptr, they only output if 
     * this == scumbler->GetSoloTrack(). **NOTE** that we don't check that the pointer passed 
     * in is actually a Track object that we own. We assume that calling code is sane.
     * @param  trackToSolo pointer to the track that should be soloed, or nullptr to clear.
     * @return             success/failure.
     */
    tk::Result SoloTrack(Track* trackToSolo);

    /**
     * Get a pointer to the track that's currently being soloed (or nullptr)
     * @return Pointer to a track object.
     */
    Track* GetSoloTrack() const;


    /**
     * Reset the loop buffers of all tracks that the scumbler owns, clearing them, and 
     * putting all of their loop pointers at the beginning.
     * @return success/fail.
     */
    tk::Result ResetAllTracks(); 

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

   /**
    * Tell this plugin connector object how we'd like to have our list of plugins sorted when
    * the user displays a popup menu of them. 
    * @param sort SortMethod enum, one of defaultOrder, sortAlphabetically, sortByCategory,
    *             sortByManufacturer, sortByFileSystemLocation
    */
   void SetPluginSortOrder(KnownPluginList::SortMethod sort);

   /**
    * Return the enum that indicates how we should be displaying plugins in the menu.
    * @return desired sort order.
    */
   KnownPluginList::SortMethod GetPluginSortOrder() const;



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

   KnownPluginList::SortMethod fPluginSort;

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

   Track* fSoloTrack; 

   int fActiveTrackIndex;

   /**
    * Our current output volume in dB (default = 0)
    */
   float fOutputVolume;

   /**
    * A pointer to the gain processor -- we do not own this; it belongs 
    * to the filter graph.
    */
   GainProcessor* fOutputGain;
   /**
    * NodeId of the output GainProcessor.
    */
   NodeId fGainNode;

};


#endif // this must be the final line in this file.