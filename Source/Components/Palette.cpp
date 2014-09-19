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

      this->SetColor(palette::kPluginFullActiveFg , 0xffffffff);
      this->SetColor(palette::kPluginFullActiveBg , 0xff000000);
      this->SetColor(palette::kPluginFullInactiveFg , 0xffffffff);
      this->SetColor(palette::kPluginFullInactiveBg , Colours::grey.getARGB());
      this->SetColor(palette::kPluginActiveHover , Colours::red.getARGB());
      this->SetColor(palette::kPluginInactiveHover , Colours::pink.getARGB());

      this->SetColor(palette::kLoopBg, 0xffffffff);
      this->SetColor(palette::kLoopFg,0xff000000);
      this->SetColor(palette::kLoopMonoWave, Colours::black.getARGB());
      this->SetColor(palette::kLoopLeftWave, 0x80ff0000);
      this->SetColor(palette::kLoopRightWave, 0x8000c480);
      this->SetColor(palette::kLoopTick, Colours::grey.getARGB());
      this->SetColor(palette::kLoopNow, Colours::red.getARGB());
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