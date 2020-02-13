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
    class DOMDocument;
    class DOMElement;
    class DOMNode;

    class DOMError;

    class DOMLSSerializer;
    class DOMConfiguration;

    class DOMLSOutput;

    //class XMLFormatTarget;
    //class DOMErrorHandler;
    // TODO: Add more?
}
namespace xercesc = xercesc_3_2;

class OtxDataType
{
    //
};

class XmlNode : public OtxDataType
{
    //
};

class XmlElement;

class XmlDocument : public OtxDataType
{
private:
    std::shared_ptr<XmlElement> _rootElement;

    std::string _encoding;
    std::string _version;
    std::string _standalone;

public:
    XmlDocument(std::shared_ptr<XmlElement> rootElement);
    XmlDocument(
        std::shared_ptr<XmlElement> rootElement,
        const std::string &encoding,
        const std::string &version,
        const std::string &standalone
    );
    virtual ~XmlDocument();

    std::shared_ptr<XmlElement> GetRootElement() const;

    std::string ToString() const;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class XmlElement : public OtxDataType
{
private:
    xercesc::DOMElement *_internalElement;

    bool _isRoot = false;
public:
    XmlElement(xercesc::DOMElement *element);
    virtual ~XmlElement();

    xercesc::DOMElement* GetDomElement() const;

    bool HasParent() const;

    void SetIsRoot(const bool &isRoot);
    bool IsRoot() const;

    //void ClearText();

    std::string ToString() const;

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
    //std::shared_ptr<xercesc::DOMLSSerializer> _writer;

    //std::shared_ptr<xercesc::DOMLSOutput> _outStream; // DON'T NEED

    //std::shared_ptr<xercesc::DOMConfiguration> _configuration; // DON'T NEED

    //std::shared_ptr<xercesc::DOMErrorHandler> _errorHandler;
    xercesc::DOMLSSerializer *_writer;

    std::shared_ptr<xercesc::DOMErrorHandler> _errorHandler;
    xercesc::DOMLSOutput *_outStream;
    

    //void Init();
    //void Clear();

    void SetErrorHandler();
    
    bool CanSetPrettyPrint();
    void SetPrettyPrintFormat();

    xercesc::DOMConfiguration* GetConfiguration() const;

public:
    XercesXmlWriter(xercesc::DOMImplementation *domImpl);
    virtual ~XercesXmlWriter();

    void SetEncoding();
    void SetEncoding(const std::string &encoding);

    std::string WriteToString(xercesc::DOMNode *domNode);
};

//class XStr
//{
//public:
//    static std::shared_ptr<XMLCh> StringToXmlCh(const std::string &str);
//    static std::string XmlChToString(const XMLCh* xmlCh);
//};

class StringToXmlCh
{
private:
    char16_t *_xmlCh;
public:
    StringToXmlCh(const std::string &str);
    virtual ~StringToXmlCh();

    char16_t* Get() const;
};

class XmlChToString
{
private:
    char *_strChars;
public:
    XmlChToString(const char16_t *xmlCh);
    virtual ~XmlChToString();

    std::string Get() const;
};

class XercesAdapter
{
private:
    XercesAdapter();

    // Don't use smart pointer OR delete this manually
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
    static bool IsNotValidEncoding(const std::string &encoding);
    // std::string stringEncoding // TODO: Implement + Change Name

    static bool IsValidXmlVersion(const std::string &version);

    void ClearXmlElementAttributes(std::shared_ptr<XmlElement> element);

public:
    XercesAdapter(XercesAdapter const &) = delete;

    XercesAdapter& operator=(XercesAdapter const &) = delete;

    virtual ~XercesAdapter();

    // Xerces Adapter: Singleton Instance
    // @return Xerces Adapter object
    static XercesAdapter& GetInstance();

    std::string NodeToString(const XmlElement &xmlElement);
    std::string NodeToString(std::shared_ptr<XmlDocument> xmlDocument);

    // @return Empty DOMDocument
    xercesc::DOMDocument* CreateEmptyDOMDocument();
    
    std::shared_ptr<XmlElement> CreateXmlElement(
        const std::map<std::string, std::string> &attributes,
        const std::string &name,
        const std::string &text
    );

    std::shared_ptr<XmlDocument> CreateXmlDocument(
        std::shared_ptr<XmlElement> &root,
        const std::string &encoding, // TODO: Change this to enum later
        const std::string &version,
        const bool &standalone
    );

    void AddXmlChildElement(
        std::shared_ptr<XmlElement> child,
        std::shared_ptr<XmlElement> insertBefore,
        std::shared_ptr<XmlElement> parent
    );

    void SetXmlElementAttributes(
        std::map<std::string, std::string> attributes,
        std::shared_ptr<XmlElement> element
    );

    void SetXmlElementAttribute(
        std::shared_ptr<XmlElement> element,
        const std::string &name,
        const std::string &value
    );
};

class XmlLib
{
private:
    static std::string GetSupportXmlVersion(const std::string &encoding);
public:
    static std::shared_ptr<XmlDocument> CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone);
    static std::shared_ptr<XmlElement> CreateXmlElement(const std::string &name, const std::string &text, const std::map<std::string, std::string> &attributes);
    
    static void AddXmlChildElement(std::shared_ptr<XmlElement> parent, std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> insertBefore);
};

static void TestCase1();
static void TestCase2();
#endif // !XercesSingleton_H
