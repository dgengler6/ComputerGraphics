// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.

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
	0.5f,-0.5f,0.0f,

    -0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,
    -0.5f,0.5f,0.0f,

    -0.5f,0.5f,0.0f,
    0.5f,0.5f,0.0f,
    0.0f,0.0f,0.5f,

    -0.5f,-0.5f,0.0f,
    -0.5f,0.5f,0.0f,
    0.0f,0.0f,0.5f,

    0.5f,-0.5f,0.0f,
    -0.5f,-0.5f,0.0f,
    0.0f,0.0f,0.5f,

    0.5f,0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.0f,0.5f


};

GLfloat colors[] =
{
	1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
	1.f,0.0f,0.0f,
	1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,
    1.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f,
    1.0f,1.0f,1.0f
};

GLfloat translationMatrix[] = {    1.0f, 0.0f, 0.0f, 0.5f,

                        0.0f, 1.0f, 0.0f, 0.0f,

                        0.0f, 0.0f, 1.0f, 0.0f,

                        0.0f, 0.0f, 0.0f, 1.0f };






// vertex array object
unsigned int vertexArrayObjID;
void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	unsigned int vertexBufferObjIDColor;
	unsigned int vertexBufferObjIDrotationMatrix;


	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);

	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader

	program = loadShaders("lab1-5.vert","lab1-5.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);


	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjIDColor);


	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 3*18*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDColor);
	glBufferData(GL_ARRAY_BUFFER, 3*18*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjIDrotationMatrix);
	//glBufferData(GL_ARRAY_BUFFER, 16*sizeof(GLfloat), rotationMatrix, GL_STATIC_DRAW);
	//glVertexAttribPointer(glGetAttribLocation(program, "in_rotationMatrix"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(glGetAttribLocation(program, "in_rotationMatrix"));

    //glUniformMatrix4fv(glGetUniformLocation(program, "translationMatrix"), 1, GL_TRUE, translationMatrix);

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");
	//glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, rotationMatrix);
	// clear the screen

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	GLfloat rotationMatrix[] = {   cos(t), -sin(t), 0.0f, 0.0f,

						sin(t), cos(t), 0.0f, 0.0f,

						0.0f, 0.0f , 1.0f , 0.0f,

						0.0f, 0.0f, 0.0f, 1.0f };

    mat4 rotx, rotz, roty, trans, total;

	rotz = Rz(t/1000);
    rotx = Rx(t/500);
    roty = Ry(t/1000);
	total = Mult(Mult(rotx, rotz),roty);


	glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, total.m);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0,3);
    glDrawArrays(GL_TRIANGLES, 3,3);
    glDrawArrays(GL_TRIANGLES, 6,3);
    glDrawArrays(GL_TRIANGLES, 9,3);
    glDrawArrays(GL_TRIANGLES, 12,3);
    glDrawArrays(GL_TRIANGLES, 15,3);	// draw object
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
