#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

int width = 600;
int height = 600;

void reshape( int w, int h)
{
  h = (h == 0) ? 1 : h;
  glViewport(0, 0, w, h);
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  width = w;
  height = h;
  glOrtho( -100, width - 101.0, -100.0, height - 101.0, -1.0, 1.0);
  glMatrixMode( GL_MODELVIEW);
}

void ploti( int x, int y)
{
  glBegin( GL_POINTS);
  glVertex2d( x, y);
  glEnd();
}

void line( int x1, int y1, int x2, int y2)
{
  int x = x1;
  int y = y1;
  double m = (y2 - y1) / (x2 - x1);
  double e = 0;

  glPointSize(1);

  glColor3d( 0, 0, 0);

  while( x != x2){
    x++;
    e += m;
    if( e >= 0.5){
      y++; 
      e--;
    }
    ploti( x, y);
  }
}

void octant( int r)
{
  int x = 0;
  int y = r;
  int d = 3 - (r << 1);

  ploti( x, y);

  while(x < y){
    if(d < 0){
      d += (x << 2) + 6;
    }else{
      d += ((x - y) << 2) + 10;
      y--;
    }
    ploti( ++x, y);
  }
}

void display()
{
  int i;
  int x, y;
  int x2, y2;
  double a = 1.99239;
  double u = 0.0;
  double v = 0.087156;
  double w;
  double p = 1.0;
  double q = 0.99619;
  double r;
  double m = 400;

  glClear( GL_COLOR_BUFFER_BIT);
  line( 0, 0, (int)m, 0);
  line( 0, 0, (int)(m * q), (int)(m * v));
  for( i = 5; i < 45; i += 5){
    w = a * v - u;
    r = a * q - p;
    x = (int)(m * r);
    y = (int)(m * w);
    line( 0, 0, x, y);
    u = v;
    v = w;
    p = q;
    q = r;
  }

  for( i = 20; i <= 400; i += 20){
    octant(i);
    glFlush();
  }

  glFlush();
  glutSwapBuffers();
}

void myInit()
{
  glClearColor( 1.0, 1.0, 1.0, 0.0);
  glColor3f( 0, 0, 0);
}

int main(int argc, char *argv[])
{

  glutInitWindowSize( width, height);
  glutInitWindowPosition( 0, 0);
  glutInit( &argc, argv);
  glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow( "Ingrid said,");
  myInit();
  glutDisplayFunc( display);
  glutReshapeFunc( reshape);
  //glutKeyboardFunc( keyboard);

  glutMainLoop();

  return 0;
}

