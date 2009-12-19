
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

void showbox( int size, int id)
{
  int s = size >> 1;
  int m = size >> 3;
  int cid = id + 1;

  if(enms[id].heat){
    double ratio = (double)enms[id].heat / (double)OLD_AGE;
    glColor3f( ratio, ratio * 0.5, 0.0);
  }else{
    glColor3f( 0.2, 0.2, 0.2); 
  }
  glBegin(GL_TRIANGLES);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(0 , 1); glVertex2d( -s,  s);
  
  glTexCoord2f(0 , 0); glVertex2d( -s, -s);
  glTexCoord2f(1 , 1); glVertex2d(  s,  s);
  glTexCoord2f(1 , 0); glVertex2d(  s, -s);
  
  glEnd();


  glColor3f( 0.8 * ((cid & 1) >> 0),
	     0.8 * ((cid & 2) >> 1),
	     0.8 * ((cid & 4) >> 2));
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
      if( IS_OLD(enms + i) || (enms[i].age & 4)){
	showbox( ENEMY_SIZE, enms[i].id);
      }
    }
    glPopMatrix();
  }
  for( i = 0; i < NUM_ENEMY; i++){
    int ci = i + 1;
    int j;
    glColor3f( 0.8 * ((ci & 1) >> 0),
	       0.8 * ((ci & 2) >> 1),
	       0.8 * ((ci & 4) >> 2));
    for( j = 0; j < enms[i].bital; j++){
      glBegin(GL_TRIANGLES);
      
      glVertex2d( -300 + j * 5, 218 - i * 21);
      glVertex2d( -297 + j * 5, 198 - i * 21);
      glVertex2d( -297 + j * 5, 218 - i * 21);
      
      glVertex2d( -300 + j * 5, 218 - i * 21);
      glVertex2d( -297 + j * 5, 198 - i * 21);
      glVertex2d( -300 + j * 5, 198 - i * 21);
      
      glEnd();
    }

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

double normalize_rad( double rad)
{
  double d;
  
  if(rad > 0.0){
    d = -M_PI * 2;
  }else{
    d =  M_PI * 2;
  }
  while(!(0.0 <= rad && rad <= M_PI * 2)){
    rad += d;
  }
  return rad;
}

void get_decision(lua_State* L, int id, int* mov, int* dir, int* sht)
{
  int i;
  lua_getglobal(L, "decision");
  lua_newtable(L);
  for( i = 0; i < NUM_ENEMY; i++){
    add_enminfo(L, i + 1, (enms + i));
  }
  lua_pushnumber(L, id + 1);
  
  lua_pcall(L, 2, 1, 0);
  *mov = getfield(L, -1, "move");
  *dir = getfield(L, -1, "direction");
  *sht = getfield(L, -1, "shoot");
  lua_pop(L, 1);
}

int hit_test( TkbEnemy* enm1, TkbEnemy* enm2)
{
  // 0, 1 => false, true
  int px[4];
  int py[4];
  int qx[4];
  int qy[4];
  int h_enm = ENEMY_SIZE >> 1;
  int i;
  int dx, dy;
  double l;

  px[0] = enm1->x - h_enm; py[0] = enm1->y - h_enm;
  px[1] = enm1->x - h_enm; py[1] = enm1->y + h_enm;
  px[2] = enm1->x + h_enm; py[2] = enm1->y + h_enm;
  px[3] = enm1->x + h_enm; py[3] = enm1->y - h_enm;

  qx[0] = enm2->x - h_enm; qy[0] = enm2->y - h_enm;
  qx[1] = enm2->x - h_enm; qy[1] = enm2->y + h_enm;
  qx[2] = enm2->x + h_enm; qy[2] = enm2->y + h_enm;
  qx[3] = enm2->x + h_enm; qy[3] = enm2->y - h_enm;
  
  for( i = 0; i < 4; i++){
    dx = enm1->x - qx[i];
    dy = enm1->y - qy[i];
    l = sqrt( (double)(dx * dx + dy * dy));
    if( l < (double)h_enm + 2.0){
      return 1;
    }
  }
  return 0;

}

void enm_move( TkbEnemy* enm, int mov)
{
  double dx = mov * 4.0 * cos(enm->theta);
  double dy = mov * 4.0 * sin(enm->theta);
  int h_enm = ENEMY_SIZE >> 1;

  enm->x += (int)dx;
  enm->y += (int)dy;

  if(enm->x < -320 + h_enm) enm->x = -320 + h_enm;
  if(enm->x >  320 - h_enm) enm->x =  320 - h_enm;
  if(enm->y < -240 + h_enm) enm->y = -240 + h_enm;
  if(enm->y >  240 - h_enm) enm->y =  240 - h_enm;

}

void enm_turn( TkbEnemy* enm, int dir)
{

  switch(dir){
  case -1: enm->theta -= D_OMG; break;
  case  0: break;
  case  1: enm->theta += D_OMG; break;
  }
}

void enm_shoot( TkbEnemy* enm, int sht)
{
  if(sht != 1) return;
  if(IS_OLD(enm) && ((enm->age - enm->attacked) > OLD_AGE)&& !(enm->heat)){
    enm->heat = OLD_AGE;
    enm->attacked = enm->age;
  }
}

void timer(int value) {
  int i, j;
  int mov = 0; // move
  int dir = 0; // direction
  int sht = 0; // shoot

  for( i = 0; i < NUM_ENEMY; i++){
    enms[i].age++;
    if(enms[i].heat > 0) enms[i].heat--;
    enms[i].theta = normalize_rad(enms[i].theta);
    for( j = 0; j < NUM_ENEMY; j++){
      if( ( i != j) && (IS_OLD(enms + i)) && (IS_OLD(enms + j))
	  && (enms[i].bital > 0) && (enms[j].bital > 0)
	  && (hit_test((enms + i),(enms + j)))){

	if((enms[i].heat) && !(enms[j].heat)){
	  enms[j].x = def_x[i];
	  enms[j].y = def_y[i];
	  enms[j].age = 0;
	  enms[j].bital--;
	}else if(!(enms[i].heat) && (enms[j].heat)){
	  
	}else{

	  enms[i].x = def_x[i];
	  enms[i].y = def_y[i];
	  enms[j].x = def_x[j];
	  enms[j].y = def_y[j];

	  enms[i].age = 0;
	  enms[j].age = 0;
	  //enms[i].bital--;
	  //enms[j].bital--;
	}
      }
    }
  }

  for( i = 0; i < NUM_ENEMY; i++){
    if( enms[i].bital > 0){
      lua_State* L = Ls[enms[i].id];
      get_decision(L, enms[i].id, &mov, &dir, &sht);
      enm_move(  enms + i, mov);
      enm_turn(  enms + i, dir);
      enm_shoot( enms + i, sht);
    }
  }

  glutPostRedisplay();
  glutTimerFunc(10 , timer , 0);
}

int main(int argc, char *argv[])
{
  int i;

  if( argc == 1){
    puts("Too few parameters. This require a file name of lua script as an argument.");
    return -1;
  }
  num_ai = argc - 1;
  Ls = (lua_State**)malloc(NUM_ENEMY * sizeof(lua_State*));

  enms = (TkbEnemy*)malloc(sizeof(TkbEnemy) * NUM_ENEMY);

  for( i = 0; i < NUM_ENEMY; i++){
    enms[i].id = i;
    enms[i].type = i % num_ai;
    enms[i].x = def_x[i];
    enms[i].y = def_y[i];
    enms[i].theta = 0.0;
    enms[i].age = 0;
    enms[i].heat = 0;
    enms[i].attacked = 0;
    enms[i].bital = 10;
  }

  for( i = 0; i < NUM_ENEMY; i++){
    Ls[i] = lua_open();
    luaL_openlibs(Ls[i]);

    if(luaL_dofile(Ls[i], argv[enms[i].type + 1]) ) {
      printf("%sを開けませんでした\n", argv[i + 1]);
      printf("error : %s\n", lua_tostring(Ls[i], -1) );
      return 1;
    }
  }



  glutInitWindowSize( width, height);
  glutInitWindowPosition( 0, 0);
  glutInit( &argc, argv);
  glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);

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

