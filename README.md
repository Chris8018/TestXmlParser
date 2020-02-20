# Test XML Parser Library for C++

```cpp
static void AddXmlChildElement(XmlElement &parent, XmlElement &child, XmlElement? &insertBefore);


static std::shared_ptr<XmlElement> CopyXmlElement(XmlElement &element);


static std::shared_ptr<XmlDocument> CreateXmlDocument(XmlElement &rootNode, std::string &version, Encodings &encoding, bool &standalone);

static std::shared_ptr<XmlElement> CreateXmlElement(std::string &name, std::string &text, Map &attributes);


static void DeleteXmlAttribute(XmlElement &element, std::string &name);

static void DeleteXmlChildElement(XmlElement &parent, XmlElement &child);


static std::shared_ptr<Map> GetXmlElementAttributes(XmlElement &element);

static std::shared_ptr<List> GetXmlElementChildElements(XmlElement &element, std::string &elementName); // List<XmlElement>

static std::string GetXmlElementName(XmlElement &element);

static std::shared_ptr<List> GetXmlElementsByXPath(XmlDocument &document, std::string &xPath); // List<XmlElement>

static std::string GetXmlElementText(XmlElement &element);

static std::shared_ptr<XmlElement> GetXmlRootElement(XmlDocument &document);


static void SetXmlComment(XmlDocument &document, std::string &comment, bool &append); // append = false

static void SetXmlElementAttributes(XmlElement &element, Map &attributes);

static void SetXmlElementText(XmlElement &element, std::string &text);

static void SetXmlProcessingInstructions(XmlDocument &document, List &processingInstructions);

static void SetXmlElementAttribute(XmlElement &element, std::string &name, std::string &value);


static void ValidateXml(std::string &path, bool &isValid, std::string &errorMessages, Encodings &fallbackEncoding);


static std::shared_ptr<XmlDocument> XmlFromByteField(ByteField &byteField, Encodings &fallbackEncoding);

static std::shared_ptr<ByteField> XmlToByteField(XmlDocument &document);


static std::shared_ptr<XmlDocument> XmlLoadFromFile(std::string &path, Encodings &fallbackEncoding);

static void XmlSaveToFile(XmlDocument &document);
```

## Element Text Escaping SYMBOL
XML Text Escape Case

~!@#$%^&*    ()_+{}|:\"  <   > ?`1234567890-=[]\\;',./ - In

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[]\\;',./ - Out - Java

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[]\\;',./ - Out - C#

~!@#$%^&amp;*()_+{}|: "&lt;&gt;?`1234567890-=[] \;',./ - Out - C++ (Xerces)
                                               

I will let the node name handle by Xerces itself
As for Text, Attribute Key and Value, I might need to handle them myself

-> Can do that either by take Exception or Try to use name checking within the chosen library.
