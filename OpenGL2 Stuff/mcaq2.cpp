#include "RTIN.h"
#include "include/Angel.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <GL/glui.h>


float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

int   main_window;
float scale = 1.0;
mat4 view_rotate = mat4(1.0);
float obj_pos[] = { 0.0, 0.0, -2.0 };
char* filename;
vec4 eye_pos = vec4(0.0, 0.0, 0.0, 1.0);
vec4 eye_dir = vec4(0.1, 0.0, 0.0, 0.0);
RTIN r;

GLUI *glui;
GLUI_EditText *textbox;
GLUI_Spinner *k_value;
GLUI_Spinner *num_edges;
GLUI_RadioGroup *radio;


#define LOAD_ID              305
#define SAVE_ID              306
#define RADIO_ID             307 
#define DECIM_ID             308
#define K_ID                 309
#define EDGES_ID             310

GLfloat light0_ambient[] =  {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat light0_diffuse[] =  {.5f, .5f, .5f, 1.0f};
GLfloat light0_position[] = {-1.0f, 1.0f, 1.0f, 0.0f};

void init(int levels) {
  r.Triangulate("heightmap.bmp", levels);
  std::cout << r.size << std::endl;
  r.flags[1] = 1;
  r.flags[2] = 1;
  r.ForceSplit(1);
  r.ForceSplit(5);
  r.ForceSplit(12);
  r.Merge(12);
  // for (int i = 0; i < r.size; i++) {
  //   if (r.Child(LEFT, i) == -1) r.flags[i] = 1;
  // }
  r.BuildWedgies();
  r.SetEye(&eye_pos, &eye_dir);
  //r.WedgieTreePrint();
}


void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
  case GLUT_KEY_LEFT:
    view_rotate = RotateZ(-5) * view_rotate;
  case GLUT_KEY_RIGHT:
    view_rotate = RotateZ(5) * view_rotate;
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
  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  eye_dir = view_rotate * vec4(1.0, 0.0, 0.0, 0.0);
  //mat4 cameraView = LookAt(eye_pos, eye_pos+(view_rotate*eye_dir), vec4(0.0,0.0,1.0,1.0));
  //glMultMatrixf();
  //glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, 0.0, 15.0 );

  glMatrixMode( GL_MODELVIEW );

  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glLoadIdentity();
  //glTranslatef( 0.0, 0.0, -2.6f ); 
  glMultMatrixf( view_rotate );
  
  glScalef( scale, scale, scale );
  
  r.DrawWire();
  r.DrawEye();
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
 
  main_window = glutCreateWindow( "SMF Viewer" );
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

  glui = GLUI_Master.create_glui_subwindow( main_window, 
                                             GLUI_SUBWINDOW_BOTTOM );
  glui->set_main_gfx_window( main_window );

  GLUI_Rotation *view_rot = new GLUI_Rotation(glui, "Rotate", view_rotate );
  view_rot->set_spin( 1.0 );
  new GLUI_Column( glui, false );
  GLUI_Translation *trans_xy = 
    new GLUI_Translation(glui, "Translate", GLUI_TRANSLATION_XY, eye_pos );
  trans_xy->set_speed( .005 );
  new GLUI_Column( glui, false );
  GLUI_Translation *trans_z = 
    new GLUI_Translation( glui, "Zoom", GLUI_TRANSLATION_Z, &eye_pos[2] );
  trans_z->set_speed( .005 );
  
  glutPostRedisplay();
  
  glutMainLoop();

  return EXIT_SUCCESS;
}

