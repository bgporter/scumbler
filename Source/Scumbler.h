// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_Scumbler
#define h_Scumbler

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScumblerDefs.h"

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
    Result Connect(uint32 source, uint32 dest);


    /**
     * Remove a connection between two nodes in the graph. Because we're not 
     * allowing any complex topologies now, we follow the same logic that was 
     * used to connect 
     * @param  source The node that's providing audio 
     * @param  dest   The node that's receiving audio.
     * @return Scumbler::Result code indicating success or the reason for failure.
     */
    Result Disconnect(uint32 source, uint32 dest);

    /**
     * Insert the provided AudioProcessor object into the Scumbler's filter 
     * graph.  The Scumbler takes ownership of the object, and it should 
     * **not** be deleted elsewhere.
     * @param  p The audio processor.
     * @return the identifier of the node.
     */
    uint32  AddProcessor(AudioProcessor* p);

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
  Result HandleConnection(uint32 source, uint32 dest, bool connecting);

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
   uint32 fInputNode;
   uint32 fOutputNode;

};


#endif // this must be the final line in this file.