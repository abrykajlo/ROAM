// ASSIGNMENT #1
// Jon Holmes
// (301123551)

#include "include/Angel.h"
#include "include/GL/glui.h"

#include "HeightMap.h"

#include <iostream>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <vector>

using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec3 point3;
typedef Angel::vec4 color4;

//---------------------------------------------------------------------------

//Variables for transforming loaded object
GLfloat pos[3] = {0.0};
float view_rotate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
float obj_scale  = 75.0;

//---------------------------------------------------------------------------

GLuint m_vaoID; //Vertex array
GLuint m_vboID; //Vertex buffer

//Maximum number of faces: 50000
const int NumVertices_Obj = 5000000*3; 

//Int ID for main window
int   main_window;

//Declarations for parsing SMF files
vector<point4> vertex_list;
vector<point3> face_list;
vector<vec3> norm_list;
unsigned int vertex_count = 0;
unsigned int face_count = 0;

//Arrays to hold final vertices and normals
point4 obj_points[NumVertices_Obj];
vec3 obj_normals[NumVertices_Obj];

// Program for transformations
GLuint obj_program;

// Shader transformation matrices
mat4  model_view_obj, projection_obj;
GLuint  ModelViewObj, ProjectionObj;

//----------------------------------------------------------------------------

//GLUI controls declarations
GLUI *glui;		
GLUI_EditText *savetext;	
GLUI_EditText *loadtext;
GLUI_Button *savebtn;	
GLUI_Button *loadbtn;	

//String list to hold render options
char *string_list[] = { (char*)"Flat Shaded", (char*)"Smooth Shaded", (char*)"Wireframe", (char*)"Shaded with Mesh Edges" };
int   curr_string = 0; //String list index

//----------------------------------------------------------------------------

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(m_vaoID);
    glUseProgram(obj_program);

    //Start with elementary matrix
    model_view_obj = mat4 (1.0); 

    //Transform model view of object based on rotation
    model_view_obj *= mat4( view_rotate[0],view_rotate[1],view_rotate[2],view_rotate[3],
                    		view_rotate[4],view_rotate[5],view_rotate[6],view_rotate[7],
                    		view_rotate[8],view_rotate[9],view_rotate[10],view_rotate[11],
                    		view_rotate[12],view_rotate[13],view_rotate[14],view_rotate[15]	);

    //Transform model view based on translation and scale
    mat4 instance = ( Translate( pos[0], pos[1], pos[2] ) * Scale( obj_scale ) );
    glUniformMatrix4fv(ModelViewObj, 1, GL_TRUE, model_view_obj * instance);

    glDrawArrays(GL_TRIANGLES, 0, NumVertices_Obj);

    glBindVertexArray(0);

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void init(void) {
    // Create vertex array object
    glGenVertexArrays(1, &m_vaoID);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Create and initialize buffer object
    glBindVertexArray(m_vaoID);

    // Pass in array of vertices (points) and normals to buffer
    glGenBuffers(1, &m_vboID);
    glBindBuffer( GL_ARRAY_BUFFER, m_vboID);
    glBufferData( GL_ARRAY_BUFFER, sizeof(obj_points) + sizeof(obj_normals), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(obj_points), obj_points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(obj_points), sizeof(obj_normals), obj_normals );

    // Load shaders and use the resulting shader program
    obj_program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(obj_program);
    
    // Set up vertex arrays
    GLuint vPosition_Obj = glGetAttribLocation(obj_program, "vPosition");
    glEnableVertexAttribArray(vPosition_Obj);
    glVertexAttribPointer( vPosition_Obj, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    GLuint vNormal = glGetAttribLocation(obj_program, "vNormal"); 
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(obj_points)) );

    // Initialize shader lighting parameters
    point4 light_position(0.0, 0.0, 2.0, 0.0);
    color4 light_ambient(0.2, 0.2, 0.2, 1.0);
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);

    color4 material_ambient(0.0, 0.2, 0.8, 1.0);
    color4 material_diffuse(0.0, 0.8, 0.2, 1.0);
    color4 material_specular(0.0, 0.2, 0.8, 1.0);
    float  material_shininess = 5.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv(glGetUniformLocation(obj_program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(obj_program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(obj_program, "SpecularProduct"), 1, specular_product);
    
    glUniform4fv(glGetUniformLocation(obj_program, "LightPosition"), 1, light_position);

    glUniform1f(glGetUniformLocation(obj_program, "Shininess"), material_shininess);
         
    // Retrieve transformation uniform variable locations
    ModelViewObj = glGetUniformLocation(obj_program, "ModelViewObj");
    ProjectionObj = glGetUniformLocation(obj_program, "ProjectionObj");

    // Rest of init
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void mouse(int button, int state, int x, int y) {
	//Redisplay on mouse click (CSIL terminals have issues displaying on init)
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshape(int width, int height) {
	//Build and bind viewport to program
    glViewport(0, 0, width, height);

    GLfloat  left = -100.0, right = 100.0;
    GLfloat  bottom = -110.0, top = 90.0;
    GLfloat  zNear = -100.0, zFar = 100.0;

    GLfloat aspect = GLfloat(width)/height;

    if ( aspect > 1.0 ) { left *= aspect; right *= aspect; }
    else { bottom /= aspect; top /= aspect; }

    //Build projection matrix based on viewport values
    mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);

    glUseProgram(obj_program);
    glUniformMatrix4fv(ProjectionObj, 1, GL_TRUE, projection);
    model_view_obj = mat4(1.0);
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y) {
	// Quit function on ESC or Q
    switch(key) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
	// Redisplay function on R (CSIL terminals have issues displaying on init)
	case 'r': case 'R':
	    glutPostRedisplay();
	    break;
    }

}

//----------------------------------------------------------------------------

void control_cb(int control) {
	// Functions for GLUI controls
    #define SAVEID 500
    #define LOADID 501
    if (control == SAVEID) cout << "Save function not implemented\n";
    if (control == LOADID) cout << "Load function not implemented\n";
}

//----------------------------------------------------------------------------

int main(int argc, char *argv[]) {


    HeightMap height_vals("heightmap.bmp", 0.5);

    //Partition amount
	int w = height_vals.GetWidth();
    int h = height_vals.GetHeight();


    //Build vertex list
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            point4 new_vertex;
            new_vertex.x = -1.0+((j/((w-1)*1.0))*2.0);
            new_vertex.z = -1.0+((i/((h-1)*1.0))*2.0);
            new_vertex.y = height_vals(new_vertex.x, new_vertex.z);
            new_vertex.w = 1;
            vertex_list.push_back(new_vertex);
        }
    }


    //Build face list
    for (int i = 0; i < (h-1); ++i) {
        for (int j = 0; j < (w-1); ++j) {
            point3 new_face1;
            point3 new_face2;

            new_face1.x = (i*w)+(j+0);
            new_face1.y = (i*w)+(j+1);
            new_face1.z = ((i+1)*w)+(j+0);
            
            new_face2.x = (i*w)+(j+1);
            new_face2.y = ((i+1)*w)+(j+0);
            new_face2.z = ((i+1)*w)+(j+1);

            face_list.push_back(new_face1);
            face_list.push_back(new_face2);
        }
    }

    //Set vertex and face count
    vertex_count = vertex_list.size();
    face_count = face_list.size();

    //Iterate through face list to build new vertex list in triangle pattern
    for (unsigned int i = 0; i < face_count; i++) {
    	//Build triangle based on indices of vertices from face list
        point4 a = vertex_list[(int)face_list[i].x];
        point4 b = vertex_list[(int)face_list[i].y];
        point4 c = vertex_list[(int)face_list[i].z];

        //Push vertices of triangle on to final vertex (point) list
        obj_points[(i*3)] = a;
        obj_points[(i*3)+1] = b;
        obj_points[(i*3)+2] = c;    

        //Calculate normal of face using vertices of the triangle
        vec3 normal = normalize( cross(b - a, c - b) );
        norm_list.push_back(normal);

        //Push normals of each face on to final normal list, with redundancy to match length of vertex list
        obj_normals[(i*3)] = normal;
        obj_normals[(i*3)+1] = normal;
        obj_normals[(i*3)+2] = normal;
    }


    //--------------------------------------------
    // END SMF PARSING NOT NEEDED
    //--------------------------------------------

    //GLUT initializations
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( 800, 600 );
    glutInitContextVersion( 3, 0 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    main_window = glutCreateWindow( "SMF Viewer" );

    //Main init
    glewInit();        
    init();

    //GLUT function binds
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    //Set background colour to grey
    glClearColor( 0.5f, 0.5f, 0.5f, 1.0f);

    //Create bottom panel using GLUI
    glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
    glui->set_main_gfx_window(main_window);

    //Add rotation ball
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui, "Rotate", view_rotate);
    view_rot->set_spin(0.05);
    new GLUI_Column(glui, false);

    //Add zoom slider
    GLUI_Translation *trans_y = new GLUI_Translation(glui, "Zoom", GLUI_TRANSLATION_Z, &obj_scale);
    trans_y->set_speed(0.05);
    new GLUI_Column(glui, false);

    //Add translation x/y sliders
    GLUI_Translation *trans_xy = new GLUI_Translation(glui, "Translate X/Y", GLUI_TRANSLATION_XY, pos);
    trans_xy->set_speed(0.05);
    new GLUI_Column(glui, false);

    //Add translate z slider
    GLUI_Translation *trans_z = new GLUI_Translation(glui, "Translate Z", GLUI_TRANSLATION_Z, &pos[2]);
    trans_z->set_speed(0.05);
    new GLUI_Column(glui, false);

    //Add save file text field and button
    new GLUI_StaticText(glui, "");
    savetext = new GLUI_EditText(glui, "Save File:", GLUI_EDITTEXT_TEXT);
    savebtn = new GLUI_Button(glui, "Save", SAVEID, control_cb);
    new GLUI_Column(glui, false);

    //Add load file text field and button
    new GLUI_StaticText(glui, "");
    loadtext = new GLUI_EditText(glui, "Open File:", GLUI_EDITTEXT_TEXT);
    loadbtn = new GLUI_Button(glui, "Open", LOADID, control_cb);
    new GLUI_Column(glui, false);

    //Add list of rendering options
    new GLUI_StaticText(glui, "");
    GLUI_Listbox *list = new GLUI_Listbox(glui, "Mode:", &curr_string);
    for(int i = 0; i < 4; i++) list->add_item(i, string_list[i]);
    new GLUI_StaticText(glui, "");
    
    //Quit button
    new GLUI_Button(glui, "Quit", 0,(GLUI_Update_CB)exit);

    //Main GLUT loop
    glutMainLoop();
    return 0;
}