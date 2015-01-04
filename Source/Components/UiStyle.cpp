// Copyright (c) 2014 Brett g Porter

#include "UiStyle.h"

namespace
{
   // an obnoxious purple to indicate the error that we don't have a 
   // palette object
   const uint32   kErrorColor = 0xffc300ff;
};
UiStyle::UiStyle(const String& fontName, Palette* palette)
:  fFontName(fontName)
,  fPalette(palette)
{

}


UiStyle::~UiStyle()
{
   this->removeAllChangeListeners();
   fPalette = nullptr;
}


void UiStyle::SetPalette(Palette* palette)
{
   fPalette = palette;
   this->sendChangeMessage();
}

Colour UiStyle::GetColor(const String& color) const
{
   jassert(fPalette);

   if (fPalette)
   {
      return fPalette->GetColor(color);
   }
   else
   {
      return Colour(kErrorColor);
   }
}


bool UiStyle::GetColor(const String& colorName, Colour& color) const
{
   jassert(fPalette);
   bool retval = false;
   if (fPalette)
   {
      retval = fPalette->GetColor(colorName, color);
   }
   return retval;
}

String UiStyle::GetFontName() const
{
   return fFontName;
}