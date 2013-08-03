#include "template.h"

using namespace templating;

Node::Node()
{
	type = "node";
	properties = std::map<std::string, Node*>();
}

void Node::set( std::string name, const char* value )
{
	properties[name] = new StringValue(value);
}

std::string Node::toString()
{
	return "";
}

Node* Node::get( std::string name )
{
	return properties[name];
}

StringValue::StringValue(const char* stringval) : Node()
{
	type = "string";
	value = stringval;
}

std::string StringValue::toString()
{
	return value;
}

NodeList::NodeList()
{
	nodes = std::list<Node*>();
}