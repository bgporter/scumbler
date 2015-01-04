// Copyright (c) 2014 Bg Porter


#include "StyledButton.h"

StyledButton::StyledButton(UiStyle* style)
:  fStyle(style)
{

}

StyledButton::~StyledButton()
{

}

void StyledButton::SetContext(const String& component, const String& button)
{
   fComponentContext = component;
   fButtonContext = button;
}


String StyledButton::FindPaletteKey(int state, const String& element) const
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