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
#include "loadobj.h"


// Globals
// Data would normally be read from files

// Reference to shader program
	GLuint program;

Model *m;
GLfloat t;

// vertex array object
    unsigned int bunnyVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry

    unsigned int bunnyVertexBufferObjID;

    unsigned int bunnyIndexBufferObjID;

    unsigned int bunnyNormalBufferObjID;



	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);


	printError("GL inits");

	// Load and compile shader

	program = loadShaders("lab1-bonus.vert","lab1-bonus.frag");
	printError("init shader");

	// Upload geometry to the GPU:
    m=LoadModel("bunny.obj");
	// Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);

    glBindVertexArray(bunnyVertexArrayObjID);



    // VBO for vertex data

    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);

    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));



    // VBO for normal data

    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);

    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    mat4 rotx, roty2, rotz, roty, trans, s, total;

	rotz = Rz(t/1000 * sin(t/1000) + 180);
    rotx = Rx(t/1000);
    roty = Ry(t/1000-90);
    trans = T(sin(t/1000),0,0);
    s = S(sin(t/700) + 1, sin(t/600) + 1, sin(t/500) + 1);
	total = Mult(Mult(Mult(Mult(rotx, rotz),roty),trans),s);

    roty2 = Ry(t/1000);


	glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, roty2.m);
	glUniform1f(glGetUniformLocation(program, "time"), t);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO

    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
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
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
    init ();
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
