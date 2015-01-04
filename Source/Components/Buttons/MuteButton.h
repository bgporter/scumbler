// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_MuteButton
#define h_MuteButton

#include "JuceHeader.h"

/**
 * rectangular button with a lower-case Helvetica "m" on it for muting a track. 
 * Native dimensions are 32x24 px.
 *
 * Notes on how this was created:
 * 1. Image created in Inkscape
 * 2. Selected text, converted object to path
 * 3. save as plain SVG (not Inkscape SVG)
 * 4. edit SVG text (replace double-quotes w single quotes, pull out unnecessary stuff)
 * 5. clean up for header file (backslash line continuations, etc.)
 */


const String kMuteButton("<?xml version='1.0' standalone='no'?>\
<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.1//EN' \
  'http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd'>\
<svg version='1.1' baseProfile='full' xmlns='http://www.w3.org/2000/svg' \
   xmlns:xlink='http://www.w3.org/1999/xlink'>\
  <g transform='translate(0,-1028.3622)'>\
    <rect\
       width='30.651215'\
       height='22.626909'\
       x='0.65996957'\
       y='1029.058'\
       id='rect2989'\
       style='fill:#{bg-fill};fill-opacity:{bg-fill-opacity};stroke:#{bg-stroke};stroke-opacity:{bg-stroke-opacity};stroke-width:3;stroke-miterlimit:4;stroke-dasharray:none' />\
    <g id='text3759'\
       style='font-size:16px;font-style:normal;font-variant:normal;font-weight:bold;font-stretch:normal;text-align:center;line-height:100%;letter-spacing:0px;word-spacing:0px;writing-mode:lr-tb;text-anchor:middle;fill:#{fg-fill};fill-opacity:{fg-fill-opacity};stroke:none;font-family:Sans;-inkscape-font-specification:Sans Bold'>\
      <path\
         d='m 17.085937,1037.5458 c 0.354156,-0.5417 0.773427,-0.9531 1.257812,-1.2344 0.489572,-0.2864 1.02603,-0.4297 1.609375,-0.4297 1.005195,0 1.770819,0.3099 2.296875,0.9297 0.526027,0.6198 0.789047,1.5208 0.789063,2.7031 l 0,5.3282 -2.8125,0 0,-4.5625 c 0.0052,-0.068 0.0078,-0.1381 0.0078,-0.211 0.0052,-0.073 0.0078,-0.1771 0.0078,-0.3125 -1.3e-5,-0.6198 -0.09116,-1.0677 -0.273438,-1.3437 -0.182304,-0.2813 -0.476574,-0.4219 -0.882812,-0.4219 -0.531261,0 -0.942719,0.2187 -1.234375,0.6562 -0.286469,0.4375 -0.434906,1.0704 -0.445313,1.8985 l 0,4.2969 -2.8125,0 0,-4.5625 c -7e-6,-0.9688 -0.08334,-1.5912 -0.25,-1.8672 -0.166673,-0.2813 -0.463548,-0.4219 -0.890625,-0.4219 -0.536464,0 -0.950526,0.2213 -1.242187,0.6641 -0.291671,0.4375 -0.437505,1.0651 -0.4375,1.8828 l 0,4.3047 -2.8125005,0 0,-8.75 2.8125005,0 0,1.2812 c 0.343745,-0.4948 0.736974,-0.8672 1.179687,-1.1172 0.447911,-0.25 0.940098,-0.375 1.476563,-0.375 0.604159,0 1.138012,0.1459 1.601562,0.4375 0.463533,0.2917 0.815095,0.7005 1.054688,1.2266'\
         id='path3783' />\
    </g>\
</g>\
</svg>");


#endif