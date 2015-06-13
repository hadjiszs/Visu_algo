#include <iostream>
namespace Wrapper
{
using namespace std;
using namespace luabridge;

void afficher(int value)
{
	std::cout<<value<<std::endl;
}

void addToState(lua_State* L)
{
	getGlobalNamespace(L).addFunction("show",&afficher);
}


}
