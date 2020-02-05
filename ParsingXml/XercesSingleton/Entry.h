#ifndef XercesSingleton_H
#define XercesSingleton_H

#include <xercesc/dom/DOMErrorHandler.hpp>

#include <memory>

#include <string>

#include <vector>
#include <map>

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

    class XMLFormatTarget;
    //class DOMErrorHandler;
    // TODO: Add more?
}
namespace xercesc = xercesc_3_2;

class OtxDataType
{
    //
};

class XmlElement;

class XmlDocument : public OtxDataType
{
private:
    std::shared_ptr<xercesc::DOMDocument> _xmlDocument;
    std::string _encoding;
public:
    XmlDocument(
        std::shared_ptr<XmlElement> rootElement,
        const std::string &version,
        const std::string &encoding,
        const bool &standalone);
    //XmlDocument();
    virtual ~XmlDocument();

    //
};

class XmlElement : public OtxDataType
{
    friend XmlElement;
    friend XmlDocument;
private:
    std::shared_ptr<xercesc::DOMDocument> _ownerDocument;

    //std::shared_ptr<xercesc_3_2::DOMNode> _parentNode;
    std::shared_ptr<xercesc::DOMElement> _xmlElement;

    bool _assigned = false;

    //void SetDomNodeParent(std::shared_ptr<xercesc_3_2::DOMNode> parent);

    void RemoveOwnerDocument();
    void SetXmlElement(
        std::shared_ptr<xercesc::DOMElement> otherElement);

public:
    XmlElement(std::string name);
    //XmlElement(std::)
    virtual ~XmlElement();

    bool IsAssigned() const;

    void SetText(const std::string &text);
    void SetAttribute(const std::string &name, const std::string &value);
    void SetAttributes(const std::map<std::string, std::string> &attributes);

    void AddChild(std::shared_ptr<XmlElement> child);
    void InsertChildBefore(
        std::shared_ptr<XmlElement> child,
        std::shared_ptr<XmlElement> element);

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
    bool HandleError(const xercesc::DOMError &domError);
    void ResetErrors();

};

class XercesXmlWriter
{
private:
    std::shared_ptr<xercesc::DOMLSSerializer> _writer;

    std::shared_ptr<xercesc::DOMLSOutput> _outStream;

    std::shared_ptr<xercesc::DOMConfiguration> _configuration;

    std::shared_ptr<xercesc::DOMErrorHandler> _errorHandler;

    void Init();
    void Clear();

    void SetErrorHandler();
    
    bool CanSetPrettyPrint();
    void SetPrettyPrintFormat();

public:
    XercesXmlWriter(std::shared_ptr<xercesc::DOMImplementation> domImpl);
    virtual ~XercesXmlWriter();

    void SetEncoding(const std::string &encoding = "UTF-8");

    std::string Write(std::shared_ptr<xercesc::DOMNode> domNode);
};

class XercesAdapter
{
    //friend XmlDocument;
    //friend XmlElement;
private:
    XercesAdapter();

    void InitializeDomImplementation();

    void Clear();

    std::shared_ptr<xercesc::DOMImplementation> _domImpl;
public:
    XercesAdapter(XercesAdapter const &) = delete;
    XercesAdapter& operator=(XercesAdapter const &) = delete;

    virtual ~XercesAdapter();

    static XercesAdapter& GetInstance();

    std::shared_ptr<xercesc::DOMDocument> CreateEmptyDocument() const;

    std::shared_ptr<xercesc::DOMElement> CreateElementFromDocument(std::shared_ptr<xercesc::DOMDocument> xmlDoc, const std::string &name);

    std::shared_ptr<xercesc::DOMElement> CopyElementToDocumentOfElement(
        std::shared_ptr<xercesc::DOMElement> element,
        std::shared_ptr<xercesc::DOMElement> targetElement,
        bool deep = false);

    std::shared_ptr<XMLCh> StringToXmlCh(const std::string &str);
    std::string XmlChToString(const std::shared_ptr<XMLCh> xmlCh);
};

class XmlLib
{
private:
    static std::string CorrectingXmlVersion(const std::string &encoding);
public:
    static std::shared_ptr<XmlDocument> CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone);
    static std::shared_ptr<XmlElement> CreateXmlElement(const std::string &name, const std::string &text, const std::map<std::string, std::string> &attributes);
    
    static void AddXmlChildElement(std::shared_ptr<XmlElement> parent, std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> insertBefore);
};

#endif // !XercesSingleton_H
