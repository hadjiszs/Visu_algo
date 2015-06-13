#include "LuaLibrary.h"
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
#include "wrapper.hpp"
#include "wrapper.cpp"
#include "graphe.hpp"
#include "graphe.cpp"
#include <iostream>

int main(void)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	LuaGraphe::addToState(L);
	int s;
	if((s = luaL_loadfile(L,"test.lua"))!=0)
	{
		std::cerr<<lua_tostring(L,-1)<<std::endl;
		lua_close(L);
		return 1;
	}
	s = lua_pcall(L,0,0,0);
	lua_close(L);	
	return 0;
}
