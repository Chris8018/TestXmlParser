#include "TestXerces.h"

// Mandatory for using any feature of Xerces.
#include <xercesc/util/PlatformUtils.hpp>

// DOM (if you want SAX, then that's a different include)
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>

#include <xercesc/util/XMLString.hpp>

#include <xercesc/dom/DOMErrorHandler.hpp>

#include <xercesc/dom/DOMError.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

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

#include <vector>

void CreateAndPrint();
std::string convertUTF16_UTF8(XMLCh *str);

bool toFile = false;

class DOMPrintErrorHandler : public DOMErrorHandler
{
public:

    DOMPrintErrorHandler() {};
    ~DOMPrintErrorHandler() {};

    /** @name The error handler interface */
    bool handleError(const DOMError &domError);
    void resetErrors() {};

private:
    /* Unimplemented constructors and operators */
    DOMPrintErrorHandler(const DOMErrorHandler &);
    void operator=(const DOMErrorHandler &);

};

int main(void)
{
    toFile = true;

    CreateAndPrint();

    return 0;
}

void CreateAndPrint()
{
    std::cout << "CreateAndPrintOnConsole" << std::endl;

    std::vector<std::string> paths = { "D:\\Workspace\\XmlStorage\\SavedData_xerces_utf16le.xml" };

    // Initialze
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const std::exception & e)
    {
        std::cout << e.what() << std::endl;
        return;
    }

    // DOMImpl
    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(
            XMLString::transcode("Core"));

    // DOMDoc2
    DOMDocument *domDoc2 =
        domImpl->createDocument(0, 0, 0);

    // Dom Ele2 - with namespace
    DOMElement *domEle2 = domDoc2->createElement(XMLString::transcode("namespace:randomChild1"));

    //domDoc2->appendChild(domEle2);

    // DOMDoc1
    DOMDocument *domDoc1 =
        domImpl->createDocument(0, XMLString::transcode("Hello_World"), 0);

    // Append Child
    DOMElement *domEle1 = domDoc1->createElement(XMLString::transcode("child1"));

    domDoc1->getFirstChild()->appendChild(domEle1);

    // Try adding random element
    DOMNode *movedEle2 = domDoc1->importNode(domEle2, false);
    domEle1->appendChild(movedEle2);

    // Try changing movedEle2
    // Changing domEle2 does not make any different
    dynamic_cast<DOMElement *>(movedEle2)->setAttribute(XMLString::transcode("sad:inde"), XMLString::transcode("a value in attr with namespace"));

    // DOMLSOutput
    DOMLSOutput *theOutPut = domImpl->createLSOutput();
    theOutPut->setEncoding(XMLString::transcode("UTF-16LE"));

    auto *check = theOutPut->getEncoding();

    // DOMLSSerializer
    DOMLSSerializer *theSerializer = domImpl->createLSSerializer();

    DOMErrorHandler *myErrorHandler = new DOMPrintErrorHandler();
    DOMConfiguration *serializerConfig = theSerializer->getDomConfig();
    serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);

    XMLFormatTarget *myFormTarget;

    if (toFile)
        myFormTarget = new LocalFileFormatTarget(paths[0].c_str());
    else
        myFormTarget = new StdOutFormatTarget();

    theOutPut->setByteStream(myFormTarget);

    // Print on Console
    theSerializer->write(domDoc1, theOutPut);

    // Print on Console
    //theSerializer->write(domDoc2, theOutPut);

    // ToString
    /*XMLCh* str = theSerializer->writeToString(domDoc1);

    std::cout << convertUTF16_UTF8(str) << std::endl;*/

    // Cleanup.
    theOutPut->release();
    theSerializer->release();
    domDoc1->release();
    XMLPlatformUtils::Terminate();
}


std::string convertUTF16_UTF8(XMLCh *str)
{
    std::u16string str_16 = str;

    std::wstring wide_string(str_16.begin(), str_16.end());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    std::string text = convert.to_bytes(wide_string);

    return text;
}

bool DOMPrintErrorHandler::handleError(const DOMError &domError)
{
    // Display whatever error message passed from the serializer
    if (domError.getSeverity() == DOMError::DOM_SEVERITY_WARNING)
        XERCES_STD_QUALIFIER cerr << "\nWarning Message: ";
    else if (domError.getSeverity() == DOMError::DOM_SEVERITY_ERROR)
        XERCES_STD_QUALIFIER cerr << "\nError Message: ";
    else
        XERCES_STD_QUALIFIER cerr << "\nFatal Message: ";

    char *msg = XMLString::transcode(domError.getMessage());
    XERCES_STD_QUALIFIER cerr << msg << XERCES_STD_QUALIFIER endl;
    XMLString::release(&msg);

    // Instructs the serializer to continue serialization if possible.
    return true;
}
