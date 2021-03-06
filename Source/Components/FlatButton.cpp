// Copyright (c) 2014 Bg Porter


#include "FlatButton.h"

FlatButton::FlatButton(const String& buttonName, FlatButton::Shape shape, UiStyle* style)
:  Button(buttonName)
,  StyledButton(style)
,  fShape(shape)
{

}

FlatButton::~FlatButton()
{

}

void FlatButton::UpdateStyle()
{

   // look up each combination of button state and button element (border/fill/fg)
   // and update the appropriate fColors struct entry.
   for (int buttonState=tk::kButtonNormal; buttonState < tk::kButtonStateCount; ++buttonState)
   {
      String bgStroke = this->FindPaletteKey(buttonState, "Border");
      String bgFill = this->FindPaletteKey(buttonState, "Fill");
      String fgStroke = this->FindPaletteKey(buttonState, "Fg");

      fColors[buttonState].border = fStyle->GetColor(bgStroke);
      fColors[buttonState].fill = fStyle->GetColor(bgFill);
      fColors[buttonState].fg = fStyle->GetColor(fgStroke);
   }
   fFontName = fStyle->GetFontName();
}

void FlatButton::paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
   tk::ButtonState state;

   bool isToggled = this->getToggleState();
   bool isEnabled = this->isEnabled();


   if (isMouseOverButton)
   {
      state = isToggled ? tk::kButtonHoverOn : tk::kButtonHover;
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

   g.setColour(fColors[state].border);
   g.drawRect(rect, 3.0);

   g.setColour(fColors[state].fg);
   g.setFont(Font(fFontName, this->getHeight()*0.8, Font::bold));
   //g.drawText(this->getButtonText(), rect, Justification::centred);
    g.drawFittedText(this->getButtonText(), 0, 0, this->getWidth(), this->getHeight(), Justification::centred, 1);

}