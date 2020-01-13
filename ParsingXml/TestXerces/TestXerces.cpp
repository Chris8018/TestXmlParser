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
//#include <memory>

//#if (_MSC_VER >= 1900 /* VS 2015*/) && (_MSC_VER < 1920 /* VS 2019 */)
//std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
//#endif

int main(void) {

    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const std::exception &)
    {
        //
    }

    DOMImplementation *p_DOMImplementation = DOMImplementationRegistry::getDOMImplementation(
        XMLString::transcode("Core"));;

    // Pointer to our DOMDocument.
    DOMDocument *p_DOMDocument = p_DOMImplementation->createDocument(0, XMLString::transcode("Hello_World"), 0);
    //DOMDocument::create
    //p_DOMDocument->set

    /*
    The output of the code above will produce the following:

        <?xml version="1.0" encoding="UTF-8" standalone="no" ?>
        <Hello_World/>

    */
    DOMElement *p_domElement = p_DOMDocument->createElement(XMLString::transcode("child"));

    //p_DOMDocument->appendChild(p_domElement);
    p_DOMDocument->getFirstChild()->appendChild(p_domElement);

    DOMLSSerializer *theSerializer = p_DOMImplementation->createLSSerializer();
    XMLCh* str = theSerializer->writeToString(p_DOMDocument);
    
    std::u16string test = str;
    
    std::wstring wide_string(test.begin(), test.end());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    std::string a = convert.to_bytes(wide_string);

    //std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t > my_conv;
    //std::istringstream ss(my_conv.to_bytes((char16_t*)str).c_str());

    //std::string a = "";
    //a = ss.str();
    std::cout << a << std::endl;

    // Cleanup.
    p_DOMDocument->release();
    XMLPlatformUtils::Terminate();


    return 0;
}


//#include <xercesc/dom/DOM.hpp>

//#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/util/XMLString.hpp>
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/util/OutOfMemoryException.hpp>
//#include <xercesc/framework/XMLFormatter.hpp>
//#include <xercesc/framework/LocalFileFormatTarget.hpp>
//#include <xercesc/dom/DOMDocument.hpp>
//#include <xercesc/dom/DOMImplementation.hpp>
//#include <xercesc/dom/DOMImplementationRegistry.hpp>
//#include <xercesc/dom/DOMLSSerializer.hpp>
//#include <xercesc/dom/DOMLSOutput.hpp>

//int main()
//{
//    std::cout << "Hello CMake." << std::endl;
//    return 0;
//}
