// ParsingXml.cpp : Defines the entry point for the application.
//

#include "TestTinyXML2.h"
#include <tinyxml2.h>
using namespace tinyxml2;

#include <iostream>
#include <vector>
#include <string>

#include <memory>

#include <typeinfo>

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

void createDoc();
void loadingDoc();

std::string XmlNodeToString(const XMLNode &);

int main()
{
    createDoc();

    loadingDoc();

    return 0;
}

void createDoc()
{
    std::cout << "Create Doc" << std::endl;

    //XMLDocument::

    std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

    // DOC
    XMLDocument xmlDoc;

    //xmlDoc.Parse(header.c_str());

    // DECLARATION NODE
    XMLDeclaration *xmlDecl = xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");

    // ROOT
    XMLNode *pRoot = xmlDoc.NewElement("Root");

    xmlDoc.InsertFirstChild(pRoot);
    //xmlDoc.InsertEndChild(pRoot);

    // INSERT ELEMENT
    XMLElement *pElement = xmlDoc.NewElement("IntValue");
    pElement->SetText(10);

    pRoot->InsertEndChild(pElement);

    XMLText *test = pElement->ToText();
    // FLOAT
    pElement = xmlDoc.NewElement("FloatValue");
    pElement->SetText(0.5f);

    // Attribute
    pElement = xmlDoc.NewElement("Date");
    pElement->SetAttribute("day", 26);
    pElement->SetAttribute("month", "April");
    pElement->SetAttribute("year", 2014);
    pElement->SetAttribute("dateFormat", "26/04/2014");

    pRoot->InsertEndChild(pElement);

    // LIST
    pElement = xmlDoc.NewElement("eg:List");

    std::vector<int> vecList = { 1, 2, 3 };
    for (const auto &item : vecList)
    {
        XMLElement *pListElement = xmlDoc.NewElement("Item");
        pListElement->SetText(item);

        pElement->InsertEndChild(pListElement);

    }

    pElement->SetAttribute("abc:itemCount", vecList.size());
    pElement->SetAttribute("itemType", typeid(int).name());

    pRoot->InsertEndChild(pElement);

    // Get Attribute(s)
    std::cout << "Try grab attibutes" << std::endl;
    pElement->FirstAttribute()->Next();

    // Convert to String
    std::cout << "Try convert Doc/Element to String" << std::endl;

    std::cout << "Doc" << std::endl;
    std::cout << XmlNodeToString(xmlDoc) << std::endl;

    std::cout << "Ele" << std::endl;
    std::cout << XmlNodeToString(*pElement) << std::endl;

    std::cout << "Decl" << std::endl;
    std::cout << XmlNodeToString(*xmlDecl) << std::endl;

    // Print to Console
    //xmlDoc.Print();

    // Save
    XMLError eResult = xmlDoc.SaveFile("D:\\Workspace\\XmlStorage\\SavedData_xmlTest_Tiny.xml");

    if (eResult != XML_SUCCESS)
        printf("Error: %i\n", eResult);
}

void loadingDoc()
{
    std::cout << "Loading Doc" << std::endl;

    std::vector<std::string> paths = { "D:\\Workspace\\XmlStorage\\SavedData.xml" };
    paths.push_back("D:\\Workspace\\XmlStorage\\BigXmlFile.xml");
    paths.push_back("D:\\Workspace\\XmlStorage\\SmallXmlFile.xml");
    paths.push_back("D:\\Workspace\\XmlStorage\\simple.xml");

    XMLDocument xmlDoc;

    XMLError eResult = xmlDoc.LoadFile(paths[3].c_str());

    if (eResult != XML_SUCCESS)
    {
        printf("Error: %i\n", eResult);
        return;
    }

    // Print to Console
    xmlDoc.Print();
}

std::string XmlNodeToString(const XMLNode &node)
{
    XMLPrinter printer;
    node.Accept(&printer);

    std::string str = printer.CStr();
    return str;
}
