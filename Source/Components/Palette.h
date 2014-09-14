// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_Palette
#define h_Palette

#include <map>

#include "JuceHeader.h"
#include "../XmlPersistent.h"


class Palette : public XmlPersistent
{
public:
   /**
    * Create the Palette object, optionally filling the color table in with our 
    * default values.
    */
   Palette(bool loadDefaults=true);

   /**
    * Destroy the Palette.
    */
   ~Palette();

   /**
    * Set the specified color name to a desired value    .
    * @param  colorName Name of the thing being colored (e.g., as stored in an XML palette file)
    * @param  argb      32-but ARGB value to use for this color
    * @return           returns false if we don't know that color name
    */
   bool SetColor(const String& colorName, uint32 argb);


   /**
    * Given a color style name, return the current Juce Colour object for it. Should
    * return a very obviously wrong color if calling code requests something we don't 
    * know about.
    * @param  colorName Name of a UI element to color
    * @return           Colour object.
    */
   Colour GetColor(const String& colorName) const;



   /**
    * Load this palette with a big chunk of color data stored as XML.
    * @param e             XMLElement containing our data.
    * @param errors        On return, may contain strings indicating errors
    * @param formatVersion (*not really sure what this is for! !!! *)
    */
   void LoadXml(XmlElement* e, StringArray& errors, int formatVersion);
   

   /**
    * Convert this palette object into an XmlElement object that can be saved.
    * @param  formatVersion Desired version number to use
    * @return               the populated XmlElement,
    */   
   XmlElement* DumpXml(int formatVersion) const;



private:
   std::map<String, uint32> fColorTable;

};

namespace palette
{
   const String kAppBg("ApplicationBackground");
   const String kAppFg("ApplicationForeground");

};

#endif // this MUST be the last line in this file.