#include "lib_lua.h"

/*
lib_lua_t* g_lua;

lib_lua_t::lib_lua_t()
{
	//初始化 Lua	
	this->lua = luaL_newstate();

	//载入Lua基本库	
	luaL_openlibs(this->lua);
}

lib_lua_t::~lib_lua_t()
{
	lua_close(this->lua);
}

int lib_lua_t::do_file( const char* file )
{
	luaL_dofile(this->lua, file);
	return 0;
}

int lib_lua_t::register_fn( const char* fn_name, LUA_FN lua_fn )
{
	lua_register(this->lua, fn_name, lua_fn);
	return 0;
}

int lib_lua_t::load_file( const char* file )
{
	luaL_loadfile(this->lua, file);
	return 0;
}
*/