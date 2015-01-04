// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_SvgImage
#define h_SvgImage

#include "JuceHeader.h"

#include <map>

// forward ref
class UiStyle;

/**
 * \class SvgImage
 *
 *  We want to be able to use SVG to define the design of our flat buttons. This class 
 *  lets us mark up SVG data to use as a template that can be styled at runtime using 
 *  palette data. 
 *
 *  Each button should have at least 4 palette colors associated with it:
 *  - bg-stroke (border)
 *  - bg-fill   (button fill color)
 *  - fg-stroke (shape/letter outline)
 *  - fg-fill   (shape/letter fill)
 *
 * (Note that in many cases, we'll expect the fg stroke and fill colors to be the same)
 * We mark these up in the SVG string like this:
 *  `<polygon points='6,12 11,8 6,4   ' style='fill:#{fg-fill};fill-opacity:{fg-fill-opacity}`
 *
 * Also note that SVG keeps color and opacity/alpha data separately, so for each of the colors
 * in the list above, there are both e.g. `bg-stroke` and `bg-stroke-opacity` template
 * placeholders.
 *
 * We keep the associations between the SVG entries and our palette identifiers in 
 * a map to adapt for future cases where there are more elements to be colored inside the
 * button (and these things may not actually *be* buttons, I guess. What this class really lets us
 * manage is palette-stylable SVG drawables.) 
 * 
 */
class SvgImage
{
public:
   /**
    * Create the object, keeping a const version of the original string.
    */
   SvgImage(const String& source);

   ~SvgImage();

   /**
    * Create an association between one of our template keys and a palette entry.
    * This should only need to be done once at startup time for each of these objects.
    * Each time the member fn `Create()` is called, we'll write the current color 
    * values from the palette into our SVG output.
    * @param svgKey     e.g. 'bg-stroke'
    * @param paletteKey e.g. 'TransportPlayHoverBorder'
    */
   void SetPaletteEntry(const String& svgKey, const String& paletteKey);   

   /**
    * We can also pass literal data into template entries in the SVG data -- 
    * perhaps useful for things like changing stroke-widths, transforms, etc
    * @param svgKey template key to set
    * @param value  Value to use for that template key.
    */
   void SetTemplateEntry(const String& svgKey, const String& value);

   /**
    * Create a new drawable object that uses current palette data from the UiStyle object
    * that we are passed.
    * @param  style UiStyle object with the active palette
    * @return       Pointer to a Drawable object that we can use. It's the caller's 
    *                       responsibility to clean this up.
    */
   Drawable* Create(UiStyle* style);



private:
   // original marked-up SVG data that we'll update with palette data at runtime.
   const String fSource;

   std::map<String, String> fColorTable;

   std::map<String, String> fTemplateMap;
};


#endif