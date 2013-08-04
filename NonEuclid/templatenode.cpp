#include "template.h"

#include <boost/lexical_cast.hpp>

using namespace templating;

#pragma region Node


Node::Node()
{
	type = "node";
	properties = std::map<std::string, Node*>();
}

#pragma region Convenience Functions

void Node::set( std::string name, const char* value )
{
	properties[name] = new StringValue(value);
}

void Node::set( std::string name, bool value )
{
	properties[name] = new BoolValue(value);
}

void Node::set( std::string name, float value )
{
	properties[name] = new FloatValue(value);
}

void Node::set( std::string name, Node* value )
{
	properties[name] = value;
}

void Node::set( std::string name, Vector3f value )
{
	properties[name] = Vector3Node( value );
}

Node* templating::Vector3Node( float x, float y, float z)
{
	Node* node = new Node();
	node->set("x", x);
	node->set("y", y);
	node->set("z", z);

	return node;
}

Node* templating::Vector3Node( Vector3f vec )
{
	Node* node = new Node();
	node->set("x", vec.x );
	node->set("y", vec.y );
	node->set("z", vec.z );

	return node;
}

#pragma endregion


std::string Node::toString()
{
	return "";
}

Node* Node::get( std::string name )
{
	size_t dot_pos = name.find(".");
    if(dot_pos == std::string::npos)
	{
		if(properties.find(name) == properties.end() )
		{
			BOOST_LOG_TRIVIAL(error) << "Cout not find property " << name;

			return this;
		}

        return properties[name];
	}
	else
	{

		std::string objname = name.substr(0, dot_pos);
		std::string propname = name.substr(dot_pos + 1, std::string::npos);

		if(properties.find(objname) == properties.end() )
		{
			BOOST_LOG_TRIVIAL(error) << "Cout not find child object " << objname;

			return this;
		}

		return properties[objname]->get(propname);
	}
	
}

bool Node::exists( std::string name )
{
	size_t dot_pos = name.find(".");
    if(dot_pos == std::string::npos)
	{
		if(properties.find(name) == properties.end() )
		{
			return false;
		}

        return true;
	}
	else
	{
		std::string objname = name.substr(0, dot_pos);
		std::string propname = name.substr(dot_pos + 1, std::string::npos);

		if(properties.find(objname) == properties.end() )
		{
			return false;
		}

		return properties[objname]->exists(propname);
	}
	
}

#pragma endregion

#pragma region StringValue

StringValue::StringValue(const char* stringval) : Node()
{
	type = "string";
	value = stringval;
}

std::string StringValue::toString()
{
	return value;
}

#pragma endregion

#pragma region NodeList

NodeList::NodeList()
{
	type = "list";
	nodes = std::list<Node*>();
}

#pragma endregion


#pragma region BoolValue


BoolValue::BoolValue(bool boolval) : Node()
{
	type = "bool";
	value = boolval;
}

std::string BoolValue::toString()
{
	if(value)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

#pragma endregion

#pragma region FloatValue

FloatValue::FloatValue(float floatval) : Node()
{
	type = "float";
	value = floatval;
}

std::string FloatValue::toString()
{
	return boost::lexical_cast<std::string>(value);
}

#pragma endregion