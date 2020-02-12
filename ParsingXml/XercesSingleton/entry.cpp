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

using xercesc::DOMDocument;
using xercesc::DOMElement;
using xercesc::DOMNode;

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

// XmlDocument------------------------------------------------------
XmlDocument::XmlDocument(std::shared_ptr<XmlElement> rootElement)
    : XmlDocument(rootElement, "UTF-8", "1.0", "no") {};

XmlDocument::XmlDocument(
    std::shared_ptr<XmlElement> rootElement,
    const std::string &encoding,
    const std::string &version,
    const std::string &standalone)
{
    // TODO: Throw if Element HasParent
    _rootElement = rootElement;
    _rootElement->SetIsRoot(true);

    _encoding = encoding;
    _version = version;
    _standalone = standalone;
}

XmlDocument::~XmlDocument()
{
}

std::string XmlDocument::ToString() const
{
    return "";
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(DOMElement *element)
{
    _internalElement = element;
}

XmlElement::~XmlElement()
{
    _internalElement->release();
    _internalElement = nullptr;
}

DOMElement* XmlElement::GetDomElement()
{
    return _internalElement;
}

bool XmlElement::HasParent() const
{
    if (_isRoot ||
        _internalElement->getParentNode() != nullptr)
    {
        return true;
    }

    return false;
}

void XmlElement::SetIsRoot(const bool &isRoot)
{
    _isRoot = isRoot;
}

bool XmlElement::IsRoot() const
{
    return _isRoot;
}

std::string XmlElement::ToString() const
{
    return "";
}

//------------------------------------------------------------------

// XmlLib-----------------------------------------------------------

std::shared_ptr<XmlDocument> XmlLib::CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone)
{
    /*std::string _version = CorrectingXmlVersion(version);
    return std::make_shared<XmlDocument>(rootNode, _version, encoding, standalone);*/
    return nullptr;
}

std::string XmlLib::GetSupportXmlVersion(const std::string &encoding)
{
    std::string result = encoding;

    if (result != "1.0" || result != "1.1")
    {
        // TODO: Write Log Warning
        result = DEFAULT_XML_VERSION; // Separate to Macro
    }

    return result;
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

    // Not Use XercesAdapter Converter to avoid Circular Dependancy
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
    _outStream = nullptr;

    _writer->release();
    _writer = nullptr;
}

DOMConfiguration* XercesXmlWriter::GetConfiguration() const
{
    return _writer->getDomConfig();
}

void XercesXmlWriter::SetErrorHandler()
{
    GetConfiguration()->setParameter(ERROR_HANDLER, _errorHandler.get());
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
    return GetConfiguration()->canSetParameter(PRETTY_PRINT, true);
}

void XercesXmlWriter::SetPrettyPrintFormat()
{
    if (CanSetPrettyPrint())
        GetConfiguration()->setParameter(PRETTY_PRINT, true);
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
    _xmlStringWriter = nullptr;

    _dummyDoc->release();
    _dummyDoc = nullptr;

    _domImpl = nullptr;

    // TODO: considering catching error
    XMLPlatformUtils::Terminate();
}

XercesAdapter &XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::string NodeToString(std::shared_ptr<XmlElement> xmlElement)
{
    return "";
}

std::string NodeToString(std::shared_ptr<XmlDocument> xmlDocument)
{
    return "";
}

DOMDocument* XercesAdapter::CreateEmptyDOMDocument()
{
    return _domImpl->createDocument(0, 0, 0);
}

std::shared_ptr<XmlElement> XercesAdapter::CreateXmlElement(
    const std::map<std::string, std::string> &attributes,
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

    // TODO: Check Empty String
    element->setTextContent(StringToXmlCh(text).Get());

    // Todo: Set Attributes
    return std::make_shared<XmlElement>(element);
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

void XercesAdapter::SetAttributes(
    DOMElement *element,
    std::map<std::string, std::string> attributes
)
{
    //
}

std::shared_ptr<XmlDocument> XercesAdapter::CreateXmlDocument(
    std::shared_ptr<XmlElement> &root,
    const std::string &encoding,
    const std::string &version,
    const bool &standalone
)
{
    if (IsNotValidEncoding(encoding))
    {
        throw new std::exception("Invalid encoding");
    }

    std::string _version =
        IsValidXmlVersion(version) ? version : DEFAULT_XML_VERSION;

    std::string _standalone = standalone ? "yes" : "no";

    return std::make_shared<XmlDocument>(
        root,
        encoding,
        _version,
        _standalone
        );
}

bool XercesAdapter::IsNotValidEncoding(const std::string &encoding)
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
    //
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

XmlChToString::XmlChToString(const char16_t *xmlCh)
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
    return 0;
}

void TestCase1()
{
    //// Basic XML Node Create

    std::cout << "Test Case 1" << std::endl;

    //std::shared_ptr<XmlElement> element1 = std::make_shared<XmlElement>("Element1");
    //std::shared_ptr<XmlElement> element2 = std::make_shared<XmlElement>("Element2");
    //std::shared_ptr<XmlElement> element3 = std::make_shared<XmlElement>("Element3");
    //std::shared_ptr<XmlElement> element4 = std::make_shared<XmlElement>("Element4");

    /*std::shared_ptr<XmlDocument> document1 = std::make_shared<XmlDocument>(element1, "1.0", "UTF-8", false);*/

    //std::cout << element1->ToString() << std::endl;

    ////std::cout << document1->ToString() << std::endl;

    //std::cout << element2->ToString() << std::endl;

    //std::cout << element3->ToString() << std::endl;

    //

    std::shared_ptr<XmlElement> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element1",
            "Hello"
        );
    //
}