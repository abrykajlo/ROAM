#include "Mesh.h"
#include "include/Angel.h"
#include <iostream>
#include <string.h>
#include <GL/glui.h>


float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

int   render_options = WIREFRAME;
int   main_window;
float scale = 1.0;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, -2.0 };
char* filename;
Mesh mesh;

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


void control_cb( int control )
{
	if (control == LOAD_ID) {
		mesh.loadMesh(textbox->get_text());
		obj_pos[0] = 0.0;
		obj_pos[1] = 0.0;
		obj_pos[2] = -2.0;
    k_value->set_int_limits(0, mesh.getNumVerts() + mesh.getNumFaces() - 2);
	} else if (control == SAVE_ID) {
    if (mesh.getNumVerts() != 0) { 
	   	mesh.saveMesh(textbox->get_text());
    }
	} else if (control == RADIO_ID) {

    int option = radio->get_int_val();
    if (option == 0) {
      render_options = WIREFRAME;
    } else if (option == 1) {
      render_options = WIREFRAME | FLAT;
    } else if (option == 2) {
      render_options = FLAT;
    } else if (option == 3) {
      render_options = SMOOTH;
    }

	} else if (control == DECIM_ID) {
    if (mesh.getNumVerts() != 0) {
    int k = k_value->get_int_val();
    int d = num_edges->get_int_val();
    while (d != 0) {
      mesh.decimate(k, 1);
      d--;
    }
    k_value->set_int_limits(0, mesh.getNumVerts() + mesh.getNumFaces());
  }
  } else if (K_ID) {
    num_edges->set_int_limits(0, k_value->get_int_val());
  }

  glutPostRedisplay();
}


void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
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
  glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, 0.1, 15.0 );

  glMatrixMode( GL_MODELVIEW );

  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -2.6f );
  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
  glMultMatrixf( view_rotate );
  
  glScalef( scale, scale, scale );
  
  mesh.drawMesh(render_options);
  glutSwapBuffers(); 
}


int main(int argc, char* argv[])
{
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
    new GLUI_Translation(glui, "Translate", GLUI_TRANSLATION_XY, obj_pos );
  trans_xy->set_speed( .005 );
  new GLUI_Column( glui, false );
  GLUI_Translation *trans_z = 
    new GLUI_Translation( glui, "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
  trans_z->set_speed( .005 );
  new GLUI_Column( glui, false );
  radio = new GLUI_RadioGroup( glui, NULL, RADIO_ID, control_cb );
  glui->add_radiobutton_to_group(radio, "wireframe");
  glui->add_radiobutton_to_group(radio, "flat with wireframe");
  glui->add_radiobutton_to_group(radio, "flat shading");
  glui->add_radiobutton_to_group(radio, "smooth");
  new GLUI_Column( glui, false );
  textbox = new GLUI_EditText( glui, "", GLUI_EDITTEXT_TEXT );
  new GLUI_Button( glui, "Save", SAVE_ID, control_cb );
  
  new GLUI_Button( glui, "Load", LOAD_ID, control_cb );
  
  new GLUI_Column(glui,false);

  k_value = new GLUI_Spinner( glui, "k value", GLUI_SPINNER_INT, K_ID, control_cb);

  num_edges = new GLUI_Spinner( glui, "num edges", GLUI_SPINNER_INT);

  k_value->set_int_limits(0, 0);

  num_edges->set_int_limits(0, 0);

  new GLUI_Button( glui, "Decimate", DECIM_ID, control_cb );

  new GLUI_Column(glui, false);
  new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

  glutPostRedisplay();
  
  glutMainLoop();

  return EXIT_SUCCESS;
}

