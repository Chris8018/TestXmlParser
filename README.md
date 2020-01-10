# Test XML Parser Library for C++

	static void AddXmlChildElement(XmlElement &parent, XmlElement &child, XmlElement &insertBefore);

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
	static std::shared_ptr<XmlDocument> XmlLoadFromFile(std::string &path, Encodings &fallbackEncoding);
	static void XmlSaveToFile(XmlDocument &document);
	static std::shared_ptr<ByteField> XmlToByteField(XmlDocument &document);