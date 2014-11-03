// Copyright (c) 2014 Bg Porter


#include "FlatButton.h"

FlatButton::FlatButton(const String& buttonName, FlatButton::Shape shape, UiStyle* style)
:  Button(buttonName)
,  fStyle(style)
,  fShape(shape)
{

}

FlatButton::~FlatButton()
{

}

void FlatButton::UpdateStyle()
{
   fColors[tk::kButtonNormal].fill = Colours::red;
   fColors[tk::kButtonHover].fill = Colours::blue;
   fColors[tk::kButtonDown].fill = Colours::green;
   fColors[tk::kButtonDisabled].fill = Colours::grey;
}

void FlatButton::paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
   tk::ButtonState state;

   bool isToggled = this->getToggleState();
   bool isEnabled = this->isEnabled();

   //VERY temporary code to just get the mechanics of this button down.
   if (isMouseOverButton)
   {
      state = isToggled ? tk::kHoverButtonOn : tk::kHoverButton;
   }
   else if (isButtonDown)
   {
      state = isToggled ? tk::kButtonDownOn : tk::kButtonDown;
   }
   else if (!isEnabled)
   {
      state = isToggled ? tk::kButtonDisabledOn : tk::kButtonDisabled;
   }
   else
   {
      state = isToggled ? tk::kButtonNormalOn : tk::kButtonNormal;
   }


   Rectangle<float> rect = Rectangle<float>(0, 0, this->getWidth(), this->getHeight());
   g.setColour(fColors[state].fill);
   g.fillRect(rect);


}