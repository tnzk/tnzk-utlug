#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

void dump_stack( lua_State* L)
{

  static int c = 0;
  int i;
  int top = lua_gettop(L);
  printf("+--+--+-------------------------\n");
  printf("|+i|-i|                     [%i]\n", ++c);
  printf("+--+--+-------------------------\n");
  
  for( i = top; i > 0; i--){
    int t = lua_type(L,i);
    printf( "|%2i|%2i| ", i, i - top - 1);
    switch(t){
    case LUA_TSTRING:
      printf("'%s'", lua_tostring(L,i));
      break;
    case LUA_TBOOLEAN:
      printf(lua_toboolean(L,i) ? "true" : "false");
      break;
    case LUA_TNUMBER:
      printf("%g", lua_tonumber(L,i));
      break;
    default:
      printf("%s", lua_typename(L,t));
      break;
    }
    printf("\n");
  }

  printf("+--+--+-------------------------\n\n");
}

int main( int argc, char** argv)
{

  lua_State* L;
  int n;
  int result;

  if( argc == 1){
    puts("Too few parameters");
    return -1;
  }

  n = atoi( argv[1]);

  L = lua_open();
  luaL_openlibs(L);

  if(luaL_dofile(L, "testlib.lua")){
    puts( "LOAD ERROR");
    return -1;
  }

  lua_getglobal(L, "things_to_buy");
  dump_stack(L);

  lua_getfield(L, -1, "coke");
  dump_stack(L);

  lua_getglobal(L, "fib");
  dump_stack(L);

  lua_pushnumber(L, n);
  dump_stack(L);

  lua_pcall(L, 1, 1, 0);
  dump_stack(L);

  result = lua_tonumber(L, -1);

  lua_close(L);

  printf("fib(%i) = %i\n", n, result);

  return 0;
}
