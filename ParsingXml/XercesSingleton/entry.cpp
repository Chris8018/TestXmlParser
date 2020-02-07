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

XmlDocument::XmlDocument(
    std::shared_ptr<XmlElement> rootElement,
    const std::string &version,
    const std::string &encoding,
    const bool &standalone)
{
    if (rootElement->IsAssigned())
    {
        throw std::exception("Is Assigned!!!!");
    }

    this->_xmlDocument = rootElement->_ownerDocument;

    rootElement->RemoveOwnerDocument();

    // TODO: Consider check Version in here instead of outside
    this->_xmlDocument
        ->setXmlVersion(StringToXmlCh(version).Get());

    // TODO: Encoding Check
    this->_encoding = encoding;

    this->_xmlDocument->setXmlStandalone(standalone);
}

XmlDocument::~XmlDocument()
{
    //_xmlDocument.reset();
    _xmlDocument->release();

    _xmlDocument = nullptr;
}

bool XmlDocument::IsNotValidEncoding(const std::string &encoding) const
{
    // TODO: Update this
    return false;
}

std::string XmlDocument::ToString() const
{
    return "";
    //return XercesAdapter::GetInstance().NodeToString(_xmlDocument);
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(std::string name)
{
    _ownerDocument = XercesAdapter::GetInstance().CreateEmptyDocument();

    _xmlElement = _ownerDocument
        ->createElement(StringToXmlCh(name).Get());
}

XmlElement::~XmlElement()
{
    _xmlElement = nullptr;

    if (!IsAssigned())
    {
        _ownerDocument->release();

        _ownerDocument = nullptr;
    }
}

bool XmlElement::IsAssigned() const
{
    return _assigned;
}

void XmlElement::RemoveOwnerDocument()
{
    if (_ownerDocument != nullptr)
    {
        _ownerDocument = nullptr;

        _assigned = true;
    }
}

std::string XmlElement::ToString() const
{
    return "";
}

//------------------------------------------------------------------

// XmlLib-----------------------------------------------------------

//std::shared_ptr<XmlDocument> XmlLib::CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone)
//{
//    std::string _version = CorrectingXmlVersion(version);
//    return std::make_shared<XmlDocument>(rootNode, _version, encoding, standalone);
//}

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

//void XercesXmlWriter::Clear()
//{
//    //_writer.reset();
//}
//
//void XercesXmlWriter::Init()
//{
//    SetErrorHandler();
//    SetEncoding();
//    SetPrettyPrintFormat();
//}

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

//std::string XercesXmlWriter::Write(std::shared_ptr<DOMNode> domNode)
//{
//    std::shared_ptr<XMLCh> xmlChs(_writer->writeToString(domNode.get()));
//
//    std::shared_ptr<char> chars(XMLString::transcode(xmlChs.get()));
//
//    std::string str;
//    str.push_back(*chars);
//
//    return str;
//}

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

    _xmlWriter = std::make_shared<XercesXmlWriter>(_domImpl);
}

XercesAdapter::~XercesAdapter()
{
    //Clear();
    //_domImpl.reset();
    //_domImpl->release

    _domImpl = nullptr;

    // TODO: considering catching error
    XMLPlatformUtils::Terminate();
}

//void XercesAdapter::Clear()
//{
//    //_domImpl.reset(); // Use release
//
//    //_xmlWriter.reset();
//}

//void XercesAdapter::InitializeDomImplementation()
//{
//    if (_domImpl == nullptr)
//        _domImpl = std::shared_ptr<DOMImplementation>(DOMImplementationRegistry::getDOMImplementation(u""));
//    if (_xmlWriter == nullptr)
//        _xmlWriter = std::make_shared<XercesXmlWriter>(_domImpl);
//}

DOMDocument* XercesAdapter::CreateEmptyDocument()
{
    return _domImpl->createDocument(0, 0, 0);
}

XercesAdapter& XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}
////---------------------------------------------------------

//// Converter-----------------------------------------------
//std::shared_ptr<XMLCh> XStr::StringToXmlCh(const std::string &str)
//{
//    return std::shared_ptr<XMLCh>(XMLString::transcode(str.c_str()));
//}
//
//std::string XStr::XmlChToString(const XMLCh* xmlCh)
//{
//    std::shared_ptr<char> convertedChar(XMLString::transcode(xmlCh));
//
//    std::string str;
//    str.push_back(*convertedChar);
//
//    return str;
//}

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

    std::shared_ptr<XmlElement> element1 = std::make_shared<XmlElement>("Element1");
    std::shared_ptr<XmlElement> element2 = std::make_shared<XmlElement>("Element2");
    std::shared_ptr<XmlElement> element3 = std::make_shared<XmlElement>("Element3");
    std::shared_ptr<XmlElement> element4 = std::make_shared<XmlElement>("Element4");

    std::shared_ptr<XmlDocument> document1 = std::make_shared<XmlDocument>(element1, "1.0", "UTF-8", false);
}