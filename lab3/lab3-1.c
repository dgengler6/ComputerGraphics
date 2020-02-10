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


// Globals
// Data would normally be read from files

// Reference to shader program
GLuint program;

// Model and Textures
Model * wb, * wr, * ww, * b;
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

#define buffer_full_setup(MODEL, NAME, MODEL_STRING) \
	buffer_object(NAME); \
	MODEL=LoadModel(MODEL_STRING); \
	glGenVertexArrays(1, &(NAME ## VertexArrayObjID)); \
	glBindVertexArray(NAME ## VertexArrayObjID); \
	if (MODEL->texCoordArray != NULL) \
		buffer_setup("in_TexCoord", NAME ## TexCoordBufferObjID, MODEL->texCoordArray, MODEL->numVertices, 2); \
	buffer_setup("in_Position", NAME ## VertexBufferObjID, MODEL->vertexArray, MODEL->numVertices, 3); \
	buffer_setup("in_Normal", NAME ## NormalBufferObjID, MODEL->normalArray, MODEL->numVertices, 3); \
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NAME ## IndexBufferObjID); \
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MODEL->numIndices*sizeof(GLuint), MODEL->indexArray, GL_STATIC_DRAW);

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim);

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim){
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
	glBufferData(GL_ARRAY_BUFFER, size*dim*sizeof(GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, in_shader), dim, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, in_shader));
}

mat4 projectionMatrix;

mat4 camMatrix;

// vertex array object
    unsigned int WBVertexArrayObjID, WRVertexArrayObjID, WWVertexArrayObjID, BVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry

  unsigned int projectionMatrixBufferObjID;
	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);
	printError("GL inits");
	// Load and compile shader
	program = loadShaders("lab3-1.vert","lab3-1.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	buffer_full_setup(wb, WB, "windmill/windmill-balcony.obj");
	buffer_full_setup(wr, WR, "windmill/windmill-roof.obj");
	buffer_full_setup(ww, WW, "windmill/windmill-walls.obj");
	buffer_full_setup(b, B, "windmill/blade.obj");

		projectionMatrix = frustum(left, right, bottom, top, near, far);

		vec3 p = SetVector(0,0,-1);
		vec3 l = SetVector(0,0,-3);
		vec3 v = SetVector(0,1,0);
		camMatrix = lookAtv(p, l, v);

    // Load and bind the texture
    LoadTGATextureSimple("quack.tga", &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
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

    mat4 rot, trans, total;

    trans = T(0, 0, -3);

    //rot = Ry(t*t/1000000);
		rot = Ry(t/1000);
		mat4 projectedCam = Mult(projectionMatrix, camMatrix);
    total = Mult(trans,rot);
		mat4 packed = Mult(projectedCam, total);

    glUniform1f(glGetUniformLocation(program, "time"), t);
		glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, total.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "camera"), 1, GL_TRUE, camMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "vingthuit"), 1, GL_TRUE, packed.m);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(WBVertexArrayObjID);    // Select VAO
	glBindVertexArray(WRVertexArrayObjID);
	glBindVertexArray(WWVertexArrayObjID);
	glBindVertexArray(BVertexArrayObjID);

    glDrawElements(GL_TRIANGLES, wb->numIndices, GL_UNSIGNED_INT, 0L);
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
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
    init ();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
