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