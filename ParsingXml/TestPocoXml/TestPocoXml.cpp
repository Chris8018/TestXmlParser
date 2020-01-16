#include "TestPocoXml.h"

// Eg1
//#include <Poco/DOM/DOMParser.h>
//#include <Poco/DOM/Document.h>
//#include <Poco/DOM/NodeIterator.h>
//#include <Poco/DOM/NodeFilter.h>
//#include <Poco/DOM/AutoPtr.h>
//#include <Poco/SAX/InputSource.h>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

#include <Poco/DOM/ProcessingInstruction.h>

#include <Poco/DOM/Text.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>

#include <Poco/UTF16Encoding.h>
#include <Poco/TextEncoding.h>
using namespace Poco::XML;

#include <iostream>
#include <vector>
#include <string>

#include <memory>

#include <fstream>

#include <typeinfo>

std::vector<std::string> paths;

void basicCreateAndPrintAndEncoding();

int main()
{
    paths.push_back("D:\\Workspace\\XmlStorage\\SavedData_xmlTest_Poco.xml");
    paths.push_back("D:\\Workspace\\XmlStorage\\SavedData_xmlTest_Poco_utf16le.xml");
    paths.push_back("D:\\Workspace\\XmlStorage\\SavedData_xmlTest_Poco_utf16be.xml");


    // String 16 test
    std::u16string str = u"1";

    //std::cout << str.c_str << std::endl;

    basicCreateAndPrintAndEncoding();

    // Eg1
    /*std::ifstream in(“test.xml”);

    InputSource src(in);
    DOMParser parser;
    Poco::AutoPtr<Document> pDoc = parser.parse(&src);
    NodeIterator it(pDoc, NodeFilter::SHOW_ELEMENTS);
    Node *pNode = it.nextNode();
    while (pNode)
    {
        std::cout << pNode->nodeName() << ":" << pNode->nodeValue() << std::endl;
        pNode = it.nextNode();
    }*/

    return 0;
}

void basicCreateAndPrintAndEncoding()
{
    // Doc
    AutoPtr<Document> pDoc = new Document;

    // Processing Instruction
    AutoPtr<ProcessingInstruction> pHeader = pDoc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\"");

    pDoc->appendChild(pHeader);

    // Root Node
    AutoPtr<Element> pRoot = pDoc->createElement("avc:root");

    pDoc->appendChild(pRoot);

    // Child Node
    AutoPtr<Element> pChild1 = pDoc->createElement("child1");
    AutoPtr<Text> pText1 = pDoc->createTextNode("text1");pChild1->appendChild(pText1);
    pRoot->appendChild(pChild1);

    // Child Node
    AutoPtr<Element> pChild2 = pDoc->createElement("ccc:child2");
    AutoPtr<Text> pText2 = pDoc->createTextNode("text2");
    pChild2->appendChild(pText2);

    pChild2->setAttribute("aaa:something", "some value");

    pRoot->appendChild(pChild2);

    // Print
    DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(XMLWriter::PRETTY_PRINT);
    writer.writeNode(std::cout, pDoc);

    // Try Encoding
    std::string encodingType = "UTF-8";
    Poco::TextEncoding *encoding = &Poco::TextEncoding::byName(encodingType);

    // Handle UTF-16 BE and LE
    if (Poco::UTF16Encoding *temp = dynamic_cast<Poco::UTF16Encoding *>(encoding))
    {
        Poco::UTF16Encoding::ByteOrderType temp2 = Poco::UTF16Encoding::ByteOrderType::LITTLE_ENDIAN_BYTE_ORDER;
        temp->setByteOrder(temp2);

        DOMWriter writeToFile;
        writeToFile.setNewLine("\n");
        writeToFile.setOptions(XMLWriter::PRETTY_PRINT);
        writeToFile.setEncoding("UTF-16", *temp);
        writeToFile.writeNode(std::ofstream(paths[1]), pDoc);
        return;
    }
    writer.setEncoding(encodingType, Poco::TextEncoding::byName(encodingType));

    //writer.s

    writer.writeNode(std::cout, pDoc);

    // Write to a file
    std::ofstream str(paths[0]);

    DOMWriter writeToFile;
    writeToFile.setNewLine("\n");
    writeToFile.setOptions(XMLWriter::PRETTY_PRINT);
    writeToFile.setEncoding("UTF-16", Poco::TextEncoding::byName("UTF-16"));
    writeToFile.writeNode(str, pDoc);
}