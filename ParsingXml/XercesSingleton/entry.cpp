#include "Entry.h"

#include "xercesc/util/PlatformUtils.hpp"

#include <xercesc/util/XMLString.hpp>

//#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMError.hpp>

#include <xercesc/dom/DOMLSSerializer.hpp>

#include <xercesc/dom/DOM.hpp>

//#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

#include <xercesc/dom/StDOMNode.hpp>

#include <xercesc/util/TransService.hpp>

using xercesc::XMLPlatformUtils;

using xercesc::DOMImplementation;
using xercesc::DOMImplementationRegistry;

using xercesc::DOMLSSerializer;

using xercesc::DOMConfiguration;
using xercesc::DOMLSOutput;

using xercesc::DOMNode;

using xercesc::DOMDocument;
using xercesc::DOMElement;

using xercesc::DOMProcessingInstruction;
using xercesc::DOMComment;

using xercesc::DOMText;
using xercesc::DOMCDATASection;
using xercesc::DOMAttr;

typedef xercesc::DOMNode::NodeType Xerces_NodeType;

using xercesc::DOMException;

using xercesc::DOMError;

using xercesc::XMLString;
using xercesc::XMLUni;

using xercesc::XMLFormatTarget;

//using xercesc::StdOutFormatTarget;
using xercesc::MemBufFormatTarget;
using xercesc::LocalFileFormatTarget;

using xercesc::StDOMNode;

using xercesc::TranscodeFromStr;

#define PRETTY_PRINT XMLUni::fgDOMWRTFormatPrettyPrint
#define ERROR_HANDLER XMLUni::fgDOMErrorHandler

#define DEFAULT_XML_VERSION "1.0"
#define DEFAULT_ENCODING "UTF-8"

#include <iostream>

#include <sstream>

// XmlNode----------------------------------------------------------

std::shared_ptr<XmlNode> XmlNode::GetParent() const
{
    return _parent;
}

void XmlNode::SetParent(std::shared_ptr<XmlNode> parentNode)
{
    _parent = parentNode;
}

bool XmlNode::HasParent() const
{
    return _parent != nullptr;
}

//------------------------------------------------------------------

// XmlCData---------------------------------------------------------

//XmlCData::XmlCData(DOMCDATASection *cdataNode)
//{
//    _internalCDataNode = cdataNode;
//}
//
//XmlCData::~XmlCData()
//{
//    if (!HasParent())
//        _internalCDataNode->release();
//}
//
//DOMCDATASection* XmlCData::GetDomCData() const
//{
//    return _internalCDataNode;
//}

//------------------------------------------------------------------

// XmlText----------------------------------------------------------

//XmlText::XmlText(DOMText *textNode)
//{
//    _internalTextNode = textNode;
//}
//
//XmlText::~XmlText()
//{
//    if (!HasParent())
//        _internalTextNode->release();
//}
//
//DOMText* XmlText::GetDomText() const
//{
//    return _internalTextNode;
//}

//------------------------------------------------------------------

// XmlProcessingInstruction-----------------------------------------

//XmlProcessingInstruction::XmlProcessingInstruction(DOMProcessingInstruction *piNode)
//{
//    _internalPINode = piNode;
//}
//
//XmlProcessingInstruction::~XmlProcessingInstruction()
//{
//    if (!HasParent())
//        _internalPINode->release();
//}
//
//DOMProcessingInstruction* XmlProcessingInstruction::GetDomPI() const
//{
//    return _internalPINode;
//}

//------------------------------------------------------------------

// XmlComment-------------------------------------------------------

//XmlComment::XmlComment(DOMComment *commentNode)
//{
//    _internalCommentNode = commentNode;
//}
//
//XmlComment::~XmlComment()
//{
//    if (!HasParent())
//        _internalCommentNode->release();
//}
//
//DOMComment* XmlComment::GetDomComment() const
//{
//    return _internalCommentNode;
//}

//------------------------------------------------------------------

// XmlOtherNode-----------------------------------------------------

//XmlOtherNode::XmlOtherNode(DOMNode *node)
//{
//    _internalNode = node;
//}
//
//XmlOtherNode::~XmlOtherNode()
//{
//    if (~HasParent())
//        _internalNode->release();
//}
//
//DOMNode* XmlOtherNode::GetDomNode() const
//{
//    return _internalNode;
//}

//------------------------------------------------------------------

// XmlDocument------------------------------------------------------
XmlDocument::XmlDocument(std::shared_ptr<XmlElement> rootElement)
    : XmlDocument(rootElement, "UTF-8", "1.0", "no") {};

XmlDocument::XmlDocument(
    std::shared_ptr<XmlElement> rootElement,
    const std::string &encoding,
    const std::string &version,
    const std::string &standalone
)
{
    if (_rootElement->HasParent())
    {
        // TODO: Throw if Element HasParent
    }

    _rootElement = rootElement;

    _encoding = encoding;
    _version = version;
    _standalone = standalone;
}

XmlDocument::~XmlDocument()
{
    // TODO: MIGHT Handle FUTURE List of P.I. and Comments
}

std::shared_ptr<XmlElement> XmlDocument::GetRootElement() const
{
    return _rootElement;
}

std::string XmlDocument::ToString() const
{
    return "";
}

std::shared_ptr<XmlNode> XmlDocument::GetParent() const
{
    return nullptr;
}

void XmlDocument::SetParent(std::shared_ptr<XmlNode> parentNode)
{
    //
}

bool XmlDocument::HasParent() const
{
    return false;
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(DOMElement *element)
{
    _internalElement = element;
}

XmlElement::~XmlElement()
{
    if (!HasParent())
        _internalElement->release();
}

DOMElement* XmlElement::GetDomElement() const
{
    return _internalElement;
}

bool XmlElement::IsRoot() const
{
    auto document =
        std::dynamic_pointer_cast<XmlDocument>(_parent);

    return document != nullptr;
}

std::string XmlElement::ToString() const
{
    return XercesAdapter::GetInstance()
        .NodeToString(*this);
}

//------------------------------------------------------------------

// XmlLib-----------------------------------------------------------
std::shared_ptr<XmlDocument> XmlLib::CreateXmlDocument(
    const std::string &encoding,
    std::shared_ptr<XmlElement> rootNode,
    const bool &standalone,
    const std::string &version
)
{
    /*std::string _version = CorrectingXmlVersion(version);
    return std::make_shared<XmlDocument>(rootNode, _version, encoding, standalone);*/
    return nullptr;
}

std::shared_ptr<XmlElement> XmlLib::CreateXmlElement(
    const XmlAttributes &attributes,
    const std::string &name,
    const std::string &text
)
{
    return nullptr;
}

void XmlLib::AddXmlChildElement(
    std::shared_ptr<XmlElement> child,
    std::shared_ptr<XmlElement> insertBefore,
    std::shared_ptr<XmlElement> parent
)
{
    //
}

void XmlLib::SetXmlElementAttributes(
    const XmlAttributes &attributes,
    std::shared_ptr<XmlElement> element
)
{
    //
}

void XmlLib::SetXmlElementAttribute(
    std::shared_ptr<XmlElement> element,
    const std::string &name,
    const std::string &value
)
{
    //
}

//------------------------------------------------------------------

// XercesAdapter----------------------------------------------------

DOMPrintErrorHandler::DOMPrintErrorHandler() {}

DOMPrintErrorHandler::~DOMPrintErrorHandler() {}

void DOMPrintErrorHandler::resetErrors() {}

bool DOMPrintErrorHandler::handleError(const DOMError &domError)
{
    // TODO: Modify this to fit with RT7
    // Display whatever error message passed from the serializer
    if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        std::cerr << "\nWarning Message: ";
    else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        std::cerr << "\nError Message: ";
    else
        std::cerr << "\nFatal Message: ";

    std::string msg = XmlChToString(domError.getMessage()).Get();

    std::cerr << msg << std::endl;

    // Instructs the serializer to continue serialization if possible.
    return true;
}

XercesXmlWriter::XercesXmlWriter(DOMImplementation *domImpl)
{
    _writer = domImpl->createLSSerializer();
    _errorHandler = std::make_shared<DOMPrintErrorHandler>();
    _outStream = domImpl->createLSOutput();

    SetPrettyPrintFormat();
    SetEncoding();
}

XercesXmlWriter::~XercesXmlWriter()
{
    _outStream->release();

    _writer->release();
}

DOMConfiguration* XercesXmlWriter::GetConfiguration() const
{
    return _writer->getDomConfig();
}

void XercesXmlWriter::SetErrorHandler()
{
    GetConfiguration()->setParameter(
        ERROR_HANDLER,
        _errorHandler.get()
    );
}

void XercesXmlWriter::SetEncoding()
{
    SetEncoding(DEFAULT_ENCODING);
}

void XercesXmlWriter::SetEncoding(const std::string &encoding)
{
    _outStream->setEncoding(StringToXmlCh(encoding).Get());
}

bool XercesXmlWriter::CanSetPrettyPrint()
{
    return GetConfiguration()->canSetParameter(
        PRETTY_PRINT,
        true
    );
}

void XercesXmlWriter::SetPrettyPrintFormat()
{
    if (CanSetPrettyPrint())
        GetConfiguration()->setParameter(
            PRETTY_PRINT,
            true);
}

std::string XercesXmlWriter::WriteToString(DOMNode *domNode)
{
    std::shared_ptr<MemBufFormatTarget> formatTarget =
        std::make_shared<MemBufFormatTarget>();

    _outStream->setByteStream(formatTarget.get());

    _writer->write(domNode, _outStream);

    std::string str = 
        XmlChToString(
            TranscodeFromStr(
                formatTarget->getRawBuffer(),
                formatTarget->getLen(),
                DEFAULT_ENCODING
            ).adopt()
        ).Get();

    return str;
}

XercesAdapter::XercesAdapter()
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (std::exception & e)
    {
        // TODO: Print error message
    }

    // TODO: Create Parser, Implementation, etc
    //InitializeDomImplementation();

    _domImpl = DOMImplementationRegistry::getDOMImplementation(u"");

    _dummyDoc = _domImpl->createDocument();

    _xmlStringWriter = new XercesXmlWriter(_domImpl);
}

XercesAdapter::~XercesAdapter()
{
    delete _xmlStringWriter;

    _dummyDoc->release();

    // TODO: considering catching error
    XMLPlatformUtils::Terminate();
}

XercesAdapter &XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::string XercesAdapter::NodeToString(const XmlElement &xmlElement)
{
    return _xmlStringWriter
        ->WriteToString(xmlElement.GetDomElement());
}

std::string XercesAdapter::NodeToString(std::shared_ptr<XmlDocument> xmlDocument)
{
    return "";
}

DOMDocument* XercesAdapter::CreateEmptyDOMDocument()
{
    return _domImpl->createDocument(0, 0, 0);
}

std::shared_ptr<XmlElement> XercesAdapter::CreateXmlElement(
    const XmlAttributes &attributes,
    const std::string &name,
    const std::string &text
)
{
    DOMElement *element = nullptr;

    try
    {
        element =
            _dummyDoc->createElement(StringToXmlCh(name).Get());
    }
    catch (const DOMException &e)
    {
        // TODO: Change Error Message
        std::string errorMessage =
            XmlChToString(e.getMessage()).Get();

        std::cout << errorMessage << std::endl;

        throw new std::exception(errorMessage.c_str());
    }
    catch (...)
    {
        // TODO: unexpected error
        throw new std::exception("Unexpected Error");
    }

    std::shared_ptr<XmlElement> xmlElement =
        std::make_shared<XmlElement>(element);

    // TODO: Check Empty String & Switch to SetText Funciton
    element->setTextContent(StringToXmlCh(text).Get());

    // TODO: Set XmlAttributes
    //

    return xmlElement;
}

bool XercesAdapter::IsEmptyOrWhiteSpaceString(const std::string &str)
{
    if (str.empty())
        return true;

    return IsWhiteSpaceString(str);
}

bool XercesAdapter::IsWhiteSpaceString(const std::string &str)
{
    // TODO: Implement
    return false;
}

void XercesAdapter::SetXmlElementAttributes(
    const XmlAttributes &attributes,
    std::shared_ptr<XmlElement> element
)
{
    ClearXmlElementAttributes(element);

    if (attributes.size() == 0)
        return;

    for (auto it = attributes.begin(); it != attributes.end(); it++)
    {
        SetXmlElementAttribute(
            element,
            it->first,
            it->second
        );
    }
}

void XercesAdapter::ClearXmlElementAttributes(std::shared_ptr<XmlElement> element)
{
    //
}

void XercesAdapter::SetXmlElementAttribute(
    std::shared_ptr<XmlElement> element,
    const std::string &name,
    const std::string &value
)
{
    auto domElement = element->GetDomElement();

    try
    {
        domElement->setAttribute(
            StringToXmlCh(name).Get(),
            StringToXmlCh(value).Get()
        );
    }
    catch (const DOMException & e)
    {
        // TODO: Change Error Message
        std::string errorMessage =
            XmlChToString(e.getMessage()).Get();

        std::cout << errorMessage << std::endl;

        throw new std::exception(errorMessage.c_str());
    }
}

std::shared_ptr<XmlDocument> XercesAdapter::CreateXmlDocument(
    std::shared_ptr<XmlElement> &root,
    const std::string &encoding,
    const std::string &version,
    const bool &standalone
)
{
    if (!IsValidEncoding(encoding))
    {
        throw new std::exception("Invalid encoding");
    }

    std::string _version =
        IsValidXmlVersion(version) ? version : DEFAULT_XML_VERSION;

    std::string _standalone = standalone ? "yes" : "no";

    std::shared_ptr<XmlDocument> document =
        std::make_shared<XmlDocument>(
        root,
        encoding,
        _version,
        _standalone
        );

    root->SetParent(document);

    return document;
}

bool XercesAdapter::IsValidEncoding(const std::string &encoding)
{
    // TODO: Implement
    return false;
}

bool XercesAdapter::IsValidXmlVersion(const std::string &version)
{
    return std::stol(version) == 1.0 ||
        std::stol(version) == 1.1;
}

void XercesAdapter::AddXmlChildElement(
    std::shared_ptr<XmlElement> child,
    std::shared_ptr<XmlElement> insertBefore,
    std::shared_ptr<XmlElement> parent
)
{
    if (child->HasParent())
    {
        throw std::exception("Has parent or is root");
    }

    auto _parent = parent->GetDomElement();
    auto _child = child->GetDomElement();
    DOMElement *_insertBefore =
        insertBefore == nullptr ?
        nullptr :
        insertBefore->GetDomElement();

    try
    {
        _parent->insertBefore(_child, _insertBefore);
    }
    catch (const DOMException &e)
    {
        std::string errorMessage = XmlChToString(e.getMessage()).Get();
        std::cout << errorMessage << std::endl;

        throw new std::exception(errorMessage.c_str());
    }
    catch (...)
    {
        // TODO: Unexpected Error message
        throw new std::exception("Unknow Error");
    }

    child->SetParent(parent);
}
////---------------------------------------------------------

//// Converter-----------------------------------------------

StringToXmlCh::StringToXmlCh(const std::string &str)
{
    _xmlCh = XMLString::transcode(str.c_str());
}

StringToXmlCh::~StringToXmlCh()
{
    XMLString::release(&_xmlCh);

    _xmlCh = nullptr;
}

XMLCh* StringToXmlCh::Get() const
{
    return _xmlCh;
}

XmlChToString::XmlChToString(const XMLCh *xmlCh)
{
    _strChars = XMLString::transcode(xmlCh);
}

XmlChToString::~XmlChToString()
{
    XMLString::release(&_strChars);

    _strChars = nullptr;
}

std::string XmlChToString::Get() const
{
    std::stringstream sstream;
    sstream << _strChars;
    
    return sstream.str();
}
//------------------------------------------------------------------

int main()
{
    TestCase1();
    TestCase2();
    return 0;
}

void TestCase1()
{
    // Basic XML Node Create

    std::cout << "Test Case 1" << std::endl;
    //int b = 0;
    //auto a = new (b) float(1.0);
    std::shared_ptr<XmlElement> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element1",
            "Hello"
        );

    std::cout << element1->ToString() << std::endl;
}

void TestCase2()
{
    // Add Child
    std::cout << "Test Case 2" << std::endl;

    std::shared_ptr<XmlElement> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element1",
            "Hello"
        );

    std::shared_ptr<XmlElement> element2 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element2",
            "Hello"
        );

    std::shared_ptr<XmlElement> element3 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element3",
            "Hello"
        );

    XercesAdapter::GetInstance().AddXmlChildElement(element2, nullptr, element1);

    try
    {
        XercesAdapter::GetInstance().AddXmlChildElement(element2, nullptr, element1);
    }
    catch (const std::exception&)
    {
        //
    }

    XercesAdapter::GetInstance().AddXmlChildElement(element3, element2, element1);

    element1.reset();

    std::cout << element2->ToString() << std::endl;
}
