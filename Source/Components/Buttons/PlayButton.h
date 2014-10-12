// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_PlayButton
#define h_PlayButton

#include "JuceHeader.h"

const String kPlayButton("<?xml version='1.0' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' \
  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg' \
   xmlns:xlink='http://www.w3.org/1999/xlink'>\
   <ellipse cx='32' cy='32' rx='32' ry='32' \
      style='fill:#{bg-fill};fill-opacity:{bg-fill-opacity};stroke:#{bg-stroke};stroke-opacity:{bg-stroke-opacity};stroke-width:3'/>\
   <polygon points='24,48 44,32 24,16   '\ 
      style='stroke:#{fg-stroke};stroke-opacity:{fg-stroke-opacity};fill:#{fg-fill};fill-opacity:{fg-fill-opacity}'/>\
</svg>");


#endif