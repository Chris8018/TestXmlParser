#ifndef TestPugiXml_H
#define TestPugiXml_H

#include <pugixml.hpp>

class XmlDocument
{
public:
	XmlDocument();
private:
	pugi::xml_document _internalDocument;
};
#endif
