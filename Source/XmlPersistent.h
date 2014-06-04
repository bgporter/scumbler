// Copyright (c) 2014 Bg Porter.

// !!! License

#ifndef h_XmlPersistent
#define h_XmlPersistent

#include "../JuceLibraryCode/JuceHeader.h"

/**
 * @class XmlPersistent
 * @brief Abstract base class to be used as a mix-in for the classes that need
 *        to be able to store/restore themselves in an XML file.
 */
class XmlPersistent
{
public:

   virtual ~XmlPersistent() {};

   /**
    * Load this object from the provided XmlElement object. If this object owns 
    * objects of classes that are also XmlPersistent, call those recursively.
    * @param e      XmlElement object with our data to restore .
    * @param errors If we encounter errors, we add strings describing those errors
    *               to this array. 
    */
   virtual void LoadXml(XmlElement* e, StringArray& errors, int formatVersion=0) = 0;

   /**
    * Create a new XmlElement object and fill it with our contents (and recursively
    * our children if appropriate)
    * @return The XmlElement to write to disk.
    */
   virtual XmlElement* DumpXml(int formatVersion=0) const = 0;

};


#endif // this MUST be the last line in this file.
