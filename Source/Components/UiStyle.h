// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_UiStyle
#define h_UiStyle

#include "JuceHeader.h"


#include "Palette.h"


class UiStyle : public ChangeBroadcaster
{
public:
   /**
    * UiStyle ctor.
    */
   UiStyle(const String& fontName, Palette* palette);

   /**
    * UiStyle dtor -- note that this isn't a virtual dtor, so we shouldn't derive from this class.
    */
   ~UiStyle();


   /**
    * Replace the current palette for this style, destroying the old one.
    * @param palette Pointer to the new palette (which this style object then takes ownership of.)
    */
   void SetPalette(Palette* palette);

   /**
    * Return a JUCE Colour object for the specified string ID as set in the current Palette.
    * @param  color Color ID string (from Palette.h)
    * @return       Colour object (by value)
    */
   Colour GetColor(const String& color) const;

   /**
    * Return the name of the font to use
    * @return name of the font.
    */
   String GetFontName() const;




private:
   /**
    * name of the font to use to display text.
    */
   String fFontName;

   /**
    * pointer to the palette object to use. 
    */
   ScopedPointer<Palette> fPalette;
   
};



#endif // this MUST be the last line in this file.