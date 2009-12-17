// Using Lua libralies
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Using C standard libraries
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

int getfield(lua_State* L, int index, const char* key)
{
  int r;
  lua_getfield(L, index, key);
  r = lua_tonumber(L, -1);
  lua_pop(L,1);
  return r;
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
/*
int main( int argc, char** argv)
{
  
  TkbEnemy enms[NUM_ENEMY];
  lua_State* L;
  lua_State** Ls;
  int i;
  int num_ai;
  int result;

  if( argc == 1){
    puts("Too few parameters. This require a file name of lua script as an argument.");
    return -1;
  }
  num_ai = argc - 1;
  Ls = (lua_State**)malloc(num_ai * sizeof(lua_State*));

  for( i = 0; i < NUM_ENEMY; i++){
    enms[i].id = i;
    enms[i].type = NUM_ENEMY - i;
    enms[i].x = i + 1;
    enms[i].y = i + 2;
    enms[i].theta = i * 15;
  }

  for( i = 0; i < num_ai; i++){
    int j;
    Ls[i] = lua_open();
    luaL_openlibs(Ls[i]);

    if(luaL_dofile(Ls[i], argv[i + 1])){
      puts( "LOAD ERROR");
      return -1;
    }
  }
    lua_getglobal(Ls[i], "decision");
    lua_newtable(Ls[i]);
    for( j = 0; j < NUM_ENEMY; j++){
      add_enminfo(Ls[i], i + 1, (enms + i));
    }
    lua_pcall(Ls[i], 1, 1, 0);


  return 0;
}

*/
