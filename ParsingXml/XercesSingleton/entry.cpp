#include "Entry.h"

#include "xercesc/util/PlatformUtils.hpp"
#include <xercesc/util/XMLString.hpp>

#include "xercesc/dom/DOM.hpp"

#include <iostream>

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
        ->setXmlVersion(xercesc::XMLString::transcode(version.c_str()));

    this->_encoding = encoding;

    this->_xmlDocument->setXmlStandalone(standalone);
}

XmlDocument::~XmlDocument()
{
    this->_xmlDocument.reset();
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(std::string name)
{
    this->_ownerDocument =
        XercesAdapter::GetInstance()
        .CreateEmptyDocument();

    //this->SetDomNodeParent(emptyDOMDocument);

    this->_xmlElement = 
        XercesAdapter::GetInstance()
        .CreateElementFromDocument(_ownerDocument, name);
}

XmlElement::~XmlElement()
{
    _xmlElement.reset();

    this->RemoveOwnerDocument();

    /*if (this->_parentNode != nullptr)
        this->_parentNode.reset();*/
}

void XmlElement::RemoveOwnerDocument()
{
    if (this->_ownerDocument != nullptr)
    {
        this->_ownerDocument.reset();

        _assigned = true;
    }
}

void XmlElement::SetXmlElement(std::shared_ptr<xercesc_3_2::DOMElement> otherElement)
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
    this->_xmlElement
        ->setTextContent(xercesc::XMLString::transcode(text.c_str()));
}

void XmlElement::SetAttribute(const std::string &name, const std::string &value)
{
    this->_xmlElement->setAttribute(
        xercesc::XMLString::transcode(name.c_str()),
        xercesc::XMLString::transcode(value.c_str()));
}

void XmlElement::SetAttributes(const std::map<std::string, std::string> &attributes)
{
    for (auto it = attributes.begin(); it != attributes.end(); it++)
    {
        this->SetAttribute(it->first, it->second);
    }
}

//void XmlElement::SetElementToDomNodeAndChangeDomNodeParent(std::shared_ptr<xercesc::DOMNode> parent)
//{
//    if (auto documentNode = std::dynamic_pointer_cast<xercesc::DOMDocument>(parent))
//    {
//        //documentNode->remo
//    }
//    else if (auto elementNode = std::dynamic_pointer_cast<xercesc::DOMElement>(parent))
//    {
//        //
//    }
//
//    this->SetDomNodeParent(parent);
//}

//void XmlElement::SetDomNodeParent(std::shared_ptr<xercesc::DOMNode> parent)
//{
//    if (_parentNode != nullptr)
//        _parentNode.reset(parent.get());
//}
//
//std::string XmlElement::ToString() const
//{
//    return "";
//}

void XmlElement::AddChildXmlElement(std::shared_ptr<XmlElement> child)
{
    //parent->_xmlElement->be
    //_ownerDocument->find
    //
    if (child->IsAssigned())
    {
        throw new std::exception("Has been added to an element.");
    }

    child->SetXmlElement(
        XercesAdapter::GetInstance()
        .CopyElementToDocumentOfElement(child->_xmlElement, this->_xmlElement, true));

    this->_xmlElement->appendChild(child->_xmlElement.get());
}

void XmlElement::InsertChildElementBeforeOtherElement(std::shared_ptr<XmlElement> child, std::shared_ptr<XmlElement> element)
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

std::shared_ptr<XmlDocument> XmlLib::CreateXmlDocument(XmlElement &rootNode, const std::string &version, const std::string &encoding, const bool &standalone)
{
    return nullptr;
}

std::shared_ptr<XmlElement> XmlLib::CreateXmlElement(const std::string &name, const std::string &text, const std::map<std::string, std::string> &attributes)
{
    return nullptr;
}

//------------------------------------------------------------------

// XercesAdapter----------------------------------------------------

XercesAdapter::XercesAdapter()
{
    try
    {
        xercesc::XMLPlatformUtils::Initialize();
    }
    catch (std::exception & e)
    {
        // TODO: Print error message
    }

    // TODO: Create Parser, Implementation
    InitializeDomImplementation();
}

XercesAdapter::~XercesAdapter()
{
    Clear();

    // TODO: considering catching error
    xercesc::XMLPlatformUtils::Terminate();
}

void XercesAdapter::Clear()
{
    _domImpl.reset();
}

void XercesAdapter::InitializeDomImplementation()
{
    if (_domImpl == nullptr)
        _domImpl = std::shared_ptr<xercesc::DOMImplementation>(xercesc::DOMImplementationRegistry::getDOMImplementation(u""));
}

XercesAdapter& XercesAdapter::GetInstance()
{
    static XercesAdapter _xercesAdapter;
    return _xercesAdapter;
}

std::shared_ptr<xercesc::DOMDocument> XercesAdapter::CreateEmptyDocument() const
{
    return std::shared_ptr<xercesc::DOMDocument>(this->_domImpl->createDocument(0, 0, 0));
}

std::shared_ptr<xercesc::DOMElement> XercesAdapter::CreateElementFromDocument(std::shared_ptr<xercesc::DOMDocument> xmlDoc, const std::string &name)
{
    return std::shared_ptr<xercesc::DOMElement>(xmlDoc->createElement(xercesc::XMLString::transcode(name.c_str())));
}

std::shared_ptr<xercesc::DOMElement> XercesAdapter::CopyElementToDocumentOfElement(
    std::shared_ptr<xercesc::DOMElement> element, 
    std::shared_ptr<xercesc::DOMElement> targetElement,
    bool deep)
{
    std::shared_ptr<xercesc::DOMElement> copiedElement((xercesc::DOMElement*)targetElement->getOwnerDocument()->importNode(element.get(), deep));
    return copiedElement;
}

//------------------------------------------------------------------
