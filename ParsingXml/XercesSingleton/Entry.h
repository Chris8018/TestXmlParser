#ifndef XercesSingleton_H
#define XercesSingleton_H

#include <xercesc/dom/DOMErrorHandler.hpp>

#include <memory>

#include <string>

#include <vector>
#include <map>
#include <list>

//#include "xercesc/dom/DOM.hpp"

namespace xercesc_3_2
{
    class DOMImplementation;

    class DOMNode;

    class DOMDocument;
    class DOMElement;

    class DOMProcessingInstruction;
    class DOMComment;

    class DOMText;
    class DOMCDATASection;

    class DOMError;

    class DOMLSSerializer;
    class DOMConfiguration;

    class DOMLSOutput;

    //class XMLFormatTarget;
    //class DOMErrorHandler;
    // TODO: Add more?
}
namespace xercesc = xercesc_3_2;

typedef std::map<std::string, std::string> XmlAttributes;

typedef char16_t XMLCh;


class OtxDataType
{
public:
    virtual std::string ToString() const { return ""; }
};

class XmlNode : public OtxDataType
{
public:
    virtual void NotifyDeepValueChanged() = 0;
};

class XmlElement;

class XmlDocument : public XmlNode
{
private:
    xercesc::DOMDocument *_internalDocument;

    std::shared_ptr<XmlElement> _rootElement;

    std::string _encoding;

    xercesc::DOMNode* ImportNode(xercesc::DOMElement *toBeImport, bool deep = true);

public:
    XmlDocument(
        xercesc::DOMDocument *documentNode,
        std::shared_ptr<XmlElement> rootElement,
        const std::string &encoding = "UTF-8"
    );
    virtual ~XmlDocument();

    xercesc::DOMDocument* GetDomDocument() const;

    std::string GetEncoding() const;

    std::shared_ptr<XmlElement> GetRootElement() const;

    // Must run before Get DOMDocument to update it
    void NotifyDeepValueChanged() override;

    std::string ToString() const override;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class XmlElement : public XmlNode
{
private:
    xercesc::DOMElement *_internalElement;

    std::shared_ptr<XmlNode> _parent = nullptr;

    void ClearTextAndCData();
    void ClearAttributes();

public:
    XmlElement(xercesc::DOMElement *element);
    virtual ~XmlElement();

    xercesc::DOMElement* GetDomElement() const;

    bool IsRoot() const;

    std::shared_ptr<XmlNode> GetParent() const;
    void SetParent(std::shared_ptr<XmlNode> parentNode);

    bool HasParent() const;

    std::string GetTagName() const;

    void NotifyDeepValueChanged() override;

    std::string GetText() const;
    void SetText(xercesc::DOMText *textNode);

    XmlAttributes GetAttributes() const;
    void SetAttributes(XmlAttributes attributes);
    void SetAttribute(std::string name, std::string value);

    void InsertChildBefore(
        std::shared_ptr<XmlElement> child,
        std::shared_ptr<XmlElement> insertBefore
    );

    std::shared_ptr<XmlElement> CopyElement(bool deep = true) const;

    std::string ToString() const override;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class DomWriterErrorHandler : public xercesc::DOMErrorHandler
{
public:
    DomWriterErrorHandler();
    virtual ~DomWriterErrorHandler();

    /** @name The error handler interface */
    bool handleError(const xercesc::DOMError &domError) override;
    void resetErrors();

};

class XercesXmlWriter
{
private:
    xercesc::DOMLSSerializer *_writer;

    std::shared_ptr<xercesc::DOMErrorHandler> _errorHandler;
    xercesc::DOMLSOutput *_outStream;

    void SetErrorHandler();
    
    bool CanSetPrettyPrint();
    void SetPrettyPrintFormat();

    xercesc::DOMConfiguration* GetConfiguration() const;

public:
    XercesXmlWriter(xercesc::DOMImplementation *domImpl);
    virtual ~XercesXmlWriter();

    std::string WriteToString(xercesc::DOMNode *domNode);
    std::string WriteToString(
        xercesc::DOMNode *domNode,
        std::string encoding
    );
};

class StringToXmlCh
{
private:
    XMLCh *_xmlCh;
public:
    StringToXmlCh(const std::string &str);
    virtual ~StringToXmlCh();

    XMLCh* Get() const;
};

class XmlChToString
{
private:
    char *_strChars;
public:
    XmlChToString(const XMLCh *xmlCh);
    virtual ~XmlChToString();

    std::string Get() const;
};

class XercesAdapter
{
private:
    XercesAdapter();

    // Don't use Smart Pointer OR deleting _domImpl manually
    // It will be deleted by XercesC itself
    xercesc::DOMImplementation *_domImpl;

    // Don't use Smart Pointer
    // This Writer need to be deleted before TERMINATE
    xercesc::DOMDocument *_dummyDoc;

    static bool IsEmptyOrWhiteSpaceString(const std::string &str);
    static bool IsWhiteSpaceString(const std::string &str);

    // TODO: Change encoding to Enum
    static bool IsValidEncoding(const std::string &encoding);
    // std::string stringEncoding // TODO: Implement + Change Name

    static bool IsValidXmlVersion(const std::string &version);

public:
    XercesAdapter(XercesAdapter const &) = delete;

    XercesAdapter& operator=(XercesAdapter const &) = delete;

    virtual ~XercesAdapter();

    // Xerces Adapter: Singleton Instance
    // @return Xerces Adapter object
    static XercesAdapter& GetInstance();

    std::string NodeToString(const XmlElement &element);
    std::string NodeToString(const XmlDocument &document);

    // @return Empty DOMDocument
    xercesc::DOMDocument* CreateEmptyDOMDocument();

    std::shared_ptr<XmlDocument> CreateXmlDocument(
        std::shared_ptr<XmlElement> root,
        const std::string &encoding = "UTF-8", // TODO: Change this to enum later
        const std::string &version = "1.0",
        const bool &standalone = false
    );

    std::shared_ptr<XmlElement> GetXmlRootElement(
        const std::shared_ptr<XmlDocument> xmlDocument
    ) const;

    void SetXmlComment(
        std::shared_ptr<XmlDocument> document,
        const std::string &comment,
        const bool &append = false
    );

    void SetXmlProcessingInstructions(
        std::shared_ptr<XmlDocument> document,
        const std::list<std::string> &processingInstructions
    );
    
    std::shared_ptr<XmlElement> CreateXmlElement(
        const std::string &name,
        const std::string &text,
        const XmlAttributes &attributes
    );

    void AddXmlChildElement(
        std::shared_ptr<XmlElement> parent,
        std::shared_ptr<XmlElement> child,
        std::shared_ptr<XmlElement> insertBefore = nullptr
    );

    void DeleteXmlChildElement(
        std::shared_ptr<XmlElement> parent,
        std::shared_ptr<XmlElement> child
    );

    std::list<std::shared_ptr<XmlElement>> GetXmlElementChildElements(
        const std::shared_ptr<XmlElement> element,
        const std::string &elementName = ""
    ) const;

    std::list<std::shared_ptr<XmlElement>> GetXmlElementsByXPath(
        const std::shared_ptr<XmlDocument> document,
        const std::string &xPath
    ) const;

    void SetXmlElementAttributes(
        std::shared_ptr<XmlElement> element,
        const XmlAttributes &attributes
    );

    void SetXmlElementAttribute(
        std::shared_ptr<XmlElement> element,
        const std::string &name,
        const std::string &value
    );

    void DeleteXmlAttribute(
        std::shared_ptr<XmlElement> element,
        const std::string &name
    );

    XmlAttributes GetXmlElementAttributes(
        const std::shared_ptr<XmlElement> element
    ) const;

    void SetXmlElementText(
        std::shared_ptr<XmlElement> element,
        const std::string &text
    );

    std::string GetXmlElementText(
        const std::shared_ptr<XmlElement> element
    ) const;

    std::string GetXmlElementName(
        const std::shared_ptr<XmlElement> xmlElement
    ) const;

    std::shared_ptr<XmlElement> CopyXmlElement(
        const std::shared_ptr<XmlElement> original
    );

    void XmlSaveToFile(
        const std::shared_ptr<XmlDocument> document
    ) const;

    std::shared_ptr<XmlDocument> XmlLoadFromFile(
        const std::string &path,
        const std::string &fallbackEncoding = "UTF-8"
    );

    std::string XmlToByteField(
        const std::shared_ptr<XmlDocument> document
    ) const;

    std::shared_ptr<XmlDocument> XmlFromByteField(
        const std::string &byteField,
        const std::string &falbackEncoding = "UTF-8"
    );

    void ValidateXml(
        const std::string &path,
        bool &isValid, // Variable<Boolean> // Not Optional
        const std::string &fallbackEncoding = "UTF-8",
        std::shared_ptr<std::string> errorMessages = nullptr //Variable<String>
    );
};

class XmlLib
{
private:
    //static std::string GetSupportXmlVersion(const std::string &encoding);
public:
    static std::shared_ptr<XmlDocument> CreateXmlDocument(
        const std::string &encoding,
        std::shared_ptr<XmlElement> rootNode,
        const bool &standalone,
        const std::string &version
    );
    static std::shared_ptr<XmlElement> CreateXmlElement(
        const XmlAttributes &attributes,
        const std::string &name,
        const std::string &text
    );
    
    static void AddXmlChildElement(
        std::shared_ptr<XmlElement> child,
        std::shared_ptr<XmlElement> insertBefore,
        std::shared_ptr<XmlElement> parent
    );

    static void SetXmlElementAttributes(
        const XmlAttributes &attributes,
        std::shared_ptr<XmlElement> element
    );

    static void SetXmlElementAttribute(
        std::shared_ptr<XmlElement> element,
        const std::string &name,
        const std::string &value
    );
};

static void TestCase1();
static void TestCase2();

static void TestCase3();
#endif // !XercesSingleton_H
