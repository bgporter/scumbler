// Copyright (c) 2014 Bg Porter


#include "SvgButton.h"

#include "ComponentDefs.h"



SvgButton::SvgButton(const String& buttonName, const String& normal, UiStyle* style)
:  DrawableButton(buttonName, DrawableButton::ImageStretched)
,  StyledButton(style)
{
   // make sure we don't let JUCE draw an outline when we're 'on'
   this->setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);

   for (int i = tk::kButtonNormal; i < tk::kButtonStateCount; ++i)
   {
      fButtonImages.insert(i, nullptr);
   }

   // we must have at least a normal image
   jassert(normal != String::empty);
   this->AddButtonImage(tk::kButtonNormal, normal);
}


SvgButton::~SvgButton()
{
   // deliberately blank
}

#if 0
void SvgButton::SetContext(const String& component, const String& button)
{
   fComponentContext = component;
   fButtonContext = button;
}
#endif

bool SvgButton::AddButtonImage(int imageIndex, const String& buttonText)
{
   // NOTE a bit of weirdness here -- above, we say that the button has to have 
   // a kNormal image, but we don't test here to make sure that they're not setting
   // kNormal to String::empty (or something else invalid.) Have to trust myself.
   // Also, you - hypocrite reader!
    bool retval = false;
    if (imageIndex >= tk::kButtonNormal && imageIndex < tk::kButtonStateCount)
   {
      fButtonImages.insert(imageIndex,
                           buttonText.isNotEmpty() ? new SvgImage(buttonText) : nullptr);
      retval = true;
   }
   else
   {
      jassert(false);
   }

    return retval;
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


void SvgButton::UpdateStyle()
{
    for (int buttonState=tk::kButtonNormal; buttonState < tk::kButtonStateCount; ++buttonState)
   {
      if (fButtonImages[buttonState])
      {
         String bgStroke = this->FindPaletteKey(buttonState, "Border");
         String bgFill = this->FindPaletteKey(buttonState, "Fill");
         String fgStroke = this->FindPaletteKey(buttonState, "Fg");
         String fgFill = this->FindPaletteKey(buttonState, "FgFill");

         this->SetPaletteEntry(buttonState, "bg-stroke", bgStroke);
         this->SetPaletteEntry(buttonState, "bg-fill", bgFill);
         this->SetPaletteEntry(buttonState, "fg-stroke", fgStroke);
         this->SetPaletteEntry(buttonState, "fg-fill", fgFill);
      }
   }


   ScopedPointer<Drawable> normal(this->CreateDrawable(tk::kButtonNormal));
   ScopedPointer<Drawable> hover(this->CreateDrawable(tk::kButtonHover));
   ScopedPointer<Drawable> down(this->CreateDrawable(tk::kButtonDown));
   ScopedPointer<Drawable> disabled(this->CreateDrawable(tk::kButtonDisabled));
   ScopedPointer<Drawable> normalOn(this->CreateDrawable(tk::kButtonNormalOn));
   ScopedPointer<Drawable> hoverOn(this->CreateDrawable(tk::kButtonHoverOn));
   ScopedPointer<Drawable> downOn(this->CreateDrawable(tk::kButtonDownOn));
   ScopedPointer<Drawable> disabledOn(this->CreateDrawable(tk::kButtonDisabledOn));
   this->setImages(  normal,
                     hover,
                     down,
                     disabled,
                     normalOn,
                     hoverOn,
                     downOn,
                     disabledOn);
}


Drawable* SvgButton::CreateDrawable(int imageIndex)
{
   return fButtonImages[imageIndex] ? fButtonImages[imageIndex]->Create(fStyle) : nullptr;
}


