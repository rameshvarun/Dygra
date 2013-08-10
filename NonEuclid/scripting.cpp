#include "scripting.h"

#include "dependencies.h"

#include "level.h"

using namespace luabind;

lua_State* scripting::newState()
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	open(L);

	module(L)
    [
        def("log", &scripting::log),

		class_<Level>("Level")
			.def(constructor<>())
			.def("LoadXML", &Level::LoadXML)
			.def("BuildShader", &Level::BuildShader)
    ];

	return L;
}

void scripting::log(std::string level, std::string message)
{
	if( level.compare("trace") == 0 )
		BOOST_LOG_TRIVIAL(trace) << message;

	if( level.compare("debug") == 0 )
		BOOST_LOG_TRIVIAL(debug) << message;

	if( level.compare("error") == 0 )
		BOOST_LOG_TRIVIAL(error) << message;
}