#include "TestPugiXml.h"

#include "pugixml.hpp"

#include <iostream>

int main()
{
	std::cout << "Test Pugi Xml" << std::endl;
	pugi::xml_document doc;


	//pugi::xml_node node1 = doc.append_child("root");
	pugi::xml_node node1;
	pugi::xml_node node2 = node1.append_child("child");

	doc.append_child("root2");
	//doc.append_child()
	node1.set_name("aaaa");
	std::cout << node1.name() << std::endl;

	return 0;
}