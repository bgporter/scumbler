// Copyright (c) 2012 Bg Porter.

// !!! License

#ifndef h_ComponentDefs
#define h_ComponentDefs

/**
 * A place to store common values that need to be kept in sync 
 */


// proportion of the track's width used by a plugin block.
const float kPluginBlockWidth = 0.25f;
// proportion of the track's width used by a plugin block.
const float kPluginBlockHeight = 0.8f;

// pixels before/after track contents.
const int kTrackMargin = 40;

// pixels to reserve under the waveform for duration/feedback controls.
const int kControlRowHeight = 30;

const float kPluginSlotHeight = 0.6f;


// dimensions of the knobs that we use.
const int kKnobWidth = 24;
const int kKnobHeight = 24;


namespace tk
{
   enum ButtonState
   {
      kButtonNormal = 0,
      kButtonHover,
      kButtonDown,
      kButtonDisabled,
      kButtonNormalOn,
      kButtonHoverOn,
      kButtonDownOn,
      kButtonDisabledOn,
      kButtonStateCount // keep this last.
   }; 
};

#endif