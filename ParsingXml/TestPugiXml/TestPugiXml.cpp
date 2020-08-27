#include "TestPugiXml.h"

#include <sstream>
#include <iostream>
#include <string>

using namespace pugi;

XmlDocument::XmlDocument() :
	_internalDocument()
{}

void PrintNodeType(xml_node node);
void PrintNode(xml_node node);
xml_document ParseFile(const std::string& path);
void TestMemoryLeak_LoadFile(const std::string& path);

int main()
{
	std::cout << "Test Pugi Xml" << std::endl;
	std::string path = "invalidXml1.xml";

	//TestMemoryLeak_LoadFile(path);
	PrintNode(ParseFile(path));

	//xml_document doc1;

	//xml_node node1 = doc1.append_child("roo t");

	//PrintNodeType(node1.root());
	//PrintNode(node1);
	//PrintNode(doc1);

	//xml_document doc2;
	//doc2.append_move(node1);

	//PrintNodeType(node1.root());
	//PrintNode(node1);
	//PrintNode(doc1);

	//node2.append_child(node1);

	//pugi::xml_node node1;
	//pugi::xml_node node2 = node1.append_child("child");

	//doc.append_child("root2");
	//doc.append_child()
	//node1.set_name("aaaa");
	//std::cout << node1.name() << std::endl;

	return 0;
}

void PrintNodeType(xml_node node)
{
	switch (node.type())
	{
	case pugi::node_null:
		std::cout << "empty node" << std::endl;
		break;
	case pugi::node_document:
		std::cout << "document node" << std::endl;
		break;
	case pugi::node_element:
		std::cout << "element node" << std::endl;
		break;
	case pugi::node_pcdata:
		std::cout << "text (plain cdata?) node" << std::endl;
		break;
	case pugi::node_cdata:
		std::cout << "cdata node" << std::endl;
		break;
	case pugi::node_comment:
		std::cout << "comment node" << std::endl;
		break;
	case pugi::node_pi:
		std::cout << "PI node" << std::endl;
		break;
	case pugi::node_declaration:
		std::cout << "declaration node" << std::endl;
		break;
	case pugi::node_doctype:
		std::cout << "doctype node" << std::endl;
		break;
	default:
		std::cout << "unknown node" << std::endl;
		break;
	}
}

void PrintNode(xml_node node)
{
	std::ostringstream osstream;
	node.print(osstream);

	std::cout << osstream.str() << std::endl;
}

xml_document ParseFile(const std::string& path)
{
	//unsigned int parseFlags = parse_cdata | parse_comments | parse_declaration | parse_pi | parse_escapes | parse_eol | parse_trim_pcdata |
	xml_document doc;
	xml_parse_result result = doc.load_file(path.c_str());

	std::cout << "Result: " << result.description() << std::endl;

	return doc;
}

void TestMemoryLeak_LoadFile(const std::string& path)
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Iteration " << i << std::endl;
		ParseFile(path);
	}
}
