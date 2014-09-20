// Copyright (c) 2014 Bg Porter

#include "Palette.h"


namespace
{
   // a different horrible color to display an error
   const uint32 kErrorColor = 0xff3cff00;
}

Palette::Palette(bool loadDefaults)
{
   if (loadDefaults)
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
   }

}

Palette::~Palette()
{
  
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
   uint32 value;
   try
   {
      value = fColorTable.at(colorName);
   }
   catch (std::out_of_range)
   {
      value = kErrorColor;
      std::cout << "Unknown palette color requested - " << colorName << std::endl;
   }

   return Colour(value);

}

void Palette::LoadXml(XmlElement* e, StringArray& errors, int formatVersion)
{

}


XmlElement* Palette::DumpXml(int formatVersion) const
{
   return nullptr;
}