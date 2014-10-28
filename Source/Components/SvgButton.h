// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_SvgButton
#define h_SvgButton

#include "JuceHeader.h"

#include "SvgImage.h"
#include "UiStyle.h"

class SvgButton : public DrawableButton
{
public:
   enum ImageType
   {
      kNormal = 0,
      kHover,
      kDown,
      kDisabled,
      kNormalOn,
      kHoverOn,
      kDownOn,
      kDisabledOn,
      kButtonImageCount // keep this last.
   };

   /**
    * Create the button, passing in the SVG text of the normal image and also the 
    * UiStyle object that we'll get our color information from.
    *
    * \param buttonName name of the button
    * \param normal     SVG text of the 'normal' button state (we need at *least* that)
    * \param style      
    */
   SvgButton(const String& buttonName, const String& normal, UiStyle* style);
   

   ~SvgButton();

   /**
    * Let this button know its context. To simplify styling, we use a convention-based
    * scheme to 
    * @param component Name of the component this button is a part of
    * @param button    'name' of this button as used in the palette name. Not necessarily
    *                   its name in source code 
    */
   void SetContext(const String& component, const String& button);


   /**
    * Add a new button-state image by passing in SVG text.
    * @param  imageIndex index, between kNormal and kButtonImageCount
    * @param  buttonText SVG Text
    * @return            true if imageIndex was Valid
    */
   bool AddButtonImage(int imageIndex, const String& buttonText);

   /**
    * Manually set a palette entry for a particular button state. Under normal 
    * circumstances, there should be no reason for you to manually call this unless
    * you create a button asset that has other colors than the 4 that we require,
    * @param imageIndex kNormal..kDisabledOn
    * @param svgKey    template key used inside the svg file (without the {   }) 
    * @param paletteKey name of the key in the palette that provides this color.
    */
   void SetPaletteEntry(int imageIndex, const String& svgKey, const String& paletteKey);

   void SetTemplateEntry(int imageIndex, const String& svgKey, const String& value);


   /**
    * When the style or palette change, we need to re-render each of the button image
    * drawables and insert them into the button that needs them.
    */
   void UpdateStyle();


private:
   /**
    * Utility fn to either return a new Drawable object or a nullptr if there's no image
    * data at the specified index.
    * @param  imageIndex index of the image, from kNormal..kButtonImageCount
    * @return            If we have image data, a pointer to Drawable, else nullptr.
    */
   Drawable* CreateDrawable(int imageIndex);


   /**
    * Contains the algorithm that maps information about a button into potential 
    * palette keys, and looks into the currently active palette to find the best 
    * match of actually defined keys. 
    * @param  state   Button state (as an integer)
    * @param  element Which element of the button are we looking for (border, fill, etc)  
    * @return         String name of the palette key for this element.
    */
   String FindPaletteKey(int state, const String& element) const;

private:
   UiStyle* fStyle;
   String   fComponentContext;
   String   fButtonContext;


   OwnedArray<SvgImage> fButtonImages;

};


#endif