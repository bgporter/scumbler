// Copyright (c) 2014 Bg Porter

// !!! license

#ifndef h_SvgButton
#define h_SvgButton

#include "JuceHeader.h"

#include "SvgImage.h"

class SvgButton
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

   SvgButton(const String& normal, const String& hover=String::empty, const String& down=String::empty,
      const String& disabled=String::empty, const String& normalOn=String::empty, 
      const String& hoverOn=String::empty, const String& downOn=String::empty, 
      const String& disabledOn=String::empty);
   ~SvgButton();

   void SetPaletteEntry(int imageIndex, const String& svgKey, const String& paletteKey);

   void SetTemplateEntry(int imageIndex, const String& svgKey, const String& value);


   /**
    * When the style or palette change, we need to re-render each of the button image
    * drawables and insert them into the button that needs them.
    * Q: Why don't we pass this button pointer into this object a a member?
    * A: Consider the case where we have a UI that has multiple identical buttons -- 
    * decoupling here makes things easier in that case.
    * @param button Pointer to the DrawableButton we need to update.
    * @param style  pointer to the style object in use.
    */
   void SetButtonImages(DrawableButton* button, UiStyle* style);


private:
   /**
    * Utility fn to either return a new Drawable object or a nullptr if there's no image
    * data at the specified index.
    * @param  imageIndex index of the image, from kNormal..kButtonImageCount
    * @param  style      Pointer to the UiStyle object with palette data.
    * @return            If we have image data, a pointer to Drawable, else nullptr.
    */
   Drawable* CreateDrawable(int imageIndex, UiStyle* style);

private:
   OwnedArray<SvgImage> fButtonImages;

};


#endif