#ifndef TEMPLATE_H
#define TEMPLATE_H


#include "dependencies.h"
#include "util.h"

#include <map>
#include <list>


namespace templating
{

		//Master node class
		class Node
		{
		public:
			Node();

			const char* type;
			std::map<std::string, Node*> properties;
			virtual std::string toString();

			Node* get( std::string name );
			bool exists( std::string name );

			//Convenience functions to set a property
			void set( std::string name, const char* value );
			void set( std::string name, bool value );
			void set( std::string name, float value );
			void set( std::string name, Node* value );
		};

		//Represents a string property
		class StringValue : public Node
		{
		public:
			StringValue(const char* stringval);
			virtual std::string toString();
			std::string value;
		};

		//Represents a boolean property
		class BoolValue : public Node
		{
		public:
			BoolValue(bool boolval);
			virtual std::string toString();
			bool value;
		};

		//Represents a float property
		class FloatValue : public Node
		{
		public:
			FloatValue( float floatval );
			virtual std::string toString();
			float value;
		};

		//Represents a list property
		class NodeList : public Node
		{
		public:
			NodeList();
			std::list<Node*> nodes;
		};


		std::string render(const char* filename, Node* rootnode);

		//Recursive function that actually expands a string containing the template
		std::string expand( std::string code, Node* rootnode);

		bool evaluate( std::string expression, Node* rootnode);
}

#endif