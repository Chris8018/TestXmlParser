#include "Entry.h"

#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/util/XMLString.hpp>

//#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMError.hpp>

#include <xercesc/dom/DOMLSSerializer.hpp>

#include <xercesc/dom/DOM.hpp>

#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

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

using xercesc::TranscodeFromStr;

#define PRETTY_PRINT XMLUni::fgDOMWRTFormatPrettyPrint
#define ERROR_HANDLER XMLUni::fgDOMErrorHandler

#define DEFAULT_XML_VERSION "1.0"
#define DEFAULT_ENCODING "UTF-8"

#include <iostream>

#include <sstream>

// XmlNode----------------------------------------------------------

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

// XmlDocument------------------------------------------------------

XmlDocument::XmlDocument(
    DOMDocument *documentNode,
    std::shared_ptr<XmlElement> rootElement,
    const std::string &encoding
)
{
    if (rootElement->HasParent())
    {
        // TODO: Throw if Element HasParent
        throw std::exception();
    }

    _internalDocument = documentNode;

    _rootElement = rootElement;

    _encoding = encoding;
}

XmlDocument::~XmlDocument()
{
    _internalDocument->release();
}

DOMDocument* XmlDocument::GetDomDocument() const
{
    return _internalDocument;
}

std::string XmlDocument::GetEncoding() const
{
    return _encoding;
}

std::shared_ptr<XmlElement> XmlDocument::GetRootElement() const
{
    return _rootElement;
}

void XmlDocument::NotifyDeepValueChanged()
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

DOMNode* XmlDocument::ImportNode(DOMElement *toBeImport, bool deep)
{
    return _internalDocument->importNode(toBeImport, deep);
}

std::string XmlDocument::ToString() const
{
    return XercesAdapter::GetInstance().NodeToString(*this);
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

std::shared_ptr<XmlNode> XmlElement::GetParent() const
{
    return _parent;
}

void XmlElement::SetParent(std::shared_ptr<XmlNode> parentNode)
{
    _parent = parentNode;
}

bool XmlElement::HasParent() const
{
    return _parent != nullptr;
}


std::string XmlElement::GetTagName() const
{
    return XmlChToString(_internalElement->getTagName()).Get();
}

void XmlElement::NotifyDeepValueChanged()
{
    if (HasParent())
        _parent->NotifyDeepValueChanged();
}

std::string XmlElement::GetText() const
{
    // TODO: Implement
    return "";
}

void XmlElement::SetText(DOMText *textNode)
{
    ClearTextAndCData();

    _internalElement->appendChild(textNode);
}

XmlAttributes XmlElement::GetAttributes() const
{
    // TODO: Implement
    return XmlAttributes();
}

void XmlElement::SetAttributes(XmlAttributes attributes)
{
    ClearAttributes();

    for (auto it = attributes.begin(); it != attributes.end(); it++)
        SetAttribute(it->first, it->second);
}

void XmlElement::SetAttribute(
    std::string name,
    std::string value
)
{
    _internalElement->setAttribute(
        StringToXmlCh(name).Get(),
        StringToXmlCh(value).Get()
    );
}

void XmlElement::InsertChildBefore(
    std::shared_ptr<XmlElement> child,
    std::shared_ptr<XmlElement> insertBefore
)
{
    if (child->HasParent())
    {
        throw std::exception();
    }

    auto _child = child->GetDomElement();
    DOMElement *_insertBefore =
        insertBefore == nullptr ?
        nullptr :
        insertBefore->GetDomElement();

    this->_internalElement->insertBefore(_child, _insertBefore);
}

std::shared_ptr<XmlElement> XmlElement::CopyElement(bool deep) const
{
    DOMElement *copy = static_cast<DOMElement*>(_internalElement->cloneNode(deep));

    return std::make_shared<XmlElement>(copy);
}

std::string XmlElement::ToString() const
{
    return XercesAdapter::GetInstance()
        .NodeToString(*this);
}

void XmlElement::ClearTextAndCData()
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

void XmlElement::ClearAttributes()
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

DomWriterErrorHandler::DomWriterErrorHandler() {}

DomWriterErrorHandler::~DomWriterErrorHandler() {}

void DomWriterErrorHandler::resetErrors() {}

bool DomWriterErrorHandler::handleError(const DOMError &domError)
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
    _errorHandler = std::make_shared<DomWriterErrorHandler>();
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
    DOMNode *domNode,
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

    //_xmlStringWriter = new XercesXmlWriter(this->_domImpl);
}

XercesAdapter::~XercesAdapter()
{
    //delete _xmlStringWriter;

    _dummyDoc->release();

    // TODO: considering catching error
    XMLPlatformUtils::Terminate();
}

XercesAdapter &XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::string XercesAdapter::NodeToString(const XmlElement &element)
{
    std::shared_ptr<XercesXmlWriter> _xmlStringWriter =
        std::make_shared<XercesXmlWriter>(this->_domImpl);
    
    auto str = _xmlStringWriter
        ->WriteToString(element.GetDomElement());

    return str;
}

std::string XercesAdapter::NodeToString(const XmlDocument &document)
{
    auto domDocument = document.GetDomDocument();

    std::shared_ptr<XercesXmlWriter> _xmlStringWriter =
        std::make_shared<XercesXmlWriter>(this->_domImpl);

    std::string str = _xmlStringWriter
        ->WriteToString(
            domDocument,
            document.GetEncoding()
        );

    return str;
}

DOMDocument* XercesAdapter::CreateEmptyDOMDocument()
{
    return _domImpl->createDocument(0, 0, 0);
}

std::shared_ptr<XmlElement> XercesAdapter::CreateXmlElement(
    const std::string &name,
    const std::string &text,
    const XmlAttributes &attributes
)
{
    std::shared_ptr<XmlElement> xmlElement;

    try
    {
        DOMElement *element =
            _dummyDoc->createElement(StringToXmlCh(name).Get());

        xmlElement = std::make_shared<XmlElement>(element);

        SetXmlElementText(xmlElement, text);

        xmlElement->SetAttributes(attributes);
    }
    catch (const DOMException &e)
    {
        // TODO: Change Error Message
        std::string errorMessage =
            XmlChToString(e.getMessage()).Get();

        std::cout << errorMessage << std::endl;

        throw std::exception();
    }
    catch (...)
    {
        // TODO: unexpected error
        throw std::exception();
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

std::shared_ptr<XmlDocument> XercesAdapter::CreateXmlDocument(
    std::shared_ptr<XmlElement> root,
    const std::string &encoding,
    const std::string &version,
    const bool &standalone
)
{
    if (!IsValidEncoding(encoding))
    {
        throw std::exception();
    }

    std::string _version =
        IsValidXmlVersion(version) ? version : DEFAULT_XML_VERSION;

    DOMDocument *document = CreateEmptyDOMDocument();
    document->setXmlVersion(StringToXmlCh(_version).Get());
    document->setXmlStandalone(standalone);

    std::shared_ptr<XmlDocument> xmlDocument =
        std::make_shared<XmlDocument>(
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
    std::shared_ptr<XmlElement> parent,
    std::shared_ptr<XmlElement> child,
    std::shared_ptr<XmlElement> insertBefore
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
    std::shared_ptr<XmlElement> element,
    const XmlAttributes &attributes
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
    std::shared_ptr<XmlElement> element,
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

        throw std::exception();
    }
    catch (...)
    {
        // TODO: Print Error
    }
}

void XercesAdapter::SetXmlElementText(
    std::shared_ptr<XmlElement> element,
    const std::string &text
)
{
    auto _text =
        _dummyDoc->createTextNode(StringToXmlCh(text).Get());

    element->SetText(_text);

    _text = nullptr; // Don't delete this

    element->NotifyDeepValueChanged();
}

std::shared_ptr<XmlElement> XercesAdapter::GetXmlRootElement(
    const std::shared_ptr<XmlDocument> xmlDocument
) const
{
    return xmlDocument->GetRootElement();
}

std::string XercesAdapter::GetXmlElementName(
    const std::shared_ptr<XmlElement> xmlElement
) const
{
    return xmlElement->GetTagName();
}

std::shared_ptr<XmlElement> XercesAdapter::CopyXmlElement(
    const std::shared_ptr<XmlElement> original
)
{
    return original->CopyElement();
}

std::list<std::shared_ptr<XmlElement>> XercesAdapter::GetXmlElementChildElements(
    const std::shared_ptr<XmlElement> element,
    const std::string &elementName
) const
{
    // TODO: Implement
    return std::list<std::shared_ptr<XmlElement>>();
}

std::string XercesAdapter::GetXmlElementText(
    const std::shared_ptr<XmlElement> element
) const
{
    // TODO: Implement
    return "";
}

XmlAttributes XercesAdapter::GetXmlElementAttributes(
    const std::shared_ptr<XmlElement> element
) const
{
    return XmlAttributes();
}

//

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

    std::shared_ptr<XmlElement> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            "Element1",
            "Hello",
            XmlAttributes()
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
            "Element1",
            "Hello",
            XmlAttributes()
        );

    std::shared_ptr<XmlElement> element2 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            "Element2",
            "Hello",
            XmlAttributes()
        );

    std::shared_ptr<XmlElement> element3 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            "Element3",
            "Hello",
            XmlAttributes()
        );

    XercesAdapter::GetInstance()
        .AddXmlChildElement(element2, element1);

    try
    {
        XercesAdapter::GetInstance()
            .AddXmlChildElement(element2, element1);
    }
    catch (const std::exception&)
    {
        //
    }

    XercesAdapter::GetInstance()
        .AddXmlChildElement(element1, element3, element2);

    //element1.reset();

    std::cout << element1->ToString() << std::endl;
}

void TestCase3()
{
    // Test Document
    std::cout << "Test Case 3" << std::endl;

    std::shared_ptr<XmlElement> element1 =
        XercesAdapter::GetInstance()
        .CreateXmlElement(
            "Element1",
            "Hello",
            XmlAttributes()
        );

    std::shared_ptr<XmlDocument> document1 =
        XercesAdapter::GetInstance()
        .CreateXmlDocument(
            element1,
            "UTF-8",
            "1.0",
            false
        );

    std::cout << document1->ToString() << std::endl;
}
