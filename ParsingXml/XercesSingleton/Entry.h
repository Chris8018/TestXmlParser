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

class XmlNodeWrapper : public OtxDataType
{
public:
    virtual void NotifyDeepValueChanged() = 0;
};

class XmlElementWrapper;

class XmlDocumentWrapper : public XmlNodeWrapper
{
private:
    xercesc::DOMDocument *_internalDocument;

    std::shared_ptr<XmlElementWrapper> _rootElement;

    std::string _encoding;

    xercesc::DOMNode* ImportNode(xercesc::DOMElement *toBeImport, bool deep = true);

public:
    XmlDocumentWrapper(
        xercesc::DOMDocument *documentNode,
        std::shared_ptr<XmlElementWrapper> rootElement,
        const std::string &encoding
    );
    virtual ~XmlDocumentWrapper();

    xercesc::DOMDocument* GetDomDocument() const;

    std::string GetEncoding() const;

    std::shared_ptr<XmlElementWrapper> GetRootElement() const;

    // Must run before Get DOMDocument to update it
    void NotifyDeepValueChanged() override;

    std::string ToString() const override;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class XmlElementWrapper : public XmlNodeWrapper
{
    //friend XmlElementWrapper;
private:
    xercesc::DOMElement *_internalElement;

    std::shared_ptr<XmlNodeWrapper> _parent = nullptr;

    void ClearTextAndCData();
    void ClearAttributes();

public:
    XmlElementWrapper(xercesc::DOMElement *element);
    virtual ~XmlElementWrapper();

    xercesc::DOMElement* GetDomElement() const;

    bool IsRoot() const;

    std::shared_ptr<XmlNodeWrapper> GetParent() const;
    void SetParent(std::shared_ptr<XmlNodeWrapper> parentNode);

    bool HasParent() const;

    std::string GetTagName() const;

    void NotifyDeepValueChanged() override;

    std::string GetText() const;
    void SetText(xercesc::DOMText *textNode);

    XmlAttributes GetAttributes() const;
    void SetAttributes(XmlAttributes attributes);
    void SetAttribute(std::string name, std::string value);

    void InsertChildBefore(
        std::shared_ptr<XmlElementWrapper> child,
        std::shared_ptr<XmlElementWrapper> insertBefore
    );

    std::shared_ptr<XmlElementWrapper> CopyElement(bool deep = true) const;

    std::string ToString() const override;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class DOMPrintErrorHandler : public xercesc::DOMErrorHandler
{
public:
    DOMPrintErrorHandler();
    virtual ~DOMPrintErrorHandler();

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

    // Don't use Smart Pointer
    // This Writer need to be deleted before TERMINATE
    XercesXmlWriter *_xmlStringWriter;

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

    std::string NodeToString(const XmlElementWrapper &element);
    std::string NodeToString(const XmlDocumentWrapper &document);

    // @return Empty DOMDocument
    xercesc::DOMDocument* CreateEmptyDOMDocument();
    
    std::shared_ptr<XmlElementWrapper> CreateXmlElement(
        const XmlAttributes &attributes,
        const std::string &name,
        const std::string &text
    );

    std::shared_ptr<XmlDocumentWrapper> CreateXmlDocument(
        std::shared_ptr<XmlElementWrapper> root,
        const std::string &encoding, // TODO: Change this to enum later
        const std::string &version,
        const bool &standalone
    );

    void AddXmlChildElement(
        std::shared_ptr<XmlElementWrapper> child,
        std::shared_ptr<XmlElementWrapper> insertBefore,
        std::shared_ptr<XmlElementWrapper> parent
    );

    void SetXmlElementAttributes(
        const XmlAttributes &attributes,
        std::shared_ptr<XmlElementWrapper> element
    );

    void SetXmlElementAttribute(
        std::shared_ptr<XmlElementWrapper> element,
        const std::string &name,
        const std::string &value
    );

    void SetXmlElementText(
        std::shared_ptr<XmlElementWrapper> element,
        const std::string &text
    );

    std::shared_ptr<XmlElementWrapper> GetXmlRootElement(
        const std::shared_ptr<XmlDocumentWrapper> xmlDocument
    ) const;

    std::string GetXmlElementName(
        const std::shared_ptr<XmlElementWrapper> xmlElement
    ) const;

    std::shared_ptr<XmlElementWrapper> CopyXmlElement(
        const std::shared_ptr<XmlElementWrapper> original
    );

    std::list<std::shared_ptr<XmlElementWrapper>> GetXmlElementChildElements(
        const std::shared_ptr<XmlElementWrapper> element,
        const std::string &elementName
    ) const;
};

class XmlLib
{
private:
    //static std::string GetSupportXmlVersion(const std::string &encoding);
public:
    static std::shared_ptr<XmlDocumentWrapper> CreateXmlDocument(
        const std::string &encoding,
        std::shared_ptr<XmlElementWrapper> rootNode,
        const bool &standalone,
        const std::string &version
    );
    static std::shared_ptr<XmlElementWrapper> CreateXmlElement(
        const XmlAttributes &attributes,
        const std::string &name,
        const std::string &text
    );
    
    static void AddXmlChildElement(
        std::shared_ptr<XmlElementWrapper> child,
        std::shared_ptr<XmlElementWrapper> insertBefore,
        std::shared_ptr<XmlElementWrapper> parent
    );

    static void SetXmlElementAttributes(
        const XmlAttributes &attributes,
        std::shared_ptr<XmlElementWrapper> element
    );

    static void SetXmlElementAttribute(
        std::shared_ptr<XmlElementWrapper> element,
        const std::string &name,
        const std::string &value
    );
};

static void TestCase1();
static void TestCase2();

static void TestCase3();
#endif // !XercesSingleton_H
