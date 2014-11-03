// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_FlatButton
#define h_FlatButton

#include "JuceHeader.h"

#include "ComponentDefs.h"
#include "UiStyle.h"





/**
 * @class FlatButton
 *
 * Derived from the JUCE base Button class, with our own custom-drawn look and feel. 
 * Has centered text using the current app UI font (passed in with the UiStyle object)
 * and either bordered with a rect or ellipse using the full bounds of the button. 
 *
 * Takes its color settings from the palette that's in the UiStyle object. Each state of the button
 * has 3 colors:
 * - border
 * - fill 
 * - text
 *
 * 
 */
class FlatButton : public Button
{
public:

   struct Colors
   {
      Colour border;
      Colour fill; 
      Colour fg;
   };

   enum Shape
   {
      kRect = 0,
      kRound
   };


   /**
    * ctor.
    */
   FlatButton(const String& buttonName, FlatButton::Shape shape, UiStyle* style);

   /**
    * dtor
    */
   ~FlatButton();

   /**
    * Get new colors/fonts when they change.
    */
   void UpdateStyle();

   /**
    * Draw the button appropriately. Note that in addition to the two state parameters
    * passed in, we also need to be sensitive to the button's isEnabled()
    * and getToggleState() states.
    * @param g                 Graphics context to use.
    * @param isMouseOverButton Draw in 'hover' state?
    * @param isButtonDown      Is the button in its down state?
    */
   void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown);

private:
   UiStyle* fStyle;
   Shape    fShape;
   Colors   fColors[tk::kButtonStateCount];
};


#endif