#include "template.h"

#include <boost/regex.hpp>

#include <boost/algorithm/string/replace.hpp>

using namespace boost;
using namespace templating;

boost::regex tag_regex("\\{{1,2}(.*?)\\}{1,2}");

boost::regex if_regex("%\\s*if(.*)%");

boost::regex for_regex("%\\s*for\\s*(\\S*)\\s*in\\s*(\\S*)\\s*%");

boost::regex variable_regex("\\s*(\\S*)\\s*");




//A function that recursively expands the template
std::string templating::expand( std::string code, Node* rootnode )
{
	//Make a copy of the string
	std::string result = code;

	std::string::const_iterator start, end;
	start = code.begin();
	end = code.end();

	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	while( regex_search(start, end, what, tag_regex, flags) )
	{
		//Get the entire match for the regex
		std::string hit = std::string(what[0].first, what[0].second);

		//Only get the expression inside the tag
		std::string tag = std::string(what[1].first, what[1].second);
		

		

		//Find out what type of tag this is
		boost::match_results<std::string::const_iterator> tagmatch;
		if( regex_match(tag, tagmatch, if_regex) ) //If tag
		{
			std::string expression = std::string(tagmatch[1].first, tagmatch[1].second);

			BOOST_LOG_TRIVIAL(trace) << "If tag with expression " << expression;
		}
		else if( regex_match(tag, tagmatch, for_regex) ) //For tag
		{
			std::string itemname = std::string(tagmatch[1].first, tagmatch[1].second); //The name that each element should take
			std::string loopname = std::string(tagmatch[2].first, tagmatch[2].second); //The name of the list of elements

			BOOST_LOG_TRIVIAL(trace) << "For tag. Iterate through " << loopname << " with each element named " << itemname;
		}
		else if( regex_match(tag, tagmatch, variable_regex) ) //Standard variable tag
		{
			std::string variable = std::string(tagmatch[1].first, tagmatch[1].second); //The name of the variable
			std::string replacement = rootnode->get( variable )->toString(); //The text that the tag should be replaced with

			replace_all(result, hit, replacement);

			BOOST_LOG_TRIVIAL(trace) << "Variable tag for variable " << variable;
		}
		else //If you could't figure out what tag type it is
		{
			BOOST_LOG_TRIVIAL(trace) << "Unkown tag " << tag;
		}

		start = what[0].second;

		
	}

	return result;
}

std::string templating::render(const char* filename, Node* rootnode)
{
	//Load file into string and then expand template
	std::string file = file_to_string(filename);
	std::string result = expand(file, rootnode);
	
	//Log result
	BOOST_LOG_TRIVIAL(debug) << result;

	return result;
}