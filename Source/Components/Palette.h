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
    * Version of Getcolor that can return false if the requested color name isn't 
    * in the palette table. 
    * @param  colorName key in the palette
    * @param  color     [out] Colour object (our ugly kErrorColor if needed)
    * @return           true if the color key was found.
    */
   bool GetColor(const String& colorName, Colour& color) const;



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

   const String kTransportResetUpBorder("TransportResetUpBorder");
   const String kTransportResetUpFill("TransportResetUpFill");
   const String kTransportResetUpFg("TransportResetUpFg");
   const String kTransportResetUpFgFill("TransportResetUpFgFill");

   const String kTransportResetHoverBorder("TransportResetHoverBorder");
   const String kTransportResetHoverFill("TransportResetHoverFill");
   const String kTransportResetHoverFg("TransportResetHoverFg");
   const String kTransportResetHoverFgFill("TransportResetHoverFgFill");

   const String kTransportResetDownBorder("TransportResetDownBorder");
   const String kTransportResetDownFill("TransportResetDownFill");
   const String kTransportResetDownFg("TransportResetDownFg");
   const String kTransportResetDownFgFill("TransportResetDownFgFill");
   
   const String kTransportResetDisabledBorder("TransportResetDisabledBorder");
   const String kTransportResetDisabledFill("TransportResetDisabledFill");
   const String kTransportResetDisabledFg("TransportResetDisabledFg");
   const String kTransportResetDisabledFgFill("TransportResetDisabledFgFill");

   const String kTrackActiveUpBorder("TrackActiveUpBorder");
   const String kTrackActiveUpFill("TrackActiveUpFill");
   const String kTrackActiveUpFg("TrackActiveUpFg");
   const String kTrackActiveUpFgFill("TrackActiveUpFgFill");

   const String kTrackActiveHoverBorder("TrackActiveHoverBorder");
   const String kTrackActiveHoverFill("TrackActiveHoverFill");
   const String kTrackActiveHoverFg("TrackActiveHoverFg");
   const String kTrackActiveHoverFgFill("TrackActiveHoverFgFill");

   const String kTrackActiveDownBorder("TrackActiveDownBorder");
   const String kTrackActiveDownFill("TrackActiveDownFill");
   const String kTrackActiveDownFg("TrackActiveDownFg");
   const String kTrackActiveDownFgFill("TrackActiveDownFgFill");

   const String kTrackActiveDisabledBorder("TrackActiveDisabledBorder");
   const String kTrackActiveDisabledFill("TrackActiveDisabledFill");
   const String kTrackActiveDisabledFg("TrackActiveDisabledFg");
   const String kTrackActiveDisabledFgFill("TrackActiveDisabledFgFill");

   const String kTrackActiveUpOnBorder("TrackActiveUpOnBorder");
   const String kTrackActiveUpOnFill("TrackActiveUpOnFill");
   const String kTrackActiveUpOnFg("TrackActiveUpOnFg");
   const String kTrackActiveUpOnFgFill("TrackActiveUpOnFgFill");

   const String kTrackActiveHoverOnBorder("TrackActiveHoverOnBorder");
   const String kTrackActiveHoverOnFill("TrackActiveHoverOnFill");
   const String kTrackActiveHoverOnFg("TrackActiveHoverOnFg");
   const String kTrackActiveHoverOnFgFill("TrackActiveHoverOnFgFill");

   const String kTrackActiveDownOnBorder("TrackActiveDownOnBorder");
   const String kTrackActiveDownOnFill("TrackActiveDownOnFill");
   const String kTrackActiveDownOnFg("TrackActiveDownOnFg");
   const String kTrackActiveDownOnFgFill("TrackActiveDownOnFgFill");

   const String kTrackActiveDisabledOnBorder("TrackActiveDisabledOnBorder");
   const String kTrackActiveDisabledOnFill("TrackActiveDisabledOnFill");
   const String kTrackActiveDisabledOnFg("TrackActiveDisabledOnFg");
   const String kTrackActiveDisabledOnFgFill("TrackActiveDisabledOnFgFill");

   const String kTrackMuteUpBorder("TrackMuteUpBorder");
   const String kTrackMuteUpFill("TrackMuteUpFill");
   const String kTrackMuteUpFg("TrackMuteUpFg");

   const String kTrackMuteHoverBorder("TrackMuteHoverBorder");
   const String kTrackMuteHoverFill("TrackMuteHoverFill");
   const String kTrackMuteHoverFg("TrackMuteHoverFg");
   
   const String kTrackMuteDownBorder("TrackMuteDownBorder");
   const String kTrackMuteDownFill("TrackMuteDownFill");
   const String kTrackMuteDownFg("TrackMuteDownFg");

   const String kTrackMuteDisabledBorder("TrackMuteDisabledBorder");
   const String kTrackMuteDisabledFill("TrackMuteDisabledFill");
   const String kTrackMuteDisabledFg("TrackMuteDisabledFg");

   const String kTrackMuteUpOnBorder("TrackMuteUpOnBorder");
   const String kTrackMuteUpOnFill("TrackMuteUpOnFill");
   const String kTrackMuteUpOnFg("TrackMuteUpOnFg");

   const String kTrackMuteHoverOnBorder("TrackMuteHoverOnBorder");
   const String kTrackMuteHoverOnFill("TrackMuteHoverOnFill");
   const String kTrackMuteHoverOnFg("TrackMuteHoverOnFg");
   
   const String kTrackMuteDownOnBorder("TrackMuteDownOnBorder");
   const String kTrackMuteDownOnFill("TrackMuteDownOnFill");
   const String kTrackMuteDownOnFg("TrackMuteDownOnFg");

   const String kTrackMuteDisabledOnBorder("TrackMuteDisabledOnBorder");
   const String kTrackMuteDisabledOnFill("TrackMuteDisabledOnFill");
   const String kTrackMuteDisabledOnFg("TrackMuteDisabledOnFg");

};



#endif // this MUST be the last line in this file.