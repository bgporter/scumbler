// Copyright (c) 2014 Bg Porter

#include "Palette.h"


namespace
{
   // a different horrible color to display an error
   const uint32 kErrorColor = 0xff3cff00;
}

Palette::Palette(FactoryPalette p)
{
   if (p != kEmptyPalette)
   {
      this->SetFactoryPalette(p);
   }
}

Palette::~Palette()
{
  
}


void Palette::SetFactoryPalette(FactoryPalette p)
{
   switch (p)
   {
      case kDarkPalette:
      {
         this->SetColor(palette::kAppBg, Colours::black);
         this->SetColor(palette::kAppFg, Colours::white);

         this->SetColor(palette::kTrackFg, palette::kAppFg); 
         this->SetColor(palette::kTrackActiveOff, Colours::black); 
         this->SetColor(palette::kTrackActiveOn, Colours::green);
         this->SetColor(palette::kTrackMuteOff, Colours::black);
         this->SetColor(palette::kTrackMuteOn, Colours::green);
         this->SetColor(palette::kTrackSoloOff, Colours::black);
         this->SetColor(palette::kTrackSoloOn, Colours::red);

         this->SetColor(palette::kPluginFullActiveFg , palette::kAppFg);
         this->SetColor(palette::kPluginFullActiveBg , palette::kAppFg);
         this->SetColor(palette::kPluginFullInactiveFg , Colours::grey);
         this->SetColor(palette::kPluginFullInactiveBg , Colours::grey);
         this->SetColor(palette::kPluginActiveHover , Colours::red);
         this->SetColor(palette::kPluginInactiveHover , Colours::pink);
         this->SetColor(palette::kPluginActiveText , Colours::black);
         this->SetColor(palette::kPluginInactiveText , Colours::black);

         this->SetColor(palette::kLoopBg, 0xffffffff);
         this->SetColor(palette::kLoopFg,0xff000000);
         this->SetColor(palette::kLoopMonoWave, Colours::white);
         this->SetColor(palette::kLoopLeftWave, 0x80ff0000);
         this->SetColor(palette::kLoopRightWave, 0x8000c480);
         this->SetColor(palette::kLoopTick, Colours::grey);
         this->SetColor(palette::kLoopNow, Colours::red);

         this->SetColor(palette::kTransportBg, Colours::grey);
         this->SetColor(palette::kTransportFg, Colours::white);
      };
      break;

      case kLightPalette:
      default:
      {
         this->SetColor(palette::kAppBg, 0xffffffff);
         this->SetColor(palette::kAppFg, 0xff000000);

         this->SetColor(palette::kTrackFg, palette::kAppFg); 
         this->SetColor(palette::kTrackActiveOff, Colours::white); 
         this->SetColor(palette::kTrackActiveOn, Colours::green);
         this->SetColor(palette::kTrackMuteOff, Colours::white);
         this->SetColor(palette::kTrackMuteOn, Colours::green);
         this->SetColor(palette::kTrackSoloOff, Colours::white);
         this->SetColor(palette::kTrackSoloOn, Colours::red);

         this->SetColor(palette::kPluginFullActiveFg , palette::kAppFg);
         this->SetColor(palette::kPluginFullActiveBg , palette::kAppFg);
         this->SetColor(palette::kPluginFullInactiveFg , Colours::grey);
         this->SetColor(palette::kPluginFullInactiveBg , Colours::grey);
         this->SetColor(palette::kPluginActiveHover , Colours::red);
         this->SetColor(palette::kPluginInactiveHover , Colours::pink);
         this->SetColor(palette::kPluginActiveText , Colours::white);
         this->SetColor(palette::kPluginInactiveText , Colours::white);

         this->SetColor(palette::kLoopBg, 0xffffffff);
         this->SetColor(palette::kLoopFg,0xff000000);
         this->SetColor(palette::kLoopMonoWave, Colours::black);
         this->SetColor(palette::kLoopLeftWave, 0x80ff0000);
         this->SetColor(palette::kLoopRightWave, 0x8000c480);
         this->SetColor(palette::kLoopTick, Colours::grey);
         this->SetColor(palette::kLoopNow, Colours::red);

         this->SetColor(palette::kTransportBg, Colours::grey);
         this->SetColor(palette::kTransportFg, Colours::black);

         this->SetColor(palette::kTransportPlayUpBorder, palette::kAppFg);
         this->SetColor(palette::kTransportPlayUpFill, palette::kAppBg);
         this->SetColor(palette::kTransportPlayUpFg, palette::kAppFg);
         this->SetColor(palette::kTransportPlayUpFgFill, palette::kAppFg);

         this->SetColor(palette::kTransportPlayHoverBorder, Colours::red);
         this->SetColor(palette::kTransportPlayHoverFill, palette::kAppBg);
         this->SetColor(palette::kTransportPlayHoverFg, Colours::red);
         this->SetColor(palette::kTransportPlayHoverFgFill, Colours::red);

         this->SetColor(palette::kTransportPlayDownBorder, palette::kAppFg);
         this->SetColor(palette::kTransportPlayDownFill, palette::kAppFg);
         this->SetColor(palette::kTransportPlayDownFg, palette::kAppBg);
         this->SetColor(palette::kTransportPlayDownFgFill, palette::kAppBg);
      
         this->SetColor(palette::kTransportResetUpBorder, palette::kAppFg); 
         this->SetColor(palette::kTransportResetUpFill, palette::kAppBg);
         this->SetColor(palette::kTransportResetUpFg,  palette::kAppFg);
         this->SetColor(palette::kTransportResetUpFgFill, palette::kAppFg);

         this->SetColor(palette::kTransportResetHoverBorder,  Colours::red);
         this->SetColor(palette::kTransportResetHoverFill, palette::kAppBg);
         this->SetColor(palette::kTransportResetHoverFg, Colours::red);
         this->SetColor(palette::kTransportResetHoverFgFill, Colours::red);

         this->SetColor(palette::kTransportResetDownBorder, palette::kAppFg);
         this->SetColor(palette::kTransportResetDownFill, palette::kAppFg);
         this->SetColor(palette::kTransportResetDownFg, palette::kAppBg);
         this->SetColor(palette::kTransportResetDownFgFill, palette::kAppBg);
         
         this->SetColor(palette::kTransportResetDisabledBorder, Colours::lightgrey);
         this->SetColor(palette::kTransportResetDisabledFill, palette::kAppBg);
         this->SetColor(palette::kTransportResetDisabledFg, Colours::lightgrey);
         this->SetColor(palette::kTransportResetDisabledFgFill, Colours::lightgrey);

         this->SetColor(palette::kTrackActiveUpBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveUpFill, Colours::lightgrey);

         this->SetColor(palette::kTrackActiveHoverBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveHoverFill, Colours::lightgrey);
         
         this->SetColor(palette::kTrackActiveDownBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveDownFill, Colours::green);

         this->SetColor(palette::kTrackActiveUpOnBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveUpOnFill, Colours::green);

         this->SetColor(palette::kTrackActiveHoverOnBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveHoverOnFill, Colours::green);
         
         this->SetColor(palette::kTrackActiveDownOnBorder, palette::kAppFg);
         this->SetColor(palette::kTrackActiveDownOnFill, Colours::green);


      };
      break;

   };
}

bool Palette::SetColor(const String& colorName, uint32 argb)
{
   fColorTable[colorName] = argb;
   return true;
}


bool Palette::SetColor(const String& colorName, const String& existingColorName)
{
   Colour c = this->GetColor(existingColorName);
   return this->SetColor(colorName, c);
}

bool Palette::SetColor(const String& colorName, Colour color)
{
   return this->SetColor(colorName, color.getARGB());
}

Colour Palette::GetColor(const String& colorName) const
{
   Colour c;
   // use the other form of this method and ignore the return value.
   this->GetColor(colorName, c);
   return c;
}


bool Palette::GetColor(const String& colorName, Colour& color) const
{
   bool retval = true;
   uint32 value;
   try
   {
      value = fColorTable.at(colorName);
   }
   catch (std::out_of_range)
   {
      value = kErrorColor;
      //std::cout << "Unknown palette color requested - " << colorName << std::endl;
      retval = false;
   }

   color = Colour(value);
   return retval;
}

void Palette::LoadXml(XmlElement* e, StringArray& errors, int formatVersion)
{

}


XmlElement* Palette::DumpXml(int formatVersion) const
{
   return nullptr;
}