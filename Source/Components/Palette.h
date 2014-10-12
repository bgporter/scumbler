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
   enum FactoryPalette
   {
      kEmptyPalette = 0, 
      kLightPalette, 
      kDarkPalette    
   };

   /**
    * Create the Palette object, optionally filling the color table in with our 
    * default values.
    */
   Palette(FactoryPalette p = kEmptyPalette);

   /**
    * Destroy the Palette.
    */
   ~Palette();


   /**
    * Make this palette use one of the hard-coded factory palettes that we support.
    * @param p [description]
    */
   void SetFactoryPalette(FactoryPalette p);

   /**
    * Set the specified color name to a desired value    .
    * @param  colorName Name of the thing being colored (e.g., as stored in an XML palette file)
    * @param  argb      32-but ARGB value to use for this color
    * @return           returns false if we don't know that color name
    */
   bool SetColor(const String& colorName, uint32 argb);

   /**
    * SetColor variant that lets us refer to other already set colors. 
    * @param  colorName         Idenitifier of the color id we want set
    * @param  existingColorName Identifier of an existing color.   
    * @return                   bool, success/fail.
    */
   bool SetColor(const String& colorName, const String& existingColorName);

   /**
    * Use a JUCE Colour object to define a palette color.
    * @param  colorName Color ID to set.
    * @param  color     JUCE Colour object to use
    * @return           success/fail
    */
   bool SetColor(const String& colorName, Colour color);


   /**
    * Given a color style name, return the current Juce Colour object for it. Should
    * return a very obviously wrong color if calling code requests something we don't 
    * know about.
    * @param  colorName Name of a UI element to color
    * @return           Colour object - in event of an error, returns an ugly color.
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
   // Application-level colors
   const String kAppBg("ApplicationBackground");
   const String kAppFg("ApplicationForeground");

   // Track component colors
   const String kTrackFg("TrackForeground");
   const String kTrackActiveOff("TrackActiveOff");
   const String kTrackActiveOn("TrackActiveOn");
   const String kTrackMuteOff("TrackMuteOff");
   const String kTrackMuteOn("TrackMuteOn");
   const String kTrackSoloOff("TrackSoloOff");
   const String kTrackSoloOn("TrackSoloOn");

   // Plugin slot colors
   const String kPluginFullActiveFg("FullPluginActiveForeground");
   const String kPluginFullActiveBg("FullPluginActiveBackground");
   const String kPluginFullInactiveFg("FullPluginInactiveForeground");
   const String kPluginFullInactiveBg("FullPluginInactiveBackground");
   const String kPluginActiveHover("PluginActiveHover");
   const String kPluginInactiveHover("PluginInactiveHover");
   const String kPluginActiveText("PluginActiveText");
   const String kPluginInactiveText("PluginInactiveText");

   // Waveform colors
   const String kLoopBg("LoopBackground");
   const String kLoopFg("LoopForeground");
   const String kLoopMonoWave("LoopMonoWave");
   const String kLoopLeftWave("LoopLeftWave");
   const String kLoopRightWave("LoopRightWave");
   const String kLoopTick("LoopTick");
   const String kLoopNow("LoopNow");

   // Transport colors
   const String kTransportBg("TransportBackground");
   const String kTransportFg("TransportForeground");
   const String kTransportPlayUpBorder("TransportPlayUpBorder");
   const String kTransportPlayUpFill("TransportPlayUpFill");
   const String kTransportPlayUpFg("TransportPlayUpFg");
   const String kTransportPlayUpFgFill("TransportPlayUpFgFill");
   const String kTransportPlayHoverBorder("TransportPlayHoverBorder");
   const String kTransportPlayHoverFill("TransportPlayHoverFill");
   const String kTransportPlayHoverFg("TransportPlayHoverFg");
   const String kTransportPlayHoverFgFill("TransportPlayHoverFgFill");
   const String kTransportPlayDownBorder("TransportPlayDownBorder");
   const String kTransportPlayDownFill("TransportPlayDownFill");
   const String kTransportPlayDownFg("TransportPlayDownFg");
   const String kTransportPlayDownFgFill("TransportPlayDownFgFill");


};

#endif // this MUST be the last line in this file.