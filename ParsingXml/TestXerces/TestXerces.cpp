#include "TestXerces.h"

// Mandatory for using any feature of Xerces.
#include <xercesc/util/PlatformUtils.hpp>

// DOM (if you want SAX, then that's a different include)
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
//#include <xercesc/dom/DOMNode.hpp>

#include <xercesc/dom/impl/DOMImplementationImpl.hpp>

#include <xercesc/util/XMLString.hpp>

#include <xercesc/dom/DOMErrorHandler.hpp>

#include <xercesc/dom/DOMError.hpp>

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

//#include <xercesc/framework/StdInInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include <xercesc/util/TransService.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <xercesc/util/OutOfMemoryException.hpp>

//#include <xercesc/util/HexBin.hpp>

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
void CountingInternalNode(DOMElement *element);

void TryDomDocFragment();

void TestSimpleMemoryLeak();

void TestSomeCase();

void TestParser();

void TestXPath();

void TestTranscode();

void TestParserFromFile();

void TestParserFromString();

bool toFile = false;

class StrX
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char* fLocalForm;
};

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

class SampleFactory
{
public:
    static void MakeXmlNode(DOMElement *n)
    {
        std::cout << "Make DOMElement" << std::endl;
    }

    static void MakeXmlNode(DOMAttr *n)
    {
        std::cout << "Make DOMAttr" << std::endl;
    }

    static void MakeXmlNode(DOMText *n)
    {
        std::cout << "Make DOMText" << std::endl;
    }

    static void MakeXmlNode(DOMProcessingInstruction *n)
    {
        std::cout << "Make DOMProcessingInstruction" << std::endl;
    }

    static void MakeXmlNode(DOMComment *n)
    {
        std::cout << "Make DOMComment" << std::endl;
    }
};

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

class DOMTreeErrorReporter : public ErrorHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DOMTreeErrorReporter() :
        fSawErrors(false)
    {
    }

    ~DOMTreeErrorReporter()
    {
    }


    // -----------------------------------------------------------------------
    //  Implementation of the error handler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& toCatch);
    void error(const SAXParseException& toCatch);
    void fatalError(const SAXParseException& toCatch);
    void resetErrors();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    bool    fSawErrors;
};

inline bool DOMTreeErrorReporter::getSawErrors() const
{
    return fSawErrors;
}

void DOMTreeErrorReporter::warning(const SAXParseException &toCatch)
{
    //XERCES_STD_QUALIFIER cerr << "Warning at file \"" << StrX(toCatch.getSystemId())
    //    << "\", line " << toCatch.getLineNumber()
    //    << ", column " << toCatch.getColumnNumber()
    //    << "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;

    std::cout << "Warning: " << StrX(toCatch.getMessage()) << std::endl;
}

void DOMTreeErrorReporter::error(const SAXParseException& toCatch)
{
    fSawErrors = true;
    //XERCES_STD_QUALIFIER cerr << "Error at file \"" << StrX(toCatch.getSystemId())
    //    << "\", line " << toCatch.getLineNumber()
    //    << ", column " << toCatch.getColumnNumber()
    //    << "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;

    std::cout << "Error: " << StrX(toCatch.getMessage()) << std::endl;
}

void DOMTreeErrorReporter::fatalError(const SAXParseException& toCatch)
{
    fSawErrors = true;
    //XERCES_STD_QUALIFIER cerr << "Fatal Error at file \"" << StrX(toCatch.getSystemId())
    //    << "\", line " << toCatch.getLineNumber()
    //    << ", column " << toCatch.getColumnNumber()
    //    << "\n   Message: " << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;

    std::cout << "Fatal: " << StrX(toCatch.getMessage()) << std::endl;
}

void DOMTreeErrorReporter::resetErrors()
{
    fSawErrors = false;
}


int main(void)
{
    toFile = true;

    CreateAndPrint();

    //CreateAndPrintSmartPointer();

    //TryDomDocFragment();

    //TestSimpleMemoryLeak();

    //TestSomeCase();

    //TestXPath();

    //TestTranscode();

    //TestParserFromFile();

    return 0;
}

void TestParserFromFile()
{
    std::cout << "Parsing from File" << std::endl;

    //// File Path
    std::vector<std::string> paths;
    paths.push_back("XmlStorage/simple.xml");
    paths.push_back("XmlStorage/XmlFileWithNotExistingXsd.xml");


    //// Xml String
    std::vector<std::string> xmls;
    xmls.push_back(R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<Hello_World>
    <child1/>
</Hello_World>)");
    xmls.push_back(R"(<?xml version="1.0" encoding="utf-16le" standalone="no" ?>
<Hello_World>
    <child1/>
</Hello_World>)");
    xmls.push_back(R"(
<Hello_World>
    <child1/>
</Hello_World>
)");
    xmls.push_back("");
    xmls.push_back(R"(<?xml version="1.0" encoding="utf-16le" standalone="no" ?>
)");


    //// Choose XML file to parse
    std::string gXmlFile = paths[0];

    //// Choose XML String to parse
    std::string xmlString = xmls[1];
    bool fromMemory = true;

    std::cout << xmls[4] << std::endl;

    // Initialze
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException & e)
    {
        std::cout << XMLString::transcode(e.getMessage()) << std::endl;
        return;
    }

    // DOMImpl
    DOMImplementation* domImpl =
        DOMImplementationRegistry::getDOMImplementation(u"");

    //// DOMLSOutput-----------------------------------------
    DOMLSOutput* theOutPut = domImpl->createLSOutput();
    //theOutPut->setEncoding(XMLString::transcode("UTF-8"));
    ////-----------------------------------------------------

    //// DOMLSSerializer-------------------------------------
    DOMLSSerializer* theSerializer = domImpl->createLSSerializer();
    ////-----------------------------------------------------

    //// Error Handler---------------------------------------
    DOMErrorHandler* myErrorHandler = new DOMPrintErrorHandler();
    ////-----------------------------------------------------

    //// Configure-------------------------------------------
    DOMConfiguration* serializerConfig = theSerializer->getDomConfig();
    // Set Error Handler
    serializerConfig->setParameter(XMLUni::fgDOMErrorHandler, myErrorHandler);
    // Set Pretty Print
    if (serializerConfig->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
        serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    ////-----------------------------------------------------

    //// Format Target---------------------------------------
    XMLFormatTarget* myFormTarget = new StdOutFormatTarget();
    ////-----------------------------------------------------

    ////-----------------------------------------------------
    theOutPut->setByteStream(myFormTarget);

    //domImpl->createLSParser() // Might not work

    //// Set up Parser
    XercesDOMParser* domParser = new XercesDOMParser;

    auto gValScheme = XercesDOMParser::Val_Auto;
    bool gDoSchema = true;
    bool gSchemaFullChecking = false;
    bool gValidationConstraintFatal = false;

    bool gDoNamespaces = true;
    bool gHandleMultipleImport = true;
    bool gDoCreate = false;

    domParser->setValidationScheme(gValScheme);
    domParser->setDoSchema(gDoSchema);
    domParser->setValidationSchemaFullChecking(gSchemaFullChecking);
    domParser->setValidationConstraintFatal(gValidationConstraintFatal);

    domParser->setHandleMultipleImports(gHandleMultipleImport);

    //domParser->setExternalNoNamespaceSchemaLocation
    //domParser->setLoadSchema
    //domParser->getExternalSchemaLocation

    domParser->setDoNamespaces(gDoNamespaces);

    domParser->setCreateEntityReferenceNodes(gDoCreate);

    DOMTreeErrorReporter* errReporter = new DOMTreeErrorReporter();
    domParser->setErrorHandler(errReporter);

    /// Parsing
    bool errorsOccured = false;
    try
    {
        //// Read Xml String
        MemBufInputSource inputSrc(
            (XMLByte*)xmlString.c_str(),
            xmlString.size(),
            "from string",
            false);

        inputSrc.setEncoding(u"UTF-8");

        if (fromMemory)
            domParser->parse(inputSrc);
        else
            domParser->parse(gXmlFile.c_str());

        auto a = domParser->getExternalSchemaLocation();
        auto b = domParser->getExternalNoNamespaceSchemaLocation();

        auto d = inputSrc.getEncoding();

        int c = 0;
    }
    catch (const OutOfMemoryException&)
    {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
        errorsOccured = true;
    }
    catch (const XMLException & e)
    {
        XERCES_STD_QUALIFIER cerr << "XML error occurred during parsing\n   Message: "
            << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        errorsOccured = true;
    }
    catch (const DOMException & e)
    {
        const unsigned int maxChars = 2047;
        XMLCh errText[maxChars + 1];

        XERCES_STD_QUALIFIER cerr << "\nDOM Error during parsing: '" << gXmlFile << "'\n"
            << "DOMException code is:  " << e.code << XERCES_STD_QUALIFIER endl;

        if (DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
            XERCES_STD_QUALIFIER cerr << "Message is: " << StrX(errText) << XERCES_STD_QUALIFIER endl;

        errorsOccured = true;
    }
    catch (...)
    {
        XERCES_STD_QUALIFIER cerr << "Unknown error occurred during parsing\n " << XERCES_STD_QUALIFIER endl;
        errorsOccured = true;
    }

    // Failed
    if (errorsOccured)
        return;

    //// Check for error report
    if (errReporter->getSawErrors())
    {
        //
        std::cout << "Got Error when parsing" << std::endl;
    }

    try
    {
        DOMDocument* doc1 = domParser->adoptDocument();

        auto a = doc1->getInputEncoding();
        auto b = doc1->getXmlEncoding();

        //doc1->getDocumentElement()->pos

        auto ele1 = doc1->getDocumentElement();

        theSerializer->write(ele1, theOutPut);

        theSerializer->write(doc1, theOutPut);
    }
    catch (const OutOfMemoryException&)
    {
        XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
    }
    catch (const DOMLSException & e)
    {
        XERCES_STD_QUALIFIER cerr << "An error occurred during serialization of the DOM tree. Msg is:"
            << XERCES_STD_QUALIFIER endl
            << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
    }
    catch (const XMLException & e)
    {
        XERCES_STD_QUALIFIER cerr << "An error occurred during creation of output transcoder. Msg is:"
            << XERCES_STD_QUALIFIER endl
            << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
    }

    delete domParser;

    XMLPlatformUtils::Terminate();
}

void TestTranscode()
{
    XMLPlatformUtils::Initialize();

    std::string str = "HelloWorld";

    XMLCh *a = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "utf-8").adopt();

    XMLCh* b = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "US-ASCII").adopt();

    XMLCh* c = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "iso8859-1").adopt();

    XMLCh* d = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "utf-16").adopt();

    XMLCh* e = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "utf-16be").adopt();

    XMLCh* f = TranscodeFromStr(
        (XMLByte*)str.c_str(),
        XMLString::stringLen(str.c_str()),
        "utf-16le").adopt();

    //XMLByte *untranscoded = TranscodeToStr(
    //    a,
    //    "US-ASCII").adopt();

    //char *result = XMLString::transcode(a);

    //std::cout << result << std::endl;

    XMLPlatformUtils::Terminate();
}

void TestXPath()
{
    std::cout << "XPath testing" << std::endl;

    // Initialze
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException & e)
    {
        std::cout << XMLString::transcode(e.getMessage()) << std::endl;
        return;
    }

    // DOMImpl
    DOMImplementation *domImpl =
        DOMImplementationRegistry::getDOMImplementation(u"");

    //// DOMLSOutput-----------------------------------------
    DOMLSOutput *theOutPut = domImpl->createLSOutput();
    theOutPut->setEncoding(XMLString::transcode("UTF-8"));
    ////-----------------------------------------------------

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
    XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
    ////-----------------------------------------------------

    ////-----------------------------------------------------
    theOutPut->setByteStream(myFormTarget);
    
    ////
    auto doc1 = domImpl->createDocument();

    auto root = doc1->createElement(u"Root");
    doc1->appendChild(root);

    auto ele1 = doc1->createElement(u"A");

    ele1->appendChild(doc1->createElement(u"A1"));
    ele1->appendChild(doc1->createElement(u"A1"));

    ele1->appendChild(doc1->createElement(u"A2"));
    ele1->appendChild(doc1->createElement(u"A2"));
    ele1->appendChild(doc1->createElement(u"A2"));

    ele1->appendChild(doc1->createElement(u"A3"));
    ele1->appendChild(doc1->createElement(u"A3"));
    ele1->appendChild(doc1->createElement(u"A3"));

    ele1->appendChild(doc1->createElement(u"A4"));

    root->appendChild(ele1);

    auto ele2 = doc1->createElement(u"B");

    ele2->appendChild(doc1->createElement(u"B1"));
    ele2->appendChild(doc1->createElement(u"B1"));
    ele2->appendChild(doc1->createElement(u"B1"));

    ele2->appendChild(doc1->createElement(u"B2"));

    ele2->appendChild(doc1->createElement(u"B3"));

    ele2->appendChild(doc1->createElement(u"B4"));
    ele2->appendChild(doc1->createElement(u"B4"));
    ele2->appendChild(doc1->createElement(u"B4"));

    root->appendChild(ele2);

    auto ele3 = doc1->createElement(u"C");

    ele3->appendChild(doc1->createElement(u"A1"));
    ele3->appendChild(doc1->createElement(u"A1"));
    ele3->appendChild(doc1->createElement(u"A1"));

    ele3->appendChild(doc1->createElement(u"A2"));
    ele3->appendChild(doc1->createElement(u"A2"));
    ele3->appendChild(doc1->createElement(u"A2"));

    ele3->appendChild(doc1->createElement(u"A3"));
    ele3->appendChild(doc1->createElement(u"A3"));
    ele3->appendChild(doc1->createElement(u"A3"));

    ele3->appendChild(doc1->createElement(u"A4"));

    root->appendChild(ele3);

    auto ele4 = doc1->createElement(u"C");

    ele4->appendChild(doc1->createElement(u"G1"));
    ele4->appendChild(doc1->createElement(u"E1"));

    root->appendChild(ele4);

    auto xPathStr = u"//A2";

    try
    {
        DOMXPathNSResolver* resolver = doc1->createNSResolver(root);
        DOMXPathResult* result = doc1->evaluate(
            xPathStr,
            root,
            resolver,
            DOMXPathResult::ORDERED_NODE_SNAPSHOT_TYPE,
            NULL);

        XMLSize_t nLength = result->getSnapshotLength();
        for (XMLSize_t i = 0; i < nLength; i++)
        {
            result->snapshotItem(i);

            auto tempNode = result->getNodeValue();

            tempNode->appendChild(doc1->createElement(u"Z"));

            theSerializer->write(tempNode, theOutPut);

            //
        }

        resolver->release();
        result->release();
    }
    catch (const DOMXPathException & ex)
    {
        std::cout << "DOMXPathException" << std::endl;
        std::cout << XMLString::transcode(ex.getMessage()) << std::endl;
        return;
    }
    catch (const DOMException &ex)
    {
        std::cout << "DOMException" << std::endl;
        std::cout << XMLString::transcode(ex.getMessage()) << std::endl;
        return;
    }

    theSerializer->write(root, theOutPut);

    XMLPlatformUtils::Terminate();
}

void CreateAndPrint()
{
    std::cout << "CreateAndPrintOnConsole" << std::endl;

    std::vector<std::string> paths;
    paths.push_back("D:\\Workspace\\XmlStorage\\SavedData_xerces_utf16le.xml");

    paths.push_back("SaveTest.xml");
    paths.push_back("XmlStorage\\SaveTest.xml");
    paths.push_back("\\..\\SaveTest.xml"); // save at root

    paths.push_back("C:\\Users\\Emotive\\CMakeBuilds\\testXmlParser\\build\\bin\\XmlStorate\\SaveTest.xml");

    paths.push_back("D:\\Workspace\\XmlStorage\\SaveTest.xml");

    std::string path = paths[3];

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

    // CData
    auto cdata1 = domDoc2->createCDATASection(u"<Test CData>");

    domEle2->appendChild(cdata1);

    // Element in Element 2 with Text

    auto tempEleText1 = domDoc2->createElement(u"TextHere");
    tempEleText1->setTextContent(u"Don not show this");

    domEle2->appendChild(tempEleText1);

    // Comment into element
    auto comment1 = domDoc2->createComment(u"A Test Comment for Element2");

    domEle2->appendChild(comment1);

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
    //domEle2->setTextContent(textWithSymbols);

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

    // Test appending Element from different Element
    auto ele1 = domDoc1->createElement(u"Nintendo");

    auto ele2 = domDoc1->createElement(u"DS");
    auto ele5 = domDoc1->createElement(u"N3DS");

    ele1->appendChild(ele2);
    ele1->appendChild(ele5);

    auto ele3 = domDoc1->createElement(u"Sony");

    auto ele4 = domDoc1->createElement(u"PS2");
    auto ele6 = domDoc1->createElement(u"PS3");

    ele3->appendChild(ele4);
    ele3->appendChild(ele6);

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
        myFormTarget = new LocalFileFormatTarget(path.c_str());
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
    //theSerializer->write(domDoc2, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    //auto temp = domEle5->removeChild(domEle6);
    //temp->release();
    //domEle6->release();

    // Print on Console
    std::cout << "Dom Elem 1" << std::endl;
    //theSerializer->write(domEle1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Dom Elem 6" << std::endl;
    //theSerializer->write(domEle6, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    //DOMDocument *doc = 0;

    // Check CData
    std::cout << XMLString::transcode(domEle2->getTextContent()) << std::endl;

    std::cout << XMLString::transcode(cdata1->getData()) << std::endl;

    //// SHOULD BE GONE when delete domDoc1
    DOMElement *dummyElement1 = domDoc1->createElement(u"Dummy1");

    auto dummyDoc = domImpl->createDocument();

    auto ele7 = dummyDoc->createElement(u"Element7");

    // Continue Testing on appendElement from different parent
    // out of scope

    // Print on Console
    std::cout << "Check Append" << std::endl;
    //theSerializer->write(ele1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Check Append" << std::endl;
    //theSerializer->write(ele3, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Should be an Error
    ele5->appendChild(ele6);

    // Print on Console
    std::cout << "Check Append" << std::endl;
    //theSerializer->write(ele1, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

    // Print on Console
    std::cout << "Check Append" << std::endl;
    //theSerializer->write(ele3, theOutPut);

    std::cout << "\n";
    std::cout << "\n";
    std::cout << "\n";

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
    //DOMImplementationImpl::

    //auto iterateTime = 1;
    //for (int i = 0; i < iterateTime; i++)
    //{
    //    CreateElement(doc1);
    //}

    DOMDocument *doc2 = domImpl->createDocument();

    auto element1 = doc1->createElement(u"element1");

    auto element2 = doc1->createElement(u"element1");
    //doc2->createCDATASection(u"<sdf>")->

    if (element1->isEqualNode(element2))
    {
        std::cout << "E1 == E2" << std::endl;
    }
    else
        std::cout << "E1 != E2" << std::endl;

    doc1->release();
    doc2->release();

    XMLPlatformUtils::Terminate();
}

void CreateElement(DOMDocument *doc)
{
    DOMElement *element1 = doc->createElement(u"element1");

    element1->setTextContent(u"This is a TEST TEXT!!");

    DOMAttr *attr1 = doc->createAttribute(u"name1");
    DOMAttr *attr2 = doc->createAttribute(u"name2");
    DOMAttr *attr3 = doc->createAttribute(u"name3");
    DOMAttr *attr4 = doc->createAttribute(u"name4");
    DOMAttr *attr5 = doc->createAttribute(u"name5");

    DOMElement *element2 = doc->createElement(u"element2");
    DOMElement *element3 = doc->createElement(u"element3");
    DOMElement *element4 = doc->createElement(u"element4");

    DOMText *text1 = doc->createTextNode(u"Text1");
    DOMText *text2 = doc->createTextNode(u"Text2");
    DOMText *text3 = doc->createTextNode(u"Text3");
    DOMText *text4 = doc->createTextNode(u"Text4");
    DOMText *text5 = doc->createTextNode(u"Text5");

    DOMProcessingInstruction *pi1 = doc->createProcessingInstruction(u"nonTarget", u"a='b'");

    DOMComment *comment1 = doc->createComment(u"This is a test comment");
    DOMComment *comment2 = doc->createComment(u"This is a test comment");
    DOMComment *comment3 = doc->createComment(u"This is a test comment");
    DOMComment *comment4 = doc->createComment(u"This is a test comment");

    element1->setAttributeNode(attr1);
    element1->setAttributeNode(attr2);
    element1->setAttributeNode(attr3);
    element1->setAttributeNode(attr4);

    //element1->appendChild(attr5); -> ERROR

    element1->appendChild(element2);
    element1->appendChild(element3);
    element1->appendChild(element4);

    element1->appendChild(text1);
    element1->appendChild(text2);
    element1->appendChild(text3);
    element1->appendChild(text4);
    element1->appendChild(text5);

    element1->appendChild(pi1);

    element1->appendChild(comment1);
    element1->appendChild(comment2);
    element1->appendChild(comment3);
    element1->appendChild(comment4);

    CountingInternalNode(element1);

    //auto temp = element1->getNodeValue();
    //element1->setNodeValue(u"WHAT!!!");
    
    //std::cout << XMLString::transcode(temp) << std::endl;

    element1->release();
}

void Case1(DOMDocument *doc)
{
    //auto element1 = doc->
}

void TestSomeCase()
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

    //for (int i = 0; i < 100000; i++)
    //{
    //    CreateElement(doc1);
    //}

    doc1->release();

    XMLPlatformUtils::Terminate();
}

void TestParser()
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

    doc1->release();

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

void CountingInternalNode(DOMElement *element)
{
    auto nodeList = element->getChildNodes();

    int attributeCounter = 0,
        textCounter = 0,
        elementCounter = 0,
        PICounter = 0,
        commentCounter = 0,
        others = 0;

    for (int i = 0; i < nodeList->getLength(); i++)
    {
        //SampleFactory::MakeXmlNode(nodeList->item(i));

        if (nodeList->item(i)->getNodeType()
            == DOMNode::NodeType::TEXT_NODE)
        {
            textCounter++;
        }
        else if (nodeList->item(i)->getNodeType()
            == DOMNode::NodeType::ATTRIBUTE_NODE)
        {
            attributeCounter++;
        }
        else if (nodeList->item(i)->getNodeType()
            == DOMNode::NodeType::ELEMENT_NODE)
        {
            elementCounter++;
        }
        else if (nodeList->item(i)->getNodeType()
            == DOMNode::NodeType::COMMENT_NODE)
        {
            commentCounter++;
        }
        else if (nodeList->item(i)->getNodeType()
            == DOMNode::NodeType::PROCESSING_INSTRUCTION_NODE)
        {
            PICounter++;
        }
        else
        {
            others++;
        }
    }

    std::cout <<
        "Text: " << textCounter << "\n" <<
        "Attribute: " << attributeCounter << "\n" <<
        "Element: " << elementCounter << "\n" <<
        "Comment: " << commentCounter << "\n" <<
        "PI: " << PICounter << "\n"
        "Others: " << others << "\n";
}
