// Copyright (c) 2013 Bg Porter

// !!! license

#ifndef h_StyledComponent
#define h_StyledComponent

#include "JuceHeader.h"
#include "UiStyle.h"


class StyledComponent : public Component, public ChangeListener
{
public:
   /**
    * Create the component and start listening to the UiStyle object for changes.
    */
   StyledComponent(UiStyle* style);

   ~StyledComponent();
   

   void changeListenerCallback (ChangeBroadcaster *source);

   /**
    * Handle new changes in the UiStyle -- should probably be called from both the 
    * derived class' ctor and also from the changeListener when the UiStyle lets 
    * us know that it's changed.
    */
   virtual void UpdateStyle();

protected:
   UiStyle* fStyle;

};

#endif // this MUST be the last line in this file.