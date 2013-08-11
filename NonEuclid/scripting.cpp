#include "scripting.h"

#include "dependencies.h"

#include "level.h"
#include "objects.h"

#include "graphics.h"

using namespace luabind;

lua_State* scripting::newState()
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	open(L);

	module(L)
    [
		//Helper functions for script
        def("log", &scripting::log),
		def("initialize", &graphics::initialize),

		//Level class
		class_<Level>("Level")
			.def(constructor<std::string>())
			.def("LoadXML", &Level::LoadXML)
			.def("BuildShader", &Level::BuildShader)
			.def("run", &Level::run)
			.def("getPos", &Level::getPos),

		//Vector3f
		class_<Vector3f>("Vector3f")
			.def(constructor<>())
			.def_readwrite("x", &Vector3f::x)
			.def_readwrite("y", &Vector3f::y)
			.def_readwrite("z", &Vector3f::z),

		//Object
		class_<Object>("Object")
			.def_readonly("type", &Object::type),

		class_<Sphere, Object>("Sphere"),

		class_<BoxAberration, Object>("BoxAberration")
			.def("isInside", &BoxAberration::isInside)
			.def("setScale", &BoxAberration::setScale)

			.def("setMin", &BoxAberration::setMin)
			.def("setMax", &BoxAberration::setMax)

			.def("getMin", &BoxAberration::getMin)
			.def("getMax", &BoxAberration::getMax)

			.def_readwrite("min", &BoxAberration::min)
			.def_readwrite("max", &BoxAberration::max)
			.def_readwrite("scale", &BoxAberration::scale)
    ];

	return L;
}

void scripting::log(std::string level, std::string message)
{
	if( level.compare("trace") == 0 )
		BOOST_LOG_TRIVIAL(trace) << message;

	if( level.compare("debug") == 0 )
		BOOST_LOG_TRIVIAL(debug) << message;

	if( level.compare("info") == 0 )
		BOOST_LOG_TRIVIAL(info) << message;

	if( level.compare("warning") == 0 )
		BOOST_LOG_TRIVIAL(warning) << message;

	if( level.compare("error") == 0 )
		BOOST_LOG_TRIVIAL(error) << message;

	if( level.compare("fatal") == 0 )
		BOOST_LOG_TRIVIAL(fatal) << message;
}