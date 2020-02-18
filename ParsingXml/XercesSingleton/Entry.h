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
    //
};

class XmlNode : public OtxDataType
{
protected:
    std::shared_ptr<XmlNode> _parent = nullptr;
public:
    virtual std::shared_ptr<XmlNode> GetParent() const;
    virtual void SetParent(std::shared_ptr<XmlNode> parentNode);

    virtual bool HasParent() const;
};

//class A
//{
//    //
//};
//
//class IXmlNodesList
//{
//    //
//};

class XmlElement;

//class XmlCData : public XmlNode
//{
//private:
//    xercesc::DOMCDATASection *_internalCDataNode;
//
//public:
//    XmlCData(xercesc::DOMCDATASection *cdataNode);
//    virtual ~XmlCData();
//
//    xercesc::DOMCDATASection* GetDomCData() const;
//};
//
//class XmlText : public XmlNode
//{
//private:
//    xercesc::DOMText *_internalTextNode;
//
//public:
//    XmlText(xercesc::DOMText *textNode);
//    virtual ~XmlText();
//
//    xercesc::DOMText* GetDomText() const;
//};
//
//class XmlProcessingInstruction : public XmlNode
//{
//private:
//    xercesc::DOMProcessingInstruction *_internalPINode;
//
//public:
//    XmlProcessingInstruction(xercesc::DOMProcessingInstruction *piNode);
//    virtual ~XmlProcessingInstruction();
//
//    xercesc::DOMProcessingInstruction* GetDomPI() const;
//};
//
//class XmlComment : public XmlNode
//{
//private:
//    xercesc::DOMComment *_internalCommentNode;
//
//public:
//    XmlComment(xercesc::DOMComment *commentNode);
//    virtual ~XmlComment();
//
//    xercesc::DOMComment* GetDomComment() const;
//};

//class XmlOtherNode : public XmlNode
//{
//private:
//    xercesc::DOMNode *_internalNode;
//
//public:
//    XmlOtherNode(xercesc::DOMNode *node);
//    virtual ~XmlOtherNode();
//
//    xercesc::DOMNode* GetDomNode() const;
//};

class XmlDocument : public XmlNode
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

    std::shared_ptr<XmlNode> GetParent() const override;
    void SetParent(std::shared_ptr<XmlNode> parentNode) override;

    bool HasParent() const override;

    std::string ToString() const;

    //bool operator==(...) const;
    //bool IsEqual(...) const;
};

class XmlElement : public XmlNode
{
private:
    xercesc::DOMElement *_internalElement;

public:
    XmlElement(xercesc::DOMElement *element);
    virtual ~XmlElement();

    xercesc::DOMElement* GetDomElement() const;

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

    void SetEncoding();
    void SetEncoding(const std::string &encoding);

    std::string WriteToString(xercesc::DOMNode *domNode);
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
        const XmlAttributes &attributes,
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
        const XmlAttributes &attributes,
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
#endif // !XercesSingleton_H
