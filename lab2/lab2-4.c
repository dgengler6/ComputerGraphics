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

// Model and Textures 
Model *m;
GLuint texture;

#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

                                            0.0f, 0.0f, -1.0f, 0.0f };


// vertex array object
    unsigned int bunnyVertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry

    unsigned int bunnyVertexBufferObjID;

    unsigned int bunnyIndexBufferObjID;

    unsigned int bunnyNormalBufferObjID;
    
    unsigned int projectionMatrixBufferObjID;

    unsigned int bunnyTexCoordBufferObjID;
    

	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);


	printError("GL inits");

	// Load and compile shader

	program = loadShaders("lab2-4.vert","lab2-4.frag");
	printError("init shader");

	// Upload geometry to the GPU:
    m=LoadModel("bunnyplus.obj");

    // Load and bind the texture 
    LoadTGATextureSimple("quack.tga", &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
    glActiveTexture(GL_TEXTURE0);

	// Allocate and activate Vertex Array Object
    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glGenBuffers(1, &bunnyTexCoordBufferObjID);  
    glGenBuffers(1, &projectionMatrixBufferObjID);

    glBindVertexArray(bunnyVertexArrayObjID);

      



    if (m->texCoordArray != NULL)

    {

        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);

        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);

        glVertexAttribPointer(glGetAttribLocation(program, "in_TexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(glGetAttribLocation(program, "in_TexCoord"));

    }

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

    // VBO for projection matrix

    

	// End of upload of geometry
	printError("init arrays");
}


void display(void)
{
	printError("pre display");

    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

    mat4 rot, trans, total;

    trans = T(0, pow(t,1.2)/100000, -3);

    rot = Ry(t*t/1000000);

    total = Mult(  trans,rot);

	glUniformMatrix4fv(glGetUniformLocation(program, "matrix"), 1, GL_TRUE, total.m);
    glUniform1f(glGetUniformLocation(program, "time"), t);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_TRUE, projectionMatrix);

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
