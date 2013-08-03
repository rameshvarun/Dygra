#include "template.h"

#include <boost/regex.hpp>

using namespace boost;

boost::regex tag_regex("\\{{1,2}(.*?)\\}{1,2}");

boost::regex if_regex("%\\s*if(.*)%");

boost::regex for_regex("%\\s*for\\s*(\\S*)\\s*in\\s*(\\S*)\\s*%");

std::string templating::render(const char* filename)
{
	std::string file = file_to_string(filename);

	std::string::const_iterator start, end;
	start = file.begin();
	end = file.end();

	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	while( regex_search(start, end, what, tag_regex, flags) )
	{
		std::string tag = std::string(what[1].first, what[1].second);
		BOOST_LOG_TRIVIAL(trace) << "Found tag " << tag;

		//Find out what type of tag this is
		boost::match_results<std::string::const_iterator> tagmatch;
		if( regex_match(tag, tagmatch, if_regex) )
		{
			std::string expression = std::string(tagmatch[1].first, tagmatch[1].second);

			BOOST_LOG_TRIVIAL(trace) << "If tag with expression " << expression;
		}
		else if( regex_match(tag, tagmatch, for_regex) )
		{
			std::string itemname = std::string(tagmatch[1].first, tagmatch[1].second);
			std::string loopname = std::string(tagmatch[2].first, tagmatch[2].second);

			BOOST_LOG_TRIVIAL(trace) << "For tag. Iterate through " << loopname << " with each element named " << itemname;
		}

		start = what[0].second;
	}

	return "";
}