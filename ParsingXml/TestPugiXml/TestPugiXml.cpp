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
void TestMovingElement1ToNewDocument();
void TestLosingDocumentNode();


int main()
{
	std::cout << "Test Pugi Xml" << std::endl;
	std::string path = "invalidXml3.xml";

	//TestMemoryLeak_LoadFile(path);
	//PrintNode(ParseFile(path));
	//XmlDocument a;
	//TestMovingElement1ToNewDocument();



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
	static const unsigned int parseFlags =
		parse_cdata |
		parse_comments |
		parse_declaration |
		parse_pi |
		parse_escapes |
		parse_eol |
		parse_trim_pcdata |
		parse_ws_pcdata;

	xml_document doc;
	xml_parse_result result = doc.load_file(path.c_str(), parseFlags);

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

// Only allow copy - similar to XercesC
void TestMovingElement1ToNewDocument()
{
	xml_document doc1;
	auto child1 = doc1.append_child("child1");
	auto child2 = child1.append_child("child2");

	xml_document doc2;
	auto child1_moved = doc2.append_move(child1); // FAIL
	auto child1_copy = doc2.append_copy(child1);

	std::cout << "Memory of 'child1': " << &child1 << std::endl;
	std::cout << "Memory of 'child1_moved': " << &child1_moved << std::endl;
	std::cout << "Memory of 'child1_copy': " << &child1_copy << std::endl;

	std::cout << "Memory of 'child1' 's 1st child: " << &child1.first_child() << std::endl;
	std::cout << "Memory of 'child1_moved' 's 1st child: " << &child1_moved.first_child() << std::endl;
	std::cout << "Memory of 'child1_copy' 's 1st child: " << &child1_copy.first_child() << std::endl;

	std::cout << "Memory of 'child1' 's root: " << &child1.root() << std::endl;
	std::cout << "Memory of 'child1_moved' 's root: " << &child1_moved.root() << std::endl;
	std::cout << "Memory of 'child1_copy' 's root: " << &child1_copy.root() << std::endl;

	child1.remove_child(child2);

	PrintNode(doc1);
	PrintNode(doc2);

	PrintNode(child1);
	PrintNode(child1_moved);
	PrintNode(child1_copy);

	PrintNode(child2);
}

// Losing Doc will delete everything its own => HENCE THIS WILL BREAK.
void TestLosingDocumentNode()
{
	xml_node child1;

	{
		xml_document doc;
		child1 = doc.append_child("child1");
		xml_node pdata1 = child1.append_child(xml_node_type::node_pcdata);
		pdata1.set_value("hello");

		doc.remove_child(child1);

		PrintNode(child1);
	}

	PrintNode(child1);
}
