// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_ScumblerDefs
#define h_ScumblerDefs

// Later, we can set this to conditionally compile in only on debug 
// builds if we like.
#define qUnitTests

// If 'qSketch' is defined, our components may draw helpful debugging stuff 
// (sketching out bounds of a component, etc.)
#define qSketch

// A macro to make it easier to calling a member function using a 
// pointer to member function.

#define mCallMemberFn(object, ptrToMember)   ((object).*(ptrToMember))

// simple macro fn to round a float to an int
#define mRoundInt(f) static_cast<int>((f) + 0.5)

#define mMin(x, y) (x) < (y) ? (x) : (y)
#define mMax(x, y) (x) < (y) ? (y) : (x)



/**
 * @typedef NodeId
 *
 * We create a typedef to make it clear that we're referring to NodeIds.
 */
typedef uint32  NodeId;

namespace tk
{
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
      kNoTargetNode,        /**< The node you're trying to manipulate isn't there. */
      kIndexOutOfRange,     /**< The index value was too high or low. */
      kSlotFull,            /**< Attempting to put something into a full slot */  
      kPluginLoadError,     /**< error loading a plugin; check error string */
      kNotWhenPlaying,      /**< this operation can't happen when we're playing. */

   };

  /**
   * @enum Special NodeId values used in the system.
   */
   enum Node
   {
       kInvalidNode = 0,           /**< indicates a an empty node variable */
       kInput       = 0xffffffff,  /**< our input node. */
       kOutput      = 0xfffffffe   /**< our output node. */
   }; 

};



#endif // this must be the last line in this file.