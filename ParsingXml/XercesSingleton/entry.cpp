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

XmlDocument::XmlDocument()
{
    //
}

XmlDocument::~XmlDocument()
{
    //
}

//------------------------------------------------------------------

// XmlElement-------------------------------------------------------

XmlElement::XmlElement(std::shared_ptr<xercesc_3_2::DOMDocument> emptyDOMDocument, std::string name)
{
    this->_parentNode = emptyDOMDocument;
    _xmlElement = 
        std::shared_ptr<xercesc::DOMElement>(
            emptyDOMDocument->createElement(
                xercesc::XMLString::transcode(name.c_str())
            )
            );
}

XmlElement::~XmlElement()
{
    //
}

bool XmlElement::IsAssigned() const
{
    return _assigned;
}

void XmlElement::SetText(const std::string &text)
{
    //std::shared_ptr<
}

void XmlElement::SetAttribute(const std::string &key, const std::string &value)
{
    //
}

void XmlElement::SetAttributes(const std::map<std::string, std::string> &attributes)
{
    //
}

void XmlElement::SetParentNode(std::shared_ptr<xercesc::DOMNode> parent)
{
    //
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

//------------------------------------------------------------------