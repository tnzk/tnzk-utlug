
// Using Lua libraries
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Using OpenGL/GLUT libraries
#include <GL/gl.h>
#include <GL/glut.h>

// Using C standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ss08.h"

int width = 640;
int height = 480;
int num_ai;
lua_State** Ls;
TkbEnemy* enms;

void reshape( int w, int h)
{
  h = (h == 0) ? 1 : h;
  glViewport(0, 0, w, h);
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  width = w;
  height = h;
  glOrtho( -320, 320, -240, 240, -1.0, 1.0);
  glMatrixMode( GL_MODELVIEW);
}

void showbox( int size, int type)
{
  int s = size >> 1;
  int m = size >> 3;

  glColor3f( 0.8 * ((type & 1) >> 0),
	     0.8 * ((type & 2) >> 1),
	     0.8 * ((type & 4) >> 2));


  glBegin(GL_TRIANGLES);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(0 , 1); glVertex2d( -s,  s);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(1 , 0); glVertex2d(  s, -s);
  
  glEnd();

  glColor3f( 1.0, 1.0, 1.0); 
  glBegin(GL_TRIANGLES);
  
  glTexCoord2f(0 , 0); glVertex2d( -m + 10, -m);
  glTexCoord2f(1 , 1); glVertex2d(  m + 10,  m);
  glTexCoord2f(0 , 1); glVertex2d( -m + 10,  m);
 
  glTexCoord2f(0 , 0); glVertex2d( -m + 10, -m);
  glTexCoord2f(1 , 1); glVertex2d(  m + 10,  m);
  glTexCoord2f(1 , 0); glVertex2d(  m + 10, -m);
  
  glEnd();


}

void display()
{
  int i;

  glClear( GL_COLOR_BUFFER_BIT);

  for( i = 0; i < NUM_ENEMY; i++){
    glPushMatrix();
    {
      glLoadIdentity();
      glTranslatef( enms[i].x,
		    enms[i].y,
		    0);
      glRotatef( RAD2DEG(enms[i].theta), 0, 0, 1.0);
      showbox( 32, enms[i].type);
    }
    glPopMatrix;
  }

  glFlush();
  glutSwapBuffers();
}

void myInit()
{
  glClearColor( 1.0, 1.0, 1.0, 0.0);
  glColor3f( 0, 0, 0);

  glEnable(GL_TEXTURE_2D);
}

void get_decision(lua_State* L, int* mov, int* dir, int* sht)
{
  int i;
  lua_getglobal(L, "decision");
  lua_newtable(L);
  for( i = 0; i < NUM_ENEMY; i++){
    add_enminfo(L, i + 1, (enms + i));
  }
  
  lua_pcall(L, 1, 1, 0);
  *mov = getfield(L, -1, "move");
  *dir = getfield(L, -1, "direction");
  *sht = getfield(L, -1, "shoot");
  lua_pop(L, 1);
}

void enm_move( TkbEnemy* enm, int mov)
{
  double dx = mov * 4.0 * cos(enm->theta);
  double dy = mov * 4.0 * sin(enm->theta);

  enm->x += (int)dx;
  enm->y += (int)dy;

}

void enm_turn( TkbEnemy* enm, int dir)
{

  switch(dir){
  case -1: enm->theta -= D_OMG; break;
  case  0: break;
  case  1: enm->theta += D_OMG; break;
  }
}

void timer(int value) {
  int i;
  int mov = 0; // move
  int dir = 0; // direction
  int sht = 0; // shoot

  for( i = 0; i < NUM_ENEMY; i++){
    lua_State* L = Ls[enms[i].type];
    get_decision(L, &mov, &dir, &sht);
    enm_move( enms + i, mov);
    enm_turn( enms + i, dir);

  }

  glutPostRedisplay();
  glutTimerFunc(20 , timer , 0);
}

int main(int argc, char *argv[])
{
  lua_State* L;
  int i;
  int result;

  if( argc == 1){
    puts("Too few parameters. This require a file name of lua script as an argument.");
    return -1;
  }
  num_ai = argc - 1;
  Ls = (lua_State**)malloc(num_ai * sizeof(lua_State*));

  for( i = 0; i < num_ai; i++){
    int j;
    Ls[i] = lua_open();
    luaL_openlibs(Ls[i]);

    if(luaL_dofile(Ls[i], argv[i + 1])){
      puts( "LOAD ERROR");
      return -1;
    }
  }

  glutInitWindowSize( width, height);
  glutInitWindowPosition( 0, 0);
  glutInit( &argc, argv);
  glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);

  enms = (TkbEnemy*)malloc(sizeof(TkbEnemy) * NUM_ENEMY);

  for( i = 0; i < NUM_ENEMY; i++){
    enms[i].id = i;
    enms[i].type = i & 1;
    enms[i].x = (i + 1) * 50 - 320;
    enms[i].y = (i + 2) * 50 - 320;
    enms[i].theta = i * 15;
  }

  glutCreateWindow("ss08");
  myInit();
  glutDisplayFunc( display);
  glutReshapeFunc( reshape);
  //glutKeyboardFunc( keyboard);
  glutTimerFunc( 100 , timer , 0);

  glutMainLoop();

  free(enms);

  return 0;
}

