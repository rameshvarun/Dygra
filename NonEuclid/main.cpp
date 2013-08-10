
#include "dependencies.h"


#include "level.h"

#include "template.h"
#include "scripting.h"

#include "graphics.h"


using namespace templating;

int main()
{

#ifdef _DEBUG
	BOOST_LOG_TRIVIAL(info) << "Running game in Debug mode.";
#else
	boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

	//Log to a file
	boost::log::add_file_log("sample.log");
#endif
	

	PHYSFS_init(NULL);
	PHYSFS_mount( "../content/", "", 1 );

	BOOST_LOG_TRIVIAL(debug) << "Initialized PhysFS filesystem.";

	graphics::initialize();

	BOOST_LOG_TRIVIAL(debug) << "Window created.";

	lua_State *mainscript = scripting::newState();
	luaL_dostring(mainscript, file_to_string("main.lua").c_str() );

	
}