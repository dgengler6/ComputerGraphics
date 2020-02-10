#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>

	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"

// FUnction headers 

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog);
mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot);

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint pr_wb, pr_wr, pr_ww, pr_b1, pr_b2, pr_b3, pr_b4;

// Model and Textures
Model * wb, * wr, * ww, * b, * b1, * b2, * b3 , * b4;
GLuint texture;

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

#define buffer_object(NAME) \
	unsigned int NAME ## VertexBufferObjID; \
	unsigned int NAME ## IndexBufferObjID; \
	unsigned int NAME ## NormalBufferObjID; \
	unsigned int NAME ## TexCoordBufferObjID; \
	glGenBuffers(1, &(NAME ## VertexBufferObjID)); \
	glGenBuffers(1, &(NAME ## IndexBufferObjID)); \
	glGenBuffers(1, &(NAME ## NormalBufferObjID)); \
	glGenBuffers(1, &(NAME ## TexCoordBufferObjID));

#define buffer_full_setup(MODEL, NAME, MODEL_STRING, PROGRAM) \
	buffer_object(NAME); \
	MODEL=LoadModel(MODEL_STRING); \
	glGenVertexArrays(1, &(NAME ## VertexArrayObjID)); \
	glBindVertexArray(NAME ## VertexArrayObjID); \
	if (MODEL->texCoordArray != NULL) \
		buffer_setup("in_TexCoord", NAME ## TexCoordBufferObjID, MODEL->texCoordArray, MODEL->numVertices, 2, PROGRAM); \
	buffer_setup("in_Position", NAME ## VertexBufferObjID, MODEL->vertexArray, MODEL->numVertices, 3, PROGRAM); \
	buffer_setup("in_Normal", NAME ## NormalBufferObjID, MODEL->normalArray, MODEL->numVertices, 3, PROGRAM); \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NAME ## IndexBufferObjID); \
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MODEL->numIndices*sizeof(GLuint), MODEL->indexArray, GL_STATIC_DRAW);


void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog){
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
	glBufferData(GL_ARRAY_BUFFER, size*dim*sizeof(GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(prog, in_shader), dim, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog, in_shader));
}



mat4 projectionMatrix;

mat4 camMatrix;

// vertex array object
  unsigned int WBVertexArrayObjID, WRVertexArrayObjID, WWVertexArrayObjID, B1VertexArrayObjID,B2VertexArrayObjID,B3VertexArrayObjID,B4VertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry

  unsigned int projectionMatrixBufferObjID;
	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);
	printError("GL inits");
	// Load and compile shader
	pr_wb = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_wr = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_ww = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b1 = loadShaders("lab3-1.vert","lab3-1.frag");
  pr_b2 = loadShaders("lab3-1.vert","lab3-1.frag");
  pr_b3 = loadShaders("lab3-1.vert","lab3-1.frag");
  pr_b4 = loadShaders("lab3-1.vert","lab3-1.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	buffer_full_setup(wb, WB, "windmill/windmill-balcony.obj", pr_wb);
	buffer_full_setup(wr, WR, "windmill/windmill-roof.obj", pr_wr);
	buffer_full_setup(ww, WW, "windmill/windmill-walls.obj", pr_ww);
	buffer_full_setup(b1, B1, "windmill/blade.obj", pr_b1);
  buffer_full_setup(b2, B2, "windmill/blade.obj", pr_b2);
  buffer_full_setup(b3, B3, "windmill/blade.obj", pr_b3);
  buffer_full_setup(b4, B4, "windmill/blade.obj", pr_b4);

		projectionMatrix = frustum(left, right, bottom, top, near, far);

		vec3 p = SetVector(15,10,15); // Camera position
		vec3 l = SetVector(0,5,0); // Look-at point 
		vec3 v = SetVector(0,1,0); //Up vector 
		camMatrix = lookAtv(p, l, v);

    // Load and bind the texture
    LoadTGATextureSimple("dam.tga", &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(pr_wb, "texUnit"), 0);
    glActiveTexture(GL_TEXTURE0);

	// Allocate and activate Vertex Array Object
    glGenBuffers(1, &projectionMatrixBufferObjID);


	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    mat4 t_rot = Rx(t/1000 );
		
		mat4 projectedCam = Mult(projectionMatrix, camMatrix);
    //balcony
    mat4 trans = T(0, 0, 0);	
    mat4 rot = Ry(-M_PI_2);
    mat4 total = Mult(trans,rot);
    mat4 packed = Mult(projectedCam, total);
    // roof
    mat4 trans1 = T(0, 0.5, 0);	
    mat4 rot1 = Ry(0);
    mat4 total1 = Mult(trans1,rot1);
		mat4 packed1 = Mult(projectedCam, total1);
    // walls 
    mat4 trans2 = T(0, 0, 0);	
    mat4 rot2 = Ry(M_PI);
    mat4 total2 = Mult(trans2,rot2);
    mat4 packed2 = Mult(projectedCam, total2);
		// blade 


    

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform4f(glGetUniformLocation(pr_wb,"plain_color"),0.5f,0.4f,0.0f,1.0f);
	glUniformMatrix4fv(glGetUniformLocation(pr_wb, "pack_mat"), 1, GL_TRUE, packed.m);
	glBindVertexArray(WBVertexArrayObjID);    // Select VAO
	glDrawElements(GL_TRIANGLES, wb->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_wr,"plain_color"),1.0f,0.0f,0.0f,1.0f);
	glUniformMatrix4fv(glGetUniformLocation(pr_wr, "pack_mat"), 1, GL_TRUE, packed1.m);
	glBindVertexArray(WRVertexArrayObjID);
	glDrawElements(GL_TRIANGLES, wr->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_ww,"plain_color"),1.0f,1.0f,1.0f,1.0f);
	glUniformMatrix4fv(glGetUniformLocation(pr_ww, "pack_mat"), 1, GL_TRUE, packed2.m);
	glBindVertexArray(WWVertexArrayObjID);
	glDrawElements(GL_TRIANGLES, ww->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_b1,"plain_color"),0.5f,0.4f,0.0f,1.0f);
	glUniformMatrix4fv(glGetUniformLocation(pr_b1, "pack_mat"), 1, GL_TRUE, bladeMatrix(0,projectedCam,t_rot).m);
	glBindVertexArray(B1VertexArrayObjID);
	glDrawElements(GL_TRIANGLES, b1->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_b2,"plain_color"),0.5f,0.4f,0.0f,1.0f);
  glUniformMatrix4fv(glGetUniformLocation(pr_b2, "pack_mat"), 1, GL_TRUE, bladeMatrix(1,projectedCam,t_rot).m);
	glBindVertexArray(B2VertexArrayObjID);
	glDrawElements(GL_TRIANGLES, b2->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_b3,"plain_color"),0.5f,0.4f,0.0f,1.0f);
  glUniformMatrix4fv(glGetUniformLocation(pr_b3, "pack_mat"), 1, GL_TRUE, bladeMatrix(2,projectedCam,t_rot).m);
	glBindVertexArray(B3VertexArrayObjID);
	glDrawElements(GL_TRIANGLES, b3->numIndices, GL_UNSIGNED_INT, 0L);

  glUniform4f(glGetUniformLocation(pr_b4,"plain_color"),0.5f,0.4f,0.0f,1.0f);
  glUniformMatrix4fv(glGetUniformLocation(pr_b4, "pack_mat"), 1, GL_TRUE, bladeMatrix(3,projectedCam,t_rot).m);
	glBindVertexArray(B4VertexArrayObjID);
	glDrawElements(GL_TRIANGLES, b4->numIndices, GL_UNSIGNED_INT, 0L);



	printError("display");


	glutSwapBuffers();
}



void OnTimer(int value)

{

    glutPostRedisplay();

    glutTimerFunc(20, &OnTimer, value);

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow ("Gotta Grind Dat Wheat !");
	glutDisplayFunc(display);
    init ();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}



mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot){
  mat4 trans4 = T(0, 9.2, 4.8);	
  mat4 rot4 = Ry(M_PI_2);
  mat4 total4 = Mult(trans4,rot4);
  mat4 br4 = Mult(Rx(i*M_PI_2),time_rot) ;
  total4= Mult(total4,br4);
  mat4 packed4 = Mult(cam, total4);
  return packed4;
}
