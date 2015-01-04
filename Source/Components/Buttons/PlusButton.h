// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_PlusButton
#define h_PlusButton

#include "JuceHeader.h"


/**
 * Button with a "+" inside it, used to add a track
 */
const String kPlusButton("<?xml version='1.0' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' \
  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg' \
   xmlns:xlink='http://www.w3.org/1999/xlink'>\
   <ellipse cx='16' cy='16' rx='16' ry='16' \
      style='fill:#{bg-fill};fill-opacity:{bg-fill-opacity};stroke:#{bg-stroke};stroke-opacity:{bg-stroke-opacity};stroke-width:3'/>\
   <path d='M 14 8 L 18 8 18 14 24 14 24 18 18 18 18 24 14 24 14 18 8 18 8 14 14 14 Z'\
      style='stroke:#{fg-stroke};stroke-opacity:{fg-stroke-opacity};fill:#{fg-fill};fill-opacity:{fg-fill-opacity}'/>\
</svg>");



const String kXButton("<?xml version='1.0' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' \
  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg' \
   xmlns:xlink='http://www.w3.org/1999/xlink'>\
   <ellipse cx='16' cy='16' rx='16' ry='16' \
      style='fill:#{bg-fill};fill-opacity:{bg-fill-opacity};stroke:#{bg-stroke};stroke-opacity:{bg-stroke-opacity};stroke-width:3'/>\
   <g transform='rotate(45 16 16)' > \
   <path d='M 14 8 L 18 8 18 14 24 14 24 18 18 18 18 24 14 24 14 18 8 18 8 14 14 14 Z'\
      style='stroke:#{fg-stroke};stroke-opacity:{fg-stroke-opacity};fill:#{fg-fill};fill-opacity:{fg-fill-opacity}'/>\
   </g> \
</svg>");

#endif