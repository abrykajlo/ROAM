#include "RTIN.h"
#include "include/Angel.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <GL/glui.h>

using namespace std;

int frame = 0, currentTime, baseTime = 0;

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

int   main_window;
int   wireframe_win;
int   terrain_win;
int   sidebar_win;
float scale = 1.0;
mat4 view_rotate = mat4(1.0);

vec4 eye_pos = vec4(0.0, 0.0, 0.0, 1.0);
vec4 eye_dir = vec4(0.1, 0.0, 0.0, 0.0);
RTIN r;

GLfloat light0_ambient[] =  {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat light0_diffuse[] =  {.5f, .5f, .5f, 1.0f};
GLfloat light0_position[] = {-1.0f, 1.0f, 1.0f, 0.0f};

void init(int levels) {
  r.Triangulate("heightmap.bmp", levels, &eye_pos, &eye_dir);
  r.SetEye(&eye_pos, &eye_dir);
}

void setOrthographicProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void setCameraView() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  mat4 cameraView = LookAt(eye_pos, eye_pos+eye_dir, vec4(0.0, 0.0, 1.0, 1.0));
  glMultMatrixf(cameraView);
}

void PrintFrameRate() {
  frame++;
  currentTime = glutGet(GLUT_ELAPSED_TIME);
  if (currentTime - baseTime > 1000) {
    printf("FPS:%4.2f", frame*1000.0/(currentTime-baseTime));
    printf("\r"); //carriage return
    baseTime = currentTime;
    frame = 0;
  }
}

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
  case 'a':
    eye_dir = RotateZ(2)*eye_dir;
    break;
  case 'd':
    eye_dir = RotateZ(-2)*eye_dir;
    break;
  case 'w':
    eye_pos = eye_pos + 0.1 * eye_dir;
    break;
  case 's':
    eye_pos = eye_pos - 0.1 * eye_dir;
    break;
  case '=':
    eye_pos = Translate(0.0, 0.0, 0.01) * eye_pos;
    break;
  case '-':
    eye_pos = Translate(0.0, 0.0, -0.01) * eye_pos;
    break;
  case '/':
    glutFullScreen();
    break;
  case 'r':
    glutPostRedisplay();
  };
  
  glutPostRedisplay();
}



void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}



void myGlutIdle( void )
{
  
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  


  glutPostRedisplay();
}


void myGlutMouse(int button, int button_state, int x, int y )
{
}



void myGlutMotion(int x, int y )
{
  glutPostRedisplay(); 
}


void myGlutReshape( int x, int y )
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );

  xy_aspect = (float)tw / (float)th;

  glutPostRedisplay();
}

void myGlutDisplay( void )
{
  PrintFrameRate();
  glutSetWindow(sidebar_win);
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glutSwapBuffers();

  glutSetWindow(wireframe_win);
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  setOrthographicProjection();
  // glMultMatrixf(cameraView);
  // glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, 0.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  //r.Update();
  r.DrawWire();
  r.DrawEye();
  glutSwapBuffers();

  glutSetWindow(terrain_win);
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  setCameraView();
  r.Draw();

  glScalef( scale, scale, scale );
  
  glutSwapBuffers(); 
}


int main(int argc, char* argv[])
{
  if (argc == 1) {
    init(5);
  } else {
    init(atoi(argv[1]));
  }
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( 800, 600 );
  
  main_window = glutCreateWindow( "ROAM - Realtime Optimally Adapting Meshes" );
  terrain_win = glutCreateSubWindow(main_window, 0, 0, 600, 600);
  wireframe_win = glutCreateSubWindow(main_window, 600, 0, 200, 200);
  sidebar_win = glutCreateSubWindow(main_window, 600, 200, 200, 400);
  glutDisplayFunc( myGlutDisplay );
  GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
  GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( myGlutMouse );
  glutMotionFunc( myGlutMotion );

  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_DEPTH_TEST);

  glewInit();


  printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );
  
  glutPostRedisplay();
  
  glutMainLoop();

  return EXIT_SUCCESS;
}

