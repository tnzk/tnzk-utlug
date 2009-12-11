#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ss08.h"

void dump_stack( lua_State* L)
{
  /*
    Dumping Lua stack to show value, positive index and negative index of each element.
    If an element is typed as a string, a boolean or a number then print its value.
    In other case this will print a type of the element.
   */
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

void setfield(lua_State* L, const char* key, int value)
{
  lua_pushnumber(L, value);
  lua_setfield(L, -2, key);
}

void add_enminfo(lua_State* L, int index, TkbEnemy* enm)
{
  lua_pushnumber(L, index);
  lua_newtable(L);
  setfield(L, "id",   enm->id);
  setfield(L, "type", enm->type);
  setfield(L, "x", enm->x);
  setfield(L, "y", enm->y);
  setfield(L, "theta", enm->theta);
  lua_settable(L, -3);
}

int main( int argc, char** argv)
{
  
  TkbEnemy enms[NUM_ENEMY];
  lua_State* L;
  int i;
  int result;

  if( argc == 1){
    puts("Too few parameters");
    return -1;
  }

  for( i = 0; i < NUM_ENEMY; i++){
    enms[i].id = i;
    enms[i].type = NUM_ENEMY - i;
    enms[i].x = i + 1;
    enms[i].y = i + 2;
    enms[i].theta = i * 15;
  }

  L = lua_open();
  luaL_openlibs(L);

  if(luaL_dofile(L, argv[1])){
    puts( "LOAD ERROR");
    return -1;
  }

  lua_getglobal(L, "decision");
  lua_newtable(L);
  for( i = 0; i < NUM_ENEMY; i++){
    add_enminfo(L, i + 1, (enms + i));
  }
  lua_pcall(L, 1, 1, 0);
  dump_stack(L);

  lua_close(L);

  //printf("fib(%i) = %i\n", n, result);

  return 0;
}
