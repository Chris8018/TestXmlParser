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

#include <memory>

void CreateAndPrint();
void CreateAndPrintSmartPointer();
std::string convertUTF16_UTF8(XMLCh *str);

bool toFile = false;

class DOMPrintErrorHandler : public DOMErrorHandler
{
public:

    DOMPrintErrorHandler() {};
    ~DOMPrintErrorHandler() {};

    /** @name The error handler interface */
    bool handleError(const DOMError &domError) override;
    void resetErrors() {};

private:
    /* Unimplemented constructors and operators */
    //DOMPrintErrorHandler(const DOMErrorHandler &);
    //void operator=(const DOMErrorHandler &);

};

int main(void)
{
    //toFile = true;

    CreateAndPrint();

    //CreateAndPrintSmartPointer();

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
    XMLCh features[3] = { (XMLCh)76U, (XMLCh)83U, (XMLCh)0U };
    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(u"");

    // DOMDoc2
    DOMDocument *domDoc2 =
        domImpl->createDocument(0, 0, 0);

    // Set Version
    domDoc2->setXmlVersion(u"1.0");

    // Set Stand Alone
    domDoc2->setXmlStandalone(u"yes");

    // P.I.
    //DOMProcessingInstruction *pi1 = domDoc2->createProcessingInstruction(u"xml", u"version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\"");
    //domDoc2->appendChild(pi1);

    // Dom Ele2 - with namespace
    DOMElement *domEle2 = nullptr;
    try
    {
        domEle2 = domDoc2->createElement(XMLString::transcode("TEST ELEMENT"));
    }
    catch (const DOMException &e)
    {
        domEle2 = domDoc2->createElement(XMLString::transcode("TEST_ELEMENT"));
        //DOMDocumentImpl::
    }

    domDoc2->appendChild(domEle2);
    domEle2->setTextContent(u"Ele2 Text");

    // DOMDoc1
    DOMDocument *domDoc1 =
        domImpl->createDocument(0, XMLString::transcode("Hello_World"), 0);

    // Append Child
    DOMElement *domEle1 = domDoc1->createElement(XMLString::transcode("child1"));

    domDoc1->getFirstChild()->appendChild(domEle1);

    //domEle1->attribute

    // DOMElement 5
    DOMElement *domEle5 = domDoc1->createElement(u"ELEMENT5");

    domEle1->appendChild(domEle5);

    // DOMElement 5
    DOMElement *domEle6 = domDoc1->createElement(u"ELEMENT6");

    domEle5->appendChild(domEle6);

    // Try adding random element
    DOMNode *movedEle2 = domDoc1->importNode(domEle2, true);
    domEle1->appendChild(movedEle2);

    // Try changing movedEle2
    // Changing domEle2 does not make any different
    //dynamic_cast<DOMElement *>(movedEle2)->setAttribute(XMLString::transcode("sad:inde"), XMLString::transcode("a value in attr with namespace"));
    domEle2->setAttribute(XMLString::transcode("sad:inde"), XMLString::transcode("value"));

    //domEle2->setAttribute(u" ", u"value");

    domEle2->setAttribute(u"key", u" ");

    domEle2->setTextContent(XMLString::transcode("~!@#$%^&*()_+{}|:\"<>?`1234567890-=[]\\;',./"));

    // DOMElement 3
    DOMElement *domEle3 = domDoc2->createElement(u"rnd:Child3");

    domEle2->appendChild(domEle3);

    //domEle2->setTextContent(XMLString::transcode("LONGGGGGG Text"));

    // DOMElement 4 - Insert Before (1 node above the new Element)
    DOMElement *domEle4 = domDoc1->createElement(u"ELEMENT4");

    domEle5->insertBefore(domEle4, domEle6);

    //// DOMLSOutput-----------------------------------------
    DOMLSOutput *theOutPut = domImpl->createLSOutput();
    theOutPut->setEncoding(XMLString::transcode("UTF-8"));
    ////-----------------------------------------------------

    // Check encoding type
    auto check = theOutPut->getEncoding();

    //// DOMLSSerializer-------------------------------------
    DOMLSSerializer *theSerializer = domImpl->createLSSerializer();
    ////-----------------------------------------------------

    //// Error Handler---------------------------------------
    DOMErrorHandler *myErrorHandler = new DOMPrintErrorHandler();
    ////-----------------------------------------------------

    //// Configure-------------------------------------------
    DOMConfiguration *serializerConfig = theSerializer->getDomConfig();
    // Set Error Handler
    serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);
    // Set Pretty Print
    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    ////-----------------------------------------------------

    //// Format Target---------------------------------------
    XMLFormatTarget *myFormTarget;

    if (toFile)
        myFormTarget = new LocalFileFormatTarget(paths[0].c_str());
    else
        myFormTarget = new StdOutFormatTarget();
    ////-----------------------------------------------------

    ////-----------------------------------------------------
    theOutPut->setByteStream(myFormTarget);
    ////-----------------------------------------------------

    // Print on Console
    std::cout << "Dom Doc 1" << std::endl;
    theSerializer->write(domDoc1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Dom Doc 2" << std::endl;
    theSerializer->write(domDoc2, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Dom Elem 1" << std::endl;
    theSerializer->write(domEle1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    //DOMDocument *doc = 0;

    //// SHOULD BE GONE when delete domDoc1
    DOMElement *dummyElement1 = domDoc1->createElement(u"Dummy1");

    // Cleanup.
    //theOutPut->release();
    theSerializer->release();
    theOutPut->release();

    auto temp = domEle5->removeChild(domEle6);
    temp->release();

    domDoc1->release();
    //domEle1->node

    //domDoc1->release();
    //delete domDoc1;
    domDoc2->release();
    XMLPlatformUtils::Terminate();
}

// FAILED BADLY
void CreateAndPrintSmartPointer()
{
    std::cout << "CreateAndPrintOnConsole With Smart Pointer" << std::endl;

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
    XMLCh features[3] = { (XMLCh)76U, (XMLCh)83U, (XMLCh)0U };
    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(u"");
    //std::shared_ptr<DOMImplementation> domImpl(DOMImplementationRegistry::getDOMImplementation(u""));
    //std::shared_ptr<DOMImplementation> domImpl(DOMImplementationRegistry::getDOMImplementation(u""));

    // DOMDoc2
    std::shared_ptr<DOMDocument> domDoc2(domImpl->createDocument(0, 0, 0));

    // Set Version
    domDoc2->setXmlVersion(u"1.0");

    // Set Stand Alone
    domDoc2->setXmlStandalone(u"yes");

    // P.I.
    //DOMProcessingInstruction *pi1 = domDoc2->createProcessingInstruction(u"xml", u"version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\"");
    //domDoc2->appendChild(pi1);

    // Dom Ele2 - with namespace
    std::shared_ptr<DOMElement> domEle2(domDoc2->createElement(XMLString::transcode("namespace:randomChild1")));

    domDoc2->appendChild(domEle2.get());
    domEle2->setTextContent(u"Ele2 Text");

    // DOMDoc1
    std::shared_ptr<DOMDocument> domDoc1(domImpl->createDocument(0, XMLString::transcode("Hello_World"), 0));


    // Append Child
    std::shared_ptr<DOMElement> domEle1(domDoc1->createElement(XMLString::transcode("child1")));

    domDoc1->getFirstChild()->appendChild(domEle1.get());

    // Try adding random element
    DOMNode *movedEle2 = domDoc1->importNode(domEle2.get(), true);
    domEle1->appendChild(movedEle2);

    // Try changing movedEle2
    // Changing domEle2 does not make any different
    //dynamic_cast<DOMElement *>(movedEle2)->setAttribute(XMLString::transcode("sad:inde"), XMLString::transcode("a value in attr with namespace"));
    domEle2->setAttribute(XMLString::transcode("sad:inde"), XMLString::transcode("value"));

    // DOMElement 3
    std::shared_ptr<DOMElement> domEle3(domDoc2->createElement(u"rnd:Child3"));

    domEle2->appendChild(domEle3.get());

    // DOMLSOutput
    DOMLSOutput *theOutPut = domImpl->createLSOutput();
    theOutPut->setEncoding(XMLString::transcode("UTF-8"));

    // Check encoding type
    auto check = theOutPut->getEncoding();

    // DOMLSSerializer
    DOMLSSerializer *theSerializer = domImpl->createLSSerializer();

    // Error Handler
    DOMErrorHandler *myErrorHandler = new DOMPrintErrorHandler();

    // Configure
    DOMConfiguration *serializerConfig = theSerializer->getDomConfig();
    // Set Error Handler
    serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);
    // Set Pretty Print
    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    //XMLUni::ver

    // Format Target
    XMLFormatTarget *myFormTarget;

    if (toFile)
        myFormTarget = new LocalFileFormatTarget(paths[0].c_str());
    else
        myFormTarget = new StdOutFormatTarget();

    theOutPut->setByteStream(myFormTarget);

    // Print on Console
    std::cout << "Dom Doc 1" << std::endl;
    theSerializer->write(domDoc1.get(), theOutPut);

    // NEW LINE
    //std::cout << "\n" << std::endl;

    // Print on Console
    std::cout << "Dom Doc 2" << std::endl;
    theSerializer->write(domDoc2.get(), theOutPut);

    // Print on Console
    std::cout << "Dom Elem 1" << std::endl;
    theSerializer->write(domEle1.get(), theOutPut);

    // NEW LINE
    std::cout << "\n" << std::endl;

    // ToString
    /*XMLCh* str = theSerializer->writeToString(domDoc1);

    std::cout << convertUTF16_UTF8(str) << std::endl;*/

    // Cleanup.
    /*theOutPut->release();
    theSerializer->release();
    domDoc1->release();*/
    XMLPlatformUtils::Terminate();
    //domImpl.reset();
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
