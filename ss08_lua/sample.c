#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int main (void)
{

  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  if( luaL_loadfile(L, "sample.lua") || lua_pcall(L, 0, 0, 0) ) {
    printf("Failed to load sample.lua\n");
    printf("error : %s\n", lua_tostring(L, -1) );
    return 1;
  }

  lua_getglobal(L, "path");
  lua_getglobal(L, "hightofhyde");
  
  printf("path: %s\n", lua_tostring(L, -2));
  printf("hight: %d\n", lua_tointeger(L, -1));


  lua_close(L);
  return 0;
}
