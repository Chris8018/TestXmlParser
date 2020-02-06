#include "Entry.h"

#include "xercesc/util/PlatformUtils.hpp"

#include <xercesc/util/XMLString.hpp>

//#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMError.hpp>

#include <xercesc/dom/DOMLSSerializer.hpp>

#include "xercesc/dom/DOM.hpp"

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <xercesc/dom/StDOMNode.hpp>

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

using xercesc::StdOutFormatTarget;
using xercesc::LocalFileFormatTarget;

using xercesc::StDOMNode;

#define PRETTY_PRINT XMLUni::fgDOMWRTFormatPrettyPrint
#define ERROR_HANDLER XMLUni::fgDOMErrorHandler

#include <iostream>

#include <sstream>

// XmlDocument------------------------------------------------------

//XmlDocument::XmlDocument(
//    std::shared_ptr<XmlElement> rootElement,
//    const std::string &version,
//    const std::string &encoding,
//    const bool &standalone)
//{
//    if (rootElement->IsAssigned())
//    {
//        throw std::exception("Is Assigned!!!!");
//    }
//
//    //this->_xmlDocument = rootElement->_ownerDocument;
//
//    //rootElement->RemoveOwnerDocument();
//
//    this->_xmlDocument
//        ->setXmlVersion(
//            XercesAdapter::GetInstance()
//            .StringToXmlCh(version)
//            .get()
//        );
//
//    this->_encoding = encoding;
//
//    this->_xmlDocument->setXmlStandalone(standalone);
//}

XmlDocument::~XmlDocument()
{
    //_xmlDocument.reset();
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
    _assigned = false;
}

XmlElement::~XmlElement()
{
    if (!_assigned)
    {
        std::cout << "Not assigned" << std::endl;
    }
}

bool XmlElement::IsAssigned() const
{
    return _assigned;
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
        result = "1.0"; // Separate to Macro
    }

    return result;
}

//------------------------------------------------------------------

// XercesAdapter----------------------------------------------------

DOMPrintErrorHandler::DOMPrintErrorHandler() {}

DOMPrintErrorHandler::~DOMPrintErrorHandler() {}

void DOMPrintErrorHandler::ResetErrors() {}

bool DOMPrintErrorHandler::HandleError(const DOMError &domError)
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
    std::shared_ptr<char> msg(
        XMLString::transcode(domError.getMessage()));

    std::cerr << *msg << std::endl;

    // Instructs the serializer to continue serialization if possible.
    return true;
}

//XercesXmlWriter::XercesXmlWriter(
//    std::shared_ptr<DOMImplementation> domImpl)
//{
//}

XercesXmlWriter::~XercesXmlWriter()
{
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

//void XercesXmlWriter::SetErrorHandler()
//{
//    _configuration->setParameter(ERROR_HANDLER, _errorHandler.get());
//}
//
//void XercesXmlWriter::SetEncoding(const std::string &encoding)
//{
//    std::shared_ptr<XMLCh> _encoding(XMLString::transcode(encoding.c_str()));
//    _outStream->setEncoding(_encoding.get());
//}

//bool XercesXmlWriter::CanSetPrettyPrint()
//{
//    return _configuration->canSetParameter(PRETTY_PRINT, true);
//}

//void XercesXmlWriter::SetPrettyPrintFormat()
//{
//    if (CanSetPrettyPrint())
//        _configuration->setParameter(PRETTY_PRINT, true);
//}

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
}

XercesAdapter::~XercesAdapter()
{
    //Clear();

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

XercesAdapter& XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::shared_ptr<XMLCh> XercesAdapter::StringToXmlCh(const std::string &str)
{
    return std::shared_ptr<XMLCh>(XMLString::transcode(str.c_str()));
}

std::string XercesAdapter::XmlChToString(const std::shared_ptr<XMLCh> xmlCh)
{
    std::shared_ptr<char> convertedChar(XMLString::transcode(xmlCh.get()));

    std::string str;
    str.push_back(*convertedChar);

    return str;
}

//------------------------------------------------------------------


void TestCase1();

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

    //XmlElement element1("Element1");

    //std::shared_ptr<XmlElement> element2 = std::make_shared<XmlElement>("Element2");
    //std::shared_ptr<XmlElement> element3 = std::make_shared<XmlElement>("Element3");
    //std::shared_ptr<XmlElement> element4 = std::make_shared<XmlElement>("Element4");

    //std::shared_ptr<XmlElement> element1 = XmlLib::CreateXmlElement("Element1", "", std::map<std::string, std::string>());

    //auto element1 = XercesAdapter::GetInstance().CreateEmptyDocument();
    //int count = element1.use_count();

    //std::cout << count << std::endl;

    //element1.reset();
}