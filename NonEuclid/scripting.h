#ifndef SCRIPTING_H
#define SCRIPTING_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

namespace scripting
{
	lua_State* newState();
	void log(std::string level, std::string message);
}

#endif