#ifndef XercesSingleton_H
#define XercesSingleton_H

#include <xercesc/dom/DOMErrorHandler.hpp>

#include <memory>

#include <string>

#include <vector>
#include <map>

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

void TestCase1();

class OtxDataType
{
    //
};

class XmlElement;

class XmlDocument : public OtxDataType
{
private:
    xercesc::DOMDocument *_xmlDocument;
    std::string _encoding;

    bool IsNotValidEncoding(const std::string &encoding) const;
public:
    XmlDocument(
        std::shared_ptr<XmlElement> rootElement,
        const std::string &version,
        const std::string &encoding,
        const bool &standalone);
    //XmlDocument();
    virtual ~XmlDocument();

    std::string ToString() const;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class XmlElement : public OtxDataType
{
    friend XmlElement;
    friend XmlDocument;
private:
    xercesc::DOMDocument *_ownerDocument;

    xercesc::DOMElement *_xmlElement;

    bool _assigned = false;

    void RemoveOwnerDocument();
    //void SetXmlElement(
    //    std::shared_ptr<xercesc::DOMElement> otherElement);

public:
    XmlElement(std::string name);
    //XmlElement(std::)
    virtual ~XmlElement();

    bool IsAssigned() const;

    //void SetText(const std::string &text);
    //void SetAttribute(const std::string &name, const std::string &value);
    //void SetAttributes(const std::map<std::string, std::string> &attributes);

    //void AddChild(std::shared_ptr<XmlElement> child);
    //void InsertChildBefore(
    //    std::shared_ptr<XmlElement> child,
    //    std::shared_ptr<XmlElement> element);

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

    //void InitializeDomImplementation();

    //void Clear();

    //std::shared_ptr<xercesc::DOMImplementation> _domImpl;
    //std::shared_ptr<XercesXmlWriter> _xmlWriter;

    /// Don't use smart pointer OR delete this manually
    /// It will be deleted by XercesC itself
    xercesc::DOMImplementation *_domImpl;

    std::shared_ptr<XercesXmlWriter> _xmlWriter;

    //xercesc::DOMImplementation *_domImpl;
public:
    XercesAdapter(XercesAdapter const &) = delete;
    XercesAdapter& operator=(XercesAdapter const &) = delete;

    virtual ~XercesAdapter();

    static XercesAdapter& GetInstance();

    xercesc::DOMDocument* CreateEmptyDocument();

    //std::shared_ptr<xercesc::DOMElement> CreateElementFromDocument(std::shared_ptr<xercesc::DOMDocument> xmlDoc, const std::string &name);

    //std::shared_ptr<xercesc::DOMElement> MoveElementToDifferentOwnerDocument(
    //    std::shared_ptr<xercesc::DOMElement> element,
    //    std::shared_ptr<xercesc::DOMElement> targetElement,
    //    bool deep = false);

    //std::string NodeToString(std::shared_ptr<xercesc::DOMNode> domNode, const std::string &encoding = "UTF-8");
};

class XmlLib
{
private:
    static std::string GetSupportXmlVersion(const std::string &encoding);
public:
    //static std::shared_ptr<XmlDocument> CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone);
    //static std::shared_ptr<XmlElement> CreateXmlElement(const std::string &name, const std::string &text, const std::map<std::string, std::string> &attributes);
    //
    //static void AddXmlChildElement(std::shared_ptr<XmlElement> parent, std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> insertBefore);
};

#endif // !XercesSingleton_H
