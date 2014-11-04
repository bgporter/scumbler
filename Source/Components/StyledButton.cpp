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