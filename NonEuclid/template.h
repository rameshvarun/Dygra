#ifndef TEMPLATE_H
#define TEMPLATE_H


#include "dependencies.h"
#include "util.h"

#include <map>
#include <list>


namespace templating
{


		class Node
		{
		public:
			Node();

			const char* type;
			std::map<std::string, Node*> properties;

			virtual std::string toString();

			Node* get( std::string name );

			void set( std::string name, const char* value );
		};

		class StringValue : public Node
		{
		public:
			StringValue(const char* stringval);

			virtual std::string toString();

			std::string value;
		};

		class NodeList : public Node
		{
		public:
			NodeList();

			std::list<Node*> nodes;
		};

		std::string render(const char* filename, Node* rootnode);

		std::string expand( std::string code, Node* rootnode);
}

#endif