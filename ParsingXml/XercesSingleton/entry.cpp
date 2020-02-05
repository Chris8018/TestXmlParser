#include "Entry.h"

#include "xercesc/util/PlatformUtils.hpp"

#include <xercesc/util/XMLString.hpp>

//#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMError.hpp>

#include <xercesc/dom/DOMLSSerializer.hpp>

#include "xercesc/dom/DOM.hpp"

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

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

#define PRETTY_PRINT XMLUni::fgDOMWRTFormatPrettyPrint
#define ERROR_HANDLER XMLUni::fgDOMErrorHandler

#include <iostream>

#include <sstream>

int main()
{
    std::cout << "Hello World!" << std::endl;
    return 0;
}

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

    this->_xmlDocument
        ->setXmlVersion(
            XercesAdapter::GetInstance()
            .StringToXmlCh(version)
            .get()
        );

    this->_encoding = encoding;

    this->_xmlDocument->setXmlStandalone(standalone);
}

XmlDocument::~XmlDocument()
{
    _xmlDocument.reset();
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(std::string name)
{
    this->_ownerDocument =
        XercesAdapter::GetInstance()
        .CreateEmptyDocument();

    this->_xmlElement = 
        XercesAdapter::GetInstance()
        .CreateElementFromDocument(_ownerDocument, name);
}

XmlElement::~XmlElement()
{
    _xmlElement.reset();

    RemoveOwnerDocument();
}

void XmlElement::RemoveOwnerDocument()
{
    if (_ownerDocument != nullptr)
    {
        _ownerDocument.reset();

        _assigned = true;
    }
}

void XmlElement::SetXmlElement(std::shared_ptr<DOMElement> otherElement)
{
    this->_xmlElement.reset(otherElement.get());

    RemoveOwnerDocument();
}

bool XmlElement::IsAssigned() const
{
    return _assigned;
}

void XmlElement::SetText(const std::string &text)
{
    _xmlElement
        ->setTextContent(
        XercesAdapter::GetInstance()
            .StringToXmlCh(text)
            .get());
}

void XmlElement::SetAttribute(const std::string &name, const std::string &value)
{
    auto _name = XercesAdapter::GetInstance()
        .StringToXmlCh(name);
    auto _value = XercesAdapter::GetInstance()
        .StringToXmlCh(value);

    _xmlElement->setAttribute(_name.get(), _value.get());
}

void XmlElement::SetAttributes(const std::map<std::string, std::string> &attributes)
{
    for (auto it = attributes.begin(); it != attributes.end(); it++)
    {
        SetAttribute(it->first, it->second);
    }
}

void XmlElement::AddChild(std::shared_ptr<XmlElement> child)
{
    if (child->IsAssigned())
    {
        throw new std::exception("Has been added to an element.");
    }

    child->SetXmlElement(
        XercesAdapter::GetInstance()
        .CopyElementToDocumentOfElement(child->_xmlElement, this->_xmlElement, true));

    this->_xmlElement->appendChild(child->_xmlElement.get());
}

void XmlElement::InsertChildBefore(std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> element)
{
    if (this->IsAssigned())
    {
        throw new std::exception("Has been added to an element.");
    }

    this->SetXmlElement(
        XercesAdapter::GetInstance()
        .CopyElementToDocumentOfElement(this->_xmlElement, element->_xmlElement, true));

    this->_xmlElement->insertBefore(
        child->_xmlElement.get(),
        element->_xmlElement.get());
}

std::string XmlElement::ToString() const
{
    return "";
}

//------------------------------------------------------------------

// XmlLib-----------------------------------------------------------

std::shared_ptr<XmlDocument> XmlLib::CreateXmlDocument(std::shared_ptr<XmlElement> rootNode, const std::string &version, const std::string &encoding, const bool &standalone)
{
    std::string _version = CorrectingXmlVersion(version);
    return std::make_shared<XmlDocument>(rootNode, _version, encoding, standalone);
}

std::string XmlLib::CorrectingXmlVersion(const std::string &encoding)
{
    std::string result = encoding;

    if (result != "1.0" || result != "1.1")
    {
        // TODO: Write Log Warning
        result = "1.0"; // Separate to Macro
    }

    return result;
}

std::shared_ptr<XmlElement> XmlLib::CreateXmlElement(const std::string &name, const std::string &text, const std::map<std::string, std::string> &attributes)
{
    std::shared_ptr<XmlElement> element = std::make_shared<XmlElement>(name);

    element->SetText(text);

    if (attributes.size() > 0)
        element->SetAttributes(attributes);

    return element;
}

void XmlLib::AddXmlChildElement(std::shared_ptr<XmlElement> parent, std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> insertBefore)
{
    if (insertBefore == nullptr)
        parent->AddChild(child);
    else
        parent->InsertChildBefore(child, insertBefore);
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

XercesXmlWriter::XercesXmlWriter(
    std::shared_ptr<DOMImplementation> domImpl)
{
    _writer = std::shared_ptr<DOMLSSerializer>(
        domImpl->createLSSerializer());

    _configuration =
        std::shared_ptr<DOMConfiguration>(
            _writer->getDomConfig());

    _errorHandler = std::shared_ptr<DOMPrintErrorHandler>();

    _outStream = std::shared_ptr<DOMLSOutput>(domImpl->createLSOutput());

    Init();
}

XercesXmlWriter::~XercesXmlWriter()
{
    Clear();
}

void XercesXmlWriter::Clear()
{
    if (_errorHandler != nullptr)
        _errorHandler.reset();

    if (_configuration != nullptr)
        _configuration.reset();

    if (_writer != nullptr)
        _writer.reset();
}

void XercesXmlWriter::Init()
{
    SetErrorHandler();
    SetEncoding();
    SetPrettyPrintFormat();
}

void XercesXmlWriter::SetErrorHandler()
{
    _configuration->setParameter(ERROR_HANDLER, _errorHandler.get());
}

void XercesXmlWriter::SetEncoding(const std::string &encoding)
{
    std::shared_ptr<XMLCh> _encoding(XMLString::transcode(encoding.c_str()));
    _outStream->setEncoding(_encoding.get());
}

bool XercesXmlWriter::CanSetPrettyPrint()
{
    return _configuration->canSetParameter(PRETTY_PRINT, true);
}

void XercesXmlWriter::SetPrettyPrintFormat()
{
    if (CanSetPrettyPrint())
        _configuration->setParameter(PRETTY_PRINT, true);
}

ToFileWriter::ToFileWriter(std::shared_ptr<xercesc::DOMImplementation> domImpl, std::string path) : XercesXmlWriter(domImpl)
{
    _formatTarget = std::shared_ptr<XMLFormatTarget>(new LocalFileFormatTarget(path.c_str()));

    _outStream->setByteStream(_formatTarget.get());
}

ToFileWriter::~ToFileWriter()
{
    if (_formatTarget != nullptr)
        _formatTarget.reset();
}

void ToFileWriter::Write(std::shared_ptr<DOMNode> domNode)
{
    _writer->write(domNode.get(), _outStream.get());
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
    InitializeDomImplementation();
}

XercesAdapter::~XercesAdapter()
{
    Clear();

    // TODO: considering catching error
    XMLPlatformUtils::Terminate();
}

void XercesAdapter::Clear()
{
    _domImpl.reset();
}

void XercesAdapter::InitializeDomImplementation()
{
    if (_domImpl == nullptr)
        _domImpl = std::shared_ptr<DOMImplementation>(DOMImplementationRegistry::getDOMImplementation(u""));
}

XercesAdapter& XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::shared_ptr<DOMDocument> XercesAdapter::CreateEmptyDocument() const
{
    return std::shared_ptr<DOMDocument>(_domImpl->createDocument(0, 0, 0));
}

std::shared_ptr<DOMElement> XercesAdapter::CreateElementFromDocument(std::shared_ptr<DOMDocument> xmlDoc, const std::string &name)
{
    return std::shared_ptr<DOMElement>(xmlDoc->createElement(StringToXmlCh(name).get()));
}

std::shared_ptr<DOMElement> XercesAdapter::CopyElementToDocumentOfElement(
    std::shared_ptr<DOMElement> element, 
    std::shared_ptr<DOMElement> targetElement,
    bool deep)
{
    std::shared_ptr<DOMElement> copiedElement(
        static_cast<DOMElement*>(
            targetElement
            ->getOwnerDocument()
            ->importNode(element.get(), deep)
            )
    );
    return copiedElement;
}

std::shared_ptr<XMLCh> XercesAdapter::StringToXmlCh(const std::string &str)
{
    return std::shared_ptr<XMLCh>(XMLString::transcode(str.c_str()));
    return nullptr;
}

std::string XercesAdapter::XmlChToString(const std::shared_ptr<XMLCh> xmlCh)
{
    std::shared_ptr<char> convertedChar(XMLString::transcode(xmlCh.get()));

    std::string str;
    str.push_back(*convertedChar);

    return str;
}

//------------------------------------------------------------------
