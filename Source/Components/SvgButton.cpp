// Copyright (c) 2014 Bg Porter


#include "SvgButton.h"



SvgButton::SvgButton(const String& buttonName, const String& normal, UiStyle* style)
:  DrawableButton(buttonName, DrawableButton::ImageStretched)
,  fStyle(style)
{
   // make sure we don't let JUCE draw an outline when we're 'on'
   this->setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);

   for (int i = kNormal; i < kButtonImageCount; ++i)
   {
      fButtonImages.insert(i, nullptr);
   }

   // we must have at least a normal image
   jassert(normal != String::empty);
   this->AddButtonImage(kNormal, normal);
}


SvgButton::~SvgButton()
{
   // deliberately blank
}

void SvgButton::SetContext(const String& component, const String& button)
{
   fComponentContext = component;
   fButtonContext = button;
}


bool SvgButton::AddButtonImage(int imageIndex, const String& buttonText)
{
   // NOTE a bit of weirdness here -- above, we say that the button has to have 
   // a kNormal image, but we don't test here to make sure that they're not setting
   // kNormal to String::empty (or something else invalid.) Have to trust myself.
   // Also, you - hypocrite reader!
    bool retval = false;
   if (imageIndex >= kNormal && imageIndex < kButtonImageCount)
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
   for (int buttonState=kNormal; buttonState < kButtonImageCount; ++buttonState)
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


   ScopedPointer<Drawable> normal(this->CreateDrawable(kNormal));
   ScopedPointer<Drawable> hover(this->CreateDrawable(kHover));
   ScopedPointer<Drawable> down(this->CreateDrawable(kDown));
   ScopedPointer<Drawable> disabled(this->CreateDrawable(kDisabled));
   ScopedPointer<Drawable> normalOn(this->CreateDrawable(kNormalOn));
   ScopedPointer<Drawable> hoverOn(this->CreateDrawable(kHoverOn));
   ScopedPointer<Drawable> downOn(this->CreateDrawable(kDownOn));
   ScopedPointer<Drawable> disabledOn(this->CreateDrawable(kDisabledOn));
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



String SvgButton::FindPaletteKey(int state, const String& element) const
{
   const char* const states[] = {"Up", "Hover", "Down", "Disabled", "UpOn", 
      "HoverOn", "DownOn", "DisabledOn", nullptr};
   StringArray stateNames(states);

   String stateName = stateNames[state];
   Colour color;

   // try increasingly less specific key names, starting with fully specified
   String key = fComponentContext + fButtonContext + stateName + element;
   if (!fStyle->GetColor(key, color))
   {
      // ...then trying a component-level key...
      key = fComponentContext + stateName + element;
      if (!fStyle->GetColor(key, color))
      {
         // then an app-wide key...
         key = stateName + element;
         if (!fStyle->GetColor(key, color))
         {
            // ...and then give up -- we'll use default app fg/bg colors
            if (element == "Fill")
            {
               key = "ApplicationBackground";
            }
            else
            {
               key = "ApplicationForeground";
            }
         }
      }
   }

   return key;

}