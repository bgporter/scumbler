// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_ResetButton
#define h_ResetButton

#include "JuceHeader.h"

const String kResetButton("<?xml version='1.0' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' \
  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg' \
   xmlns:xlink='http://www.w3.org/1999/xlink'>\
   <ellipse cx='16' cy='16' rx='16' ry='16' \
      style='fill:#{bg-fill};fill-opacity:{bg-fill-opacity};stroke:#{bg-stroke};stroke-opacity:{bg-stroke-opacity};stroke-width:3'/>\
   <path d='M 10 22 L 13 22 13 16 21 22 21 11 13 16 13 11 10 11 Z'\
      style='stroke:#{fg-stroke};stroke-opacity:{fg-stroke-opacity};fill:#{fg-fill};fill-opacity:{fg-fill-opacity}'/>\
</svg>");


#endif