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
    * us know that it's changed. This is useful especially for updating components like 
    * buttons etc that remember their colors once they're set -- update them when we get
    * new palette information and they don't need to be re-set during each paint() operation.
    */
   virtual void UpdateStyle();

protected:
   UiStyle* fStyle;

};

#endif // this MUST be the last line in this file.