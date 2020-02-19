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

// XmlNodeWrapper----------------------------------------------------------

//------------------------------------------------------------------

// XmlCData---------------------------------------------------------

//------------------------------------------------------------------

// XmlText----------------------------------------------------------

//------------------------------------------------------------------

// XmlProcessingInstruction-----------------------------------------

//------------------------------------------------------------------

// XmlComment-------------------------------------------------------

//------------------------------------------------------------------

// XmlOtherNode-----------------------------------------------------

//------------------------------------------------------------------

// XmlDocumentWrapper------------------------------------------------------

XmlDocumentWrapper::XmlDocumentWrapper(
    xercesc::DOMDocument *documentNode,
    std::shared_ptr<XmlElementWrapper> rootElement,
    const std::string &encoding
)
{
    if (rootElement->HasParent())
    {
        // TODO: Throw if Element HasParent
        throw std::exception("Has parent");
    }

    _internalDocument = documentNode;

    _rootElement = rootElement;

    _encoding = encoding;
}

XmlDocumentWrapper::~XmlDocumentWrapper()
{
    _internalDocument->release();
}

DOMDocument* XmlDocumentWrapper::GetDomDocument() const
{
    return _internalDocument;
}

std::string XmlDocumentWrapper::GetEncoding() const
{
    return _encoding;
}

std::shared_ptr<XmlElementWrapper> XmlDocumentWrapper::GetRootElement() const
{
    return _rootElement;
}

void XmlDocumentWrapper::NotifyDeepValueChanged()
{
    auto currentRootElement = _internalDocument->getDocumentElement();

    auto rootElement = _rootElement->GetDomElement();

    if (currentRootElement == nullptr)
    {
        auto importedNode = ImportNode(rootElement);
        _internalDocument->appendChild(importedNode);
    }
    else if (!currentRootElement->isEqualNode(rootElement))
    {
        auto importedNode = ImportNode(rootElement);
        _internalDocument->replaceChild(importedNode, currentRootElement);
    }
}

DOMNode* XmlDocumentWrapper::ImportNode(DOMElement *toBeImport, bool deep)
{
    return _internalDocument->importNode(toBeImport, deep);
}

std::string XmlDocumentWrapper::ToString() const
{
    return XercesAdapter::GetInstance().NodeToString(*this);
}

//------------------------------------------------------------------

// XmlElementWrapper-------------------------------------------------------

XmlElementWrapper::XmlElementWrapper(DOMElement *element)
{
    _internalElement = element;
}

XmlElementWrapper::~XmlElementWrapper()
{
    if (!HasParent())
        _internalElement->release();
}

DOMElement* XmlElementWrapper::GetDomElement() const
{
    return _internalElement;
}

bool XmlElementWrapper::IsRoot() const
{
    auto document =
        std::dynamic_pointer_cast<XmlDocumentWrapper>(_parent);

    return document != nullptr;
}

std::shared_ptr<XmlNodeWrapper> XmlElementWrapper::GetParent() const
{
    return _parent;
}

void XmlElementWrapper::SetParent(std::shared_ptr<XmlNodeWrapper> parentNode)
{
    _parent = parentNode;
}

bool XmlElementWrapper::HasParent() const
{
    return _parent != nullptr;
}


std::string XmlElementWrapper::GetTagName() const
{
    return XmlChToString(_internalElement->getTagName()).Get();
}

void XmlElementWrapper::NotifyDeepValueChanged()
{
    if (HasParent())
        _parent->NotifyDeepValueChanged();
}

std::string XmlElementWrapper::GetText() const
{
    // TODO: Implement
    return "";
}

void XmlElementWrapper::SetText(DOMText *textNode)
{
    ClearTextAndCData();

    _internalElement->appendChild(textNode);
}

XmlAttributes XmlElementWrapper::GetAttributes() const
{
    // TODO: Implement
    return XmlAttributes();
}

void XmlElementWrapper::SetAttributes(XmlAttributes attributes)
{
    ClearAttributes();

    for (auto it = attributes.begin(); it != attributes.end(); it++)
        SetAttribute(it->first, it->second);
}

void XmlElementWrapper::SetAttribute(
    std::string name,
    std::string value
)
{
    _internalElement->setAttribute(
        StringToXmlCh(name).Get(),
        StringToXmlCh(value).Get()
    );
}

void XmlElementWrapper::InsertChildBefore(
    std::shared_ptr<XmlElementWrapper> child,
    std::shared_ptr<XmlElementWrapper> insertBefore
)
{
    if (child->HasParent())
    {
        throw std::exception("Has parent or is root");
    }

    auto _child = child->GetDomElement();
    DOMElement *_insertBefore =
        insertBefore == nullptr ?
        nullptr :
        insertBefore->GetDomElement();

    this->_internalElement->insertBefore(_child, _insertBefore);
}

std::shared_ptr<XmlElementWrapper> XmlElementWrapper::CopyElement(bool deep) const
{
    DOMElement *copy = static_cast<DOMElement*>(_internalElement->cloneNode(deep));

    return std::make_shared<XmlElementWrapper>(copy);
}

std::string XmlElementWrapper::ToString() const
{
    return XercesAdapter::GetInstance()
        .NodeToString(*this);
}

void XmlElementWrapper::ClearTextAndCData()
{
    auto nodesList = _internalElement->getChildNodes();
    std::list<DOMNode*> nodesToBeRemoved;

    for (size_t i = 0; i < nodesList->getLength(); i++)
    {
        Xerces_NodeType type = nodesList->item(i)->getNodeType();
        if (type == Xerces_NodeType::CDATA_SECTION_NODE ||
            type == Xerces_NodeType::TEXT_NODE)
            nodesToBeRemoved.push_back(nodesList->item(i));
    }

    nodesList = nullptr; // Don't delete this

    for (auto it = nodesToBeRemoved.begin(); it != nodesToBeRemoved.end(); it++)
    {
        _internalElement->removeChild(*it);
        (*it)->release();
    }

    nodesToBeRemoved.clear();
}

void XmlElementWrapper::ClearAttributes()
{
    auto attributesList = _internalElement->getAttributes();

    std::list<DOMAttr*> attributesToBeRemoved;

    for (int i = 0; i < attributesList->getLength(); i++)
    {
        attributesToBeRemoved.push_back(
            static_cast<DOMAttr*>(attributesList->item(i))
        );
    }

    for (auto it = attributesToBeRemoved.begin(); it != attributesToBeRemoved.end(); it++)
    {
        _internalElement->removeAttributeNode(*it);
    }
}

//------------------------------------------------------------------

// XmlLib-----------------------------------------------------------
std::shared_ptr<XmlDocumentWrapper> XmlLib::CreateXmlDocument(
    const std::string &encoding,
    std::shared_ptr<XmlElementWrapper> rootNode,
    const bool &standalone,
    const std::string &version
)
{
    /*std::string _version = CorrectingXmlVersion(version);
    return std::make_shared<XmlDocumentWrapper>(rootNode, _version, encoding, standalone);*/
    return nullptr;
}

std::shared_ptr<XmlElementWrapper> XmlLib::CreateXmlElement(
    const XmlAttributes &attributes,
    const std::string &name,
    const std::string &text
)
{
    return nullptr;
}

void XmlLib::AddXmlChildElement(
    std::shared_ptr<XmlElementWrapper> child,
    std::shared_ptr<XmlElementWrapper> insertBefore,
    std::shared_ptr<XmlElementWrapper> parent
)
{
    //
}

void XmlLib::SetXmlElementAttributes(
    const XmlAttributes &attributes,
    std::shared_ptr<XmlElementWrapper> element
)
{
    //
}

void XmlLib::SetXmlElementAttribute(
    std::shared_ptr<XmlElementWrapper> element,
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
    return WriteToString(domNode, DEFAULT_ENCODING);
}

std::string XercesXmlWriter::WriteToString(
    xercesc::DOMNode *domNode,
    std::string encoding
)
{
    std::shared_ptr<MemBufFormatTarget> formatTarget =
        std::make_shared<MemBufFormatTarget>();

    _outStream->setByteStream(formatTarget.get());

    _outStream->setEncoding(StringToXmlCh(encoding).Get());

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

//-----------------------------------

XercesAdapter::XercesAdapter()
{
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (std::exception &e)
    {
        // TODO: Print error message
    }
    catch (...)
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

std::string XercesAdapter::NodeToString(const XmlElementWrapper &element)
{
    return _xmlStringWriter
        ->WriteToString(element.GetDomElement());
}

std::string XercesAdapter::NodeToString(const XmlDocumentWrapper &document)
{
    auto domDocument = document.GetDomDocument();
    auto rootDomElement = document.GetRootElement()->GetDomElement();


    return _xmlStringWriter
        ->WriteToString(
            domDocument,
            document.GetEncoding()
            );
}

DOMDocument* XercesAdapter::CreateEmptyDOMDocument()
{
    return _domImpl->createDocument(0, 0, 0);
}

std::shared_ptr<XmlElementWrapper> XercesAdapter::CreateXmlElement(
    const XmlAttributes &attributes,
    const std::string &name,
    const std::string &text
)
{
    std::shared_ptr<XmlElementWrapper> xmlElement;

    try
    {
        DOMElement *element =
            _dummyDoc->createElement(StringToXmlCh(name).Get());

        xmlElement = std::make_shared<XmlElementWrapper>(element);

        SetXmlElementText(xmlElement, text);

        xmlElement->SetAttributes(attributes);
    }
    catch (const DOMException &e)
    {
        // TODO: Change Error Message
        std::string errorMessage =
            XmlChToString(e.getMessage()).Get();

        std::cout << errorMessage << std::endl;

        throw std::exception(errorMessage.c_str());
    }
    catch (...)
    {
        // TODO: unexpected error
        throw std::exception("Unexpected Error");
    }


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

std::shared_ptr<XmlDocumentWrapper> XercesAdapter::CreateXmlDocument(
    std::shared_ptr<XmlElementWrapper> root,
    const std::string &encoding,
    const std::string &version,
    const bool &standalone
)
{
    if (!IsValidEncoding(encoding))
    {
        throw std::exception("Invalid encoding");
    }

    std::string _version =
        IsValidXmlVersion(version) ? version : DEFAULT_XML_VERSION;

    DOMDocument *document = CreateEmptyDOMDocument();
    document->setXmlVersion(StringToXmlCh(_version).Get());
    document->setXmlStandalone(standalone);

    std::shared_ptr<XmlDocumentWrapper> xmlDocument =
        std::make_shared<XmlDocumentWrapper>(
            document,
            root,
            encoding
            );

    root->SetParent(xmlDocument);

    xmlDocument->NotifyDeepValueChanged();

    return xmlDocument;
}

bool XercesAdapter::IsValidEncoding(const std::string &encoding)
{
    // TODO: Implement
    return true;
}

bool XercesAdapter::IsValidXmlVersion(const std::string &version)
{
    return std::stol(version) == 1.0 ||
        std::stol(version) == 1.1;
}

void XercesAdapter::AddXmlChildElement(
    std::shared_ptr<XmlElementWrapper> child,
    std::shared_ptr<XmlElementWrapper> insertBefore,
    std::shared_ptr<XmlElementWrapper> parent
)
{
    try
    {
        parent->InsertChildBefore(child, insertBefore);

        child->SetParent(parent);

        parent->NotifyDeepValueChanged();
    }
    catch (...)
    {
        // TODO: Catch Error
        // TODO: Print Error message
    }
}


void XercesAdapter::SetXmlElementAttributes(
    const XmlAttributes &attributes,
    std::shared_ptr<XmlElementWrapper> element
)
{
    try
    {
        element->SetAttributes(attributes);

        element->NotifyDeepValueChanged();
    }
    catch (...)
    {
        // TODO: Catch More Specific Error
        // TODO: Print error message
    }
}

void XercesAdapter::SetXmlElementAttribute(
    std::shared_ptr<XmlElementWrapper> element,
    const std::string &name,
    const std::string &value
)
{
    try
    {
        element->SetAttribute(name, value);

        element->NotifyDeepValueChanged();
    }
    catch (const DOMException & e)
    {
        // TODO: Change Error Message
        std::string errorMessage =
            XmlChToString(e.getMessage()).Get();

        std::cout << errorMessage << std::endl;

        throw std::exception(errorMessage.c_str());
    }
    catch (...)
    {
        // TODO: Print Error
    }
}

void XercesAdapter::SetXmlElementText(
    std::shared_ptr<XmlElementWrapper> element,
    const std::string &text
)
{
    auto _text =
        _dummyDoc->createTextNode(StringToXmlCh(text).Get());

    element->SetText(_text);

    _text = nullptr; // Don't delete this

    element->NotifyDeepValueChanged();
}

std::shared_ptr<XmlElementWrapper> XercesAdapter::GetXmlRootElement(
    const std::shared_ptr<XmlDocumentWrapper> xmlDocument
) const
{
    return xmlDocument->GetRootElement();
}

std::string XercesAdapter::GetXmlElementName(
    const std::shared_ptr<XmlElementWrapper> xmlElement
) const
{
    return xmlElement->GetTagName();
}

std::shared_ptr<XmlElementWrapper> XercesAdapter::CopyXmlElement(
    const std::shared_ptr<XmlElementWrapper> original
)
{
    return original->CopyElement();
}

std::list<std::shared_ptr<XmlElementWrapper>> XercesAdapter::GetXmlElementChildElements(
    const std::shared_ptr<XmlElementWrapper> element,
    const std::string &elementName
) const
{
    // TODO: Implement
    return std::list<std::shared_ptr<XmlElementWrapper>>();
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
    std::cout << std::endl;
    TestCase2();
    std::cout << std::endl;
    TestCase3();
    return 0;
}

void TestCase1()
{
    // Basic XML Node Create

    std::cout << "Test Case 1" << std::endl;
    //int b = 0;
    //auto a = new (b) float(1.0);
    std::shared_ptr<XmlElementWrapper> element1 =
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

    std::shared_ptr<XmlElementWrapper> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element1",
            "Hello"
        );

    std::shared_ptr<XmlElementWrapper> element2 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            std::map<std::string, std::string>(),
            "Element2",
            "Hello"
        );

    std::shared_ptr<XmlElementWrapper> element3 =
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

    //element1.reset();

    std::cout << element1->ToString() << std::endl;
}

void TestCase3()
{
    // Test Document
    std::cout << "Test Case 3" << std::endl;

    std::shared_ptr<XmlElementWrapper> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            XmlAttributes(),
            "Element1",
            "Hello"
        );

    std::shared_ptr<XmlDocumentWrapper> document1 =
        XercesAdapter::GetInstance()
        .CreateXmlDocument(
            element1,
            "UTF-8",
            "1.0",
            false
        );

    std::cout << document1->ToString() << std::endl;
}
