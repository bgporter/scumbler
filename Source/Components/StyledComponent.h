// Copyright (c) 2013 Bg Porter

// !!! license

#ifndef h_StyledComponent
#define h_StyledComponent

#include "JuceHeader.h"
#include "UiStyle.h"


class StyledComponent : public Component, public ChangeListener
{
public:
   StyledComponent(UiStyle* style);
   ~StyledComponent();
   void changeListenerCallback (ChangeBroadcaster *source);

protected:
   UiStyle* fStyle;

};

#endif // this MUST be the last line in this file.