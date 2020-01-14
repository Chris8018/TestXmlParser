#include "TestXerces.h"

// Mandatory for using any feature of Xerces.
#include <xercesc/util/PlatformUtils.hpp>

// DOM (if you want SAX, then that's a different include)
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>

#include <xercesc/util/XMLString.hpp>

//#include <xercesc/util/OutOfMemoryException.hpp>

// Define namespace symbols (Otherwise we'd have to prefix Xerces code with 
// "XERCES_CPP_NAMESPACE::")
XERCES_CPP_NAMESPACE_USE

#include <locale>

#include <codecvt>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

std::string convertUTF16_UTF8(XMLCh *str);

int main(void) {

    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(
            XMLString::transcode("Core"));;

    DOMDocument *domDoc1 =
        domImpl->createDocument(0, XMLString::transcode("Hello_World"), 0);

    DOMElement *domEle1 = domDoc1->createElement(XMLString::transcode("child1"));

    domDoc1->getFirstChild()->appendChild(domEle1);

    DOMLSSerializer *theSerializer = domImpl->createLSSerializer();
    XMLCh* str = theSerializer->writeToString(domDoc1);

    std::cout << convertUTF16_UTF8(str) << std::endl;

    // Cleanup.
    domDoc1->release();
    XMLPlatformUtils::Terminate();


    return 0;
}


std::string convertUTF16_UTF8(XMLCh *str)
{
    std::u16string str_16 = str;

    std::wstring wide_string(str_16.begin(), str_16.end());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    std::string text = convert.to_bytes(wide_string);

    return text;
}
