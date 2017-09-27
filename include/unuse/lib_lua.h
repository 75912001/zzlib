/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	lua
	brief:		ok
*********************************************************************/

/************************************************************************/
/* luaopen_base(L);  // 基础函数库，print、tonumber等
luaopen_table(L);  // 操作table的函数库
luaopen_io(L);  // 操作文件的函数库
luaopen_string(L); // 字符串操作
luaopen_math(L);  // 算术运算
luaopen_debug(L); // 提供调试Lua程序的接口                                                                     */
/************************************************************************/
#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
//#include <luabind/luabind.hpp>

extern "C"{
	typedef int (*LUA_FUNC_TYPE)(lua_State* lua_state);
};

class lib_lua_t
{
public:
	lib_lua_t(){}
	virtual ~lib_lua_t(){
		//关闭lua
		lua_close(this->plua_state);
	}
	bool init(){
		//初始化lua
		this->plua_state = lua_open();
		//初始化Luabind
		//luabind::open(this->plua_state);
		// 使用Luabind注册函数和类，装载和运行Lua脚本
		// ...
		//载入lua基本库
		luaL_openlibs(this->plua_state);
		return true;
	}
	bool add_func(const char* func_name, LUA_FUNC_TYPE func){
		lua_register(this->plua_state, func_name, func);
		return true;
	}
	bool run_script(const char* script){
		luaL_dofile(this->plua_state, script);
		return true;
	}
protected:
	
private:
	lua_State* plua_state;
	lib_lua_t(const lib_lua_t& cr);
	lib_lua_t& operator=(const lib_lua_t& cr);
};