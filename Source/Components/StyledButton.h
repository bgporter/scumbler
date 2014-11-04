// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_StyledButton
#define h_StyledButton

#include "JuceHeader.h"

#include "ComponentDefs.h"
#include "UiStyle.h"


/**
 * @class StyledButton
 *
 * Common code to work with our UiStyle/palette design, shared between the SvgButton and
 * FlatButton classes. NOTE that even though this has 'button' in its name, it's not derived
 * from any of the JUCE button classes; this needs to be mixed-in to your derived class.
 *
 * Also, unlike the StyledComponent class, this doesn't add a listener to the UiStyle object - 
 * we count on the component that owns us calling our UpdateStyle() method when it needs to.
 */
class StyledButton
{
public:
   StyledButton(UiStyle* style);

   virtual ~StyledButton();

   /**
    * Let this button know its context. To simplify styling, we use a convention-based
    * scheme to 
    * @param component Name of the component this button is a part of
    * @param button    'name' of this button as used in the palette name. Not necessarily
    *                   its name in source code 
    */
   void SetContext(const String& component, const String& button);


   /**
    * Do whatever needs to be done to adapt to changes in the palette/style
    */
   virtual void UpdateStyle() = 0;

protected:
   /**
    * Contains the algorithm that maps information about a button into potential 
    * palette keys, and looks into the currently active palette to find the best 
    * match of actually defined keys. 
    * @param  state   Button state (as an integer)
    * @param  element Which element of the button are we looking for (border, fill, etc)  
    * @return         String name of the palette key for this element.
    */
   String FindPaletteKey(int state, const String& element) const;   

protected:
   UiStyle* fStyle;
   String   fComponentContext;
   String   fButtonContext;

};



#endif