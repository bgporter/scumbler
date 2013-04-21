// Copyright (c) 2013 Bg Porter.

// !!! License

#ifndef h_PluginSlotComponent
#define h_PluginSlotComponent

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginBlock.h"
#include "PluginEditorWindow.h"

class PluginSlotComponent  : public Component
                           , public SettableTooltipClient
{
public:
   enum 
   {
      kNoPreferredWidth = 0   
   };


   PluginSlotComponent(PluginBlock* block, int index);
   ~PluginSlotComponent();

   bool IsEmpty() const;

   /**
     * @name PluginEditorWindow operations.
     */
   ///@{   
   
   /**
    * Remember where the editor window is located so we can restore things 
    * from disk correctly.
    * @param x x position of the upper left
    * @param y y position of the upper left.
    */
   void SetEditorPosition(int x, int y);

   /**
    * Find out where the editor window should go -- used inside the ctor of
    * the pluginEditorWindow.
    */
   Point<int> GetEditorPosition() const;

   /**
    * Display or hide the editor window. If the editor is already displayed but 
    * minimized, it will be restored to its last size and location.
    * @param display True to display/restore, False to delete.
    */
   void ShowEditor(bool display = true);

   /**
    * Ask this slot how many pixels wide it would ideally like to be, 
    * based on the length of the length of the loaded plugin string. If this
    * slot is empty, will return kNoPreferredWidth
    * @return Desired number of pixels wide this slot should be.
    */
   int GetPreferredWidth() const;


   ///@}
   
   ///@{ @name JUCE Component virtual overrides.
   void paint(Graphics& g);
   void resized();   
   void mouseDown(const MouseEvent& e);
   void mouseEnter(const MouseEvent& e);
   void mouseExit(const MouseEvent& e);
   void mouseDoubleClick(const MouseEvent& e);

   ///@}

private:
   PluginBlock*   fPluginBlock;
   int            fIndex;
   bool           fMouseOver;
   String         fPluginName;
   Point<int>     fPluginEditorLocation;
   ScopedPointer<PluginEditorWindow> fEditor;
};


#endif // this must be the last line in this file.
