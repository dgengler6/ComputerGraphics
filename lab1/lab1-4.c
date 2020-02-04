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

// Globals
// Data would normally be read from files

// Reference to shader program
	GLuint program;

GLfloat vertices[] =
{
	0.5f,0.5f,0.0f,
	-0.5f,0.5f,0.0f,
	0.5f,-0.5f,0.0f
};

GLfloat colors[] =
{
	0.3f,0.3f,1.0f,
	0.3f,1.0f,1.0f,
	1.f,0.4f,1.0f
};

// vertex array object
unsigned int vertexArrayObjID;
void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vertexBufferObjIDColor;

	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);

	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader

	program = loadShaders("lab1-4.vert","lab1-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &vertexBufferObjIDColor);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDColor);
	glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	// clear the screen

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

  mat4 rz = Rz(t/1000);

	glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, rz.m);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0,3);
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
    glEnable(GL_CULL_FACE);
    glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
