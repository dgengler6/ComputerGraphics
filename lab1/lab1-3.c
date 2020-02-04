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
	0.7f,-0.2f,0.0f,
	-0.0f,0.7f,0.0f,
	-0.7f,-0.2f,0.0f
};

// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader

	program = loadShaders("lab1-3.vert","lab1-3.frag");
	printError("init shader");


	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	t=t/1000;

	GLfloat rotationMatrix[] = {
		cos(t),-sin(t), 0.0f, 0.0f,
		sin(t), cos(t), 0.0f, 0.0f,
		0.0f  , 0.0f  , 1.0f, 0.0f,
		0.0f  , 0.0f  , 0.0f, 1.0f };

	glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);

	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0,3);	// draw object
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

	glutTimerFunc(20, &OnTimer, 0);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();

	glutMainLoop();
	return 0;
}
