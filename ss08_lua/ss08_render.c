
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

void showbox( int size)
{
  int s = size >> 1;

  glBegin(GL_TRIANGLES);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(0 , 1); glVertex2d( -s,  s);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(1 , 0); glVertex2d(  s, -s);
  
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
      glRotatef( enms[i].theta, 0, 0, 1.0);
      glColor3f( 0.8 * ((enms[i].type & 1) >> 0),
		 0.8 * ((enms[i].type & 2) >> 1),
		 0.8 * ((enms[i].type & 4) >> 2));
      showbox( 32);
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

void timer(int value) {
  int i = 0;
  int j;
  lua_getglobal(Ls[i], "decision");
  lua_newtable(Ls[i]);
  for( j = 0; j < NUM_ENEMY; j++){
    add_enminfo(Ls[i], i + 1, (enms + i));
    }
  lua_pcall(Ls[i], 1, 1, 0);
  
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
    enms[i].type = NUM_ENEMY - i;
    enms[i].x = (i + 1) * 100 - 320;
    enms[i].y = (i + 2) * 100 - 320;
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

