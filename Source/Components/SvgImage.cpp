// Copyright (c) 2014 Brett g Porter

#include "SvgButton.h"

#include "UiStyle.h"


namespace
{
   struct SvgColor
   {
     String rgb;
     String opacity;
   };

   const uint32 kRgbMask(0x00FFFFFF);

   void ArgbToSvg(Colour c, SvgColor* svg)
   {
     uint32 argb = c.getARGB();
     uint32 rgb = argb & kRgbMask;
     float alpha = c.getFloatAlpha();

     svg->rgb = String::toHexString(static_cast<int>(rgb)).paddedLeft('0', 6);
     svg->opacity = String(alpha);

   }
}


SvgButton::SvgButton(const String& source)
:fSource(source)
{
   
}

SvgButton::~SvgButton()
{
   
}

void SvgButton::SetPaletteEntry(const String& svgKey, const String& paletteKey)
{
   fColorTable[svgKey] = paletteKey;
}

Drawable* SvgButton::Create(UiStyle* style)
{
   // make a writable copy...
   String svg(fSource);

   std::map<String, String>::iterator it;

   // walk through each color table association and replace the template items 
   // with correctly formatted color data.
   for (it = fColorTable.begin(); it != fColorTable.end(); ++it)
   {
      String svgItem = it->first;
      String palette = it->second;

      SvgColor color;

      ArgbToSvg(style->GetColor(palette), &color);

      svg = svg.replace("{" + svgItem + "}", color.rgb);
      svg = svg.replace("{" + svgItem + "-opacity}", color.opacity);
   }

   std::cout << svg << std::endl;

   XmlElement* x = XmlDocument::parse(svg);
   Drawable* retval = Drawable::createFromSVG(*x);
   delete x;
   return retval;

}