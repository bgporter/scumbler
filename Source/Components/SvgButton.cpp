// Copyright (c) 2014 Bg Porter


#include "SvgButton.h"



SvgButton::SvgButton(const String& normal, const String& hover, const String& down,
   const String& disabled, const String& normalOn, const String& hoverOn, 
   const String& downOn, const String& disabledOn)
{
   // we must have at least a normal image
   jassert(normal != String::empty);
   fButtonImages.insert(kNormal, new SvgImage(normal));
   // for the rest, we either insert an SvgImage or nullptr
   fButtonImages.insert(kHover, hover.isNotEmpty() ? new SvgImage(hover) : nullptr);
   fButtonImages.insert(kDown, down.isNotEmpty() ? new SvgImage(down) : nullptr);
   fButtonImages.insert(kDisabled, disabled.isNotEmpty() ? new SvgImage(disabled) : nullptr);
   fButtonImages.insert(kNormalOn, normalOn.isNotEmpty() ? new SvgImage(normalOn) : nullptr);
   fButtonImages.insert(kHoverOn, hoverOn.isNotEmpty() ? new SvgImage(hoverOn) : nullptr);
   fButtonImages.insert(kDownOn, downOn.isNotEmpty() ? new SvgImage(downOn) : nullptr);
   fButtonImages.insert(kDisabledOn, disabledOn.isNotEmpty() ? new SvgImage(disabledOn) : nullptr);

}


SvgButton::~SvgButton()
{
   // deliberately blank
}

void SvgButton::SetPaletteEntry(int imageIndex, const String& svgKey, const String& paletteKey)
{
   if (fButtonImages[imageIndex])
   {
      fButtonImages[imageIndex]->SetPaletteEntry(svgKey, paletteKey);
   }
   else 
   {
      jassert(false);
   }

}

void SvgButton::SetTemplateEntry(int imageIndex, const String& svgKey, const String& value)
{
   if (fButtonImages[imageIndex])
   {
      fButtonImages[imageIndex]->SetTemplateEntry(svgKey, value);
   }
   else 
   {
      jassert(false);
   }


}


void SvgButton::SetButtonImages(DrawableButton* button, UiStyle* style)
{
   button->setImages(this->CreateDrawable(kNormal),
                     this->CreateDrawable(kHover),
                     this->CreateDrawable(kDown),
                     this->CreateDrawable(kDisabled),
                     this->CreateDrawable(kNormalOn),
                     this->CreateDrawable(kHoverOn),
                     this->CreateDrawable(kDisabledOn)
                     );
}


Drawable* SvgButton::CreateDrawable(int imageIndex, UiStyle* style)
{
   return fButtonImages[imageIndex] ? fButtonImages[imageIndex]->Create(style) : nullptr;
}
