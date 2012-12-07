// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Scumbler
#define h_Scumbler

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScumblerDefs.h"

/**
 * @typedef NodeId
 *
 * We create a typedef to make it clear that we're referring to NodeIds.
 */
typedef uint32  NodeId;

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
class Scumbler : public ChangeBroadcaster
{
public:
#ifdef qUnitTests
  friend class ScumblerTest;
#endif    

  /**
   * \enum Result
   * Result codes that can be returned from our operations.
   */
  enum Result
  {
    kSuccess = 0,         /**< The operation succeeded */
    kFailure,             /**< (non-specific) failure */
    kIllegalConnection,   /**< Attempt to make an illegal connection */
    kNotConnected,        /**< Attempt to disconnect nodes that aren't connected. */
    kAlreadyConnected,    /**< Attempt to connect two nodes already connected. */
    kNoSourceNode,        /**< The specified source node doesn't exist. */
    kNoDestNode,          /**< The specified destination node doesn't exist. */
  };

  enum
  {
    kInput  = 0xffffffff,
    kOutput = 0xfffffffe
  };

  /**
    * Constructor
    * \param deviceManager A reference to an AudioDeviceManager object that's
    *     owned elsewhere in the application. 
    */
  Scumbler(AudioDeviceManager& deviceManager);

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
    */
    void Reset();

    /**
     * \name Connect
     * \brief Connect a source node to a destination node in the graph.
     * @return Scumbler::Result code indicating success or the reason for failure.
     */
    Result Connect(NodeId source, NodeId dest);


    /**
     * Remove a connection between two nodes in the graph. Because we're not 
     * allowing any complex topologies now, we follow the same logic that was 
     * used to connect 
     * @param  source The node that's providing audio 
     * @param  dest   The node that's receiving audio.
     * @return Scumbler::Result code indicating success or the reason for failure.
     */
    Result Disconnect(NodeId source, NodeId dest);

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
     * @return         Scumbler::Result
     */
    Result InsertBetween(NodeId before, NodeId newNode, NodeId after);

    /**
     * Disconnect the node `nodetoRemove` that's connected between `before` and 
     * `after`. The `nodeToRemove` is still owned by the graph and can be reconnected
     * elsewhere.
     * @param  before       The node that must be connected to `nodeToRemove` as its
     *                      input. After this operation, `before` will be connected to 
     *                      `after`.
     * @param  nodeToRemove The node that we're trying to remove.
     * @param  after        The node that must be connected to `nodeToRemove`
     *                      as its output. After this operation, `before` will 
     *                      be connected to `after`.
     * @return              Scumbler::Result
     */
    Result RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after);
    /**
     * Insert the provided AudioProcessor object into the Scumbler's filter 
     * graph.  The Scumbler takes ownership of the object, and it should 
     * **not** be deleted elsewhere.
     * @param  p The audio processor.
     * @return the identifier of the node.
     */
    NodeId  AddProcessor(AudioProcessor* p);

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

  typedef bool (AudioProcessorGraph::*fnPtr)(uint32, int, uint32, int);
  /**
   * Internal function that takes care of either connecting or disconnecting two nodes in an audio processor graph.
   * @param  source     node id of the  source node.
   * @param  dest       node id of the destination node.    
   * @param  connecting If true, we are connecting the nodes (else disconnecting)
   * @return            Scumbler::Result code indicating success or the reason for failure.
   */
  Result HandleConnection(NodeId source, NodeId dest, bool connecting);

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

   /**
    * Are we playing right now?
    */
   bool fPlaying;

   /**
    * node IDs for the input and output processors.
    */
   NodeId fInputNode;
   NodeId fOutputNode;

};


#endif // this must be the final line in this file.