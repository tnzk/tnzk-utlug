#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

int width = 640;
int height = 480;

void reshape( int w, int h)
{
  h = (h == 0) ? 1 : h;
  glViewport(0, 0, w, h);
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  width = w;
  height = h;
  glOrtho( 0, 640, 480, 0, -1.0, 1.0);
  glMatrixMode( GL_MODELVIEW);
}

void display()
{
  glClear( GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  glVertex2d(100,100);
  glVertex2d(200,200);
  glVertex2d(100,200);
  glEnd();

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

  glutCreateWindow( "ss08");
  myInit();
  glutDisplayFunc( display);
  glutReshapeFunc( reshape);
  //glutKeyboardFunc( keyboard);

  glutMainLoop();

  return 0;
}

