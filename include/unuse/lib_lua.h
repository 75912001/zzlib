/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	lua
	brief:		ok
*********************************************************************/

/************************************************************************/
/* luaopen_base(L);  // ���������⣬print��tonumber��
luaopen_table(L);  // ����table�ĺ�����
luaopen_io(L);  // �����ļ��ĺ�����
luaopen_string(L); // �ַ�������
luaopen_math(L);  // ��������
luaopen_debug(L); // �ṩ����Lua����Ľӿ�                                                                     */
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
		//�ر�lua
		lua_close(this->plua_state);
	}
	bool init(){
		//��ʼ��lua
		this->plua_state = lua_open();
		//��ʼ��Luabind
		//luabind::open(this->plua_state);
		// ʹ��Luabindע�ắ�����࣬װ�غ�����Lua�ű�
		// ...
		//����lua������
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