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

void TryDomDocFragment();

void TestSimpleMemoryLeak();

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

    //CreateAndPrint();

    //CreateAndPrintSmartPointer();

    //TryDomDocFragment();

    TestSimpleMemoryLeak();

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
    catch (const XMLException &e)
    {
        std::cout << XMLString::transcode(e.getMessage()) << std::endl;
        return;
    }

    // DOMImpl
    //XMLCh features[3] = { (XMLCh)76U, (XMLCh)83U, (XMLCh)0U };
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
    domEle2->setAttribute(u"attr2", u"2");
    domEle2->setAttribute(u"attr3", u"3");
    domEle2->setAttribute(u"attr4", u"4");
    //domEle2->remov


    XMLCh *textWithSymbols = XMLString::transcode("~!@#$%^&*()_+{}|:\"<>?`1234567890-=[]\\;',./");
    domEle2->setTextContent(textWithSymbols);
    //((DOMAttr*) domEle1->getAttributes()->item(1))->getna
    //domEle1->setAttribute(u" ", u" ");

    // DOMElement 3
    DOMElement *domEle3 = domDoc2->createElement(u"rnd:Child3");

    domEle2->appendChild(domEle3);

    // DOMElement 4 - Insert Before (1 node above the new Element)
    DOMElement *domEle4 = domDoc1->createElement(u"ELEMENT4");

    domEle5->insertBefore(domEle4, domEle6);

    //domEle5->appendChild(domEle4);

    // DOMElement 7
    DOMElement *domEle7 = domDoc1->createElement(u"ELEMENT7");

    domEle5->insertBefore(domEle7, nullptr);

    // DOMElement 8
    DOMElement *domEle8 = domDoc1->createElement(u"ELEMENT8");

    domDoc1->getDocumentElement()->insertBefore(domEle8, nullptr);

    // DOMElement 9
    DOMElement *domEle9 = domDoc1->createElement(u"ELEMENT9");

    try
    {
        domEle5->insertBefore(domEle9, domEle8);
    }
    catch (const DOMException &e)
    {
        std::cout << XMLString::transcode(e.getMessage()) << std::endl;
    }
    //domEle5->appendChild(domEle8);

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

    //auto temp = domEle5->removeChild(domEle6);
    //temp->release();
    //domEle6->release();

    // Print on Console
    std::cout << "Dom Elem 1" << std::endl;
    theSerializer->write(domEle1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Dom Elem 6" << std::endl;
    theSerializer->write(domEle6, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    //DOMDocument *doc = 0;

    //// SHOULD BE GONE when delete domDoc1
    DOMElement *dummyElement1 = domDoc1->createElement(u"Dummy1");

    auto dummyDoc = domImpl->createDocument();

    auto ele7 = dummyDoc->createElement(u"Element7");

    // Cleanup.
    //theOutPut->release();
    theSerializer->release();
    theOutPut->release();

    //auto temp = domEle5->removeChild(domEle6);
    //temp->release();

    //try
    //{
    //    auto testRemove = (DOMElement*)domEle5->removeChild(domEle6);
    //    //domEle6->release();
    //    testRemove->release();
    //}
    //catch (const std::exception &e)
    //{
    //    std::cout << e.what() << std::endl;
    //}
    //catch (const DOMException &e)
    //{
    //    std::cout << XMLString::transcode(e.getMessage()) << std::endl;
    //}

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

void TryDomDocFragment()
{
    std::cout << "Try Dom Doc Fragment" << std::endl;

    std::vector<std::string> paths = { "D:\\Workspace\\XmlStorage\\SavedData_xerces_NonDocument.xml" };

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

    DOMDocument *doc1 = domImpl->createDocument();

    //auto domFrag1 = doc1->createDocumentFragment();

    //doc1->eva
    //domFrag1->set
    //auto ele1 = doc1->createElement(u"Element1");

    //domFrag1->appendChild(ele1);


    //auto domFrag2 = doc1->createDocumentFragment();

    //auto ele2 = doc1->createElement(u"Element2");

    //domFrag2->appendChild(ele2);

    //ele1->appendChild(domFrag2);

    //doc1->appendChild(domFrag1);

    auto ele1 = doc1->createElement(u"Element1");

    doc1->appendChild(ele1);


    auto ele2 = doc1->createElement(u"Element2");

    ele1->appendChild(ele2);


    auto test1 = ele1->getParentNode();

    auto ele3 = doc1->createElement(u"Element3");

    auto test2 = ele3->getParentNode();

    /// Both null check work
    /// if (test2) will also work
    //doc1->evaluate()
    if (test2 == NULL)
    {
        std::cout << "" << std::endl;
    }

    if (test2 == nullptr)
    {
        std::cout << "" << std::endl;
    }

    auto test3 = doc1->getParentNode();

    auto test4 = ele2->getParentNode();

    auto test5 = static_cast<DOMElement *>(test1);

    auto test6 = static_cast<DOMDocument *>(test1);

    //if ((DOMDocument*)test5 == doc1)
    //{
    //    int a = 1;
    //}

    //if (test6 == doc1)
    //{
    //    int a = 1;
    //}

    //// DOMLSOutput-----------------------------------------
    DOMLSOutput *theOutPut = domImpl->createLSOutput();
    theOutPut->setEncoding(XMLString::transcode("UTF-8"));
    ////-----------------------------------------------------

    // Check encoding type
    //auto check = theOutPut->getEncoding();

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

    //theSerializer->write(domFrag1, theOutPut);

    theSerializer->write(ele1, theOutPut);

    theSerializer->release();
    theOutPut->release();

    //domFrag1->release();

    doc1->release();

    XMLPlatformUtils::Terminate();
}

void CreateElement(DOMDocument *doc);

void TestSimpleMemoryLeak()
{
    std::cout << "Test Memory Leak" << std::endl;

    // Initialze
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException & e)
    {
        //std::cout << e.what() << std::endl;
        return;
    }

    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(u"");

    DOMDocument *doc1 = domImpl->createDocument();

    for (int i = 0; i < 1000000; i++)
    {
        CreateElement(doc1);
    }

    doc1->release();

    XMLPlatformUtils::Terminate();
}

void CreateElement(DOMDocument *doc)
{
    DOMElement *element1 = doc->createElement(u"element1");
    //doc->adoptNode()
    //doc->setNamedItem(nullptr);
    element1->release();
    //DOMNode *rem = doc->appendChild(element1);
    //rem->release();
    //element1->setAttribute(u"name1", u"value1");

    //auto attr1 = element1->getAttributeNode(u"name1");
    //element1->removeAttributeNode(attr1);

    //attr1->release();

    //doc->appendChild(ele1);
    //doc->removeChild(ele1)->release();
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
