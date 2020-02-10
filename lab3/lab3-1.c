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
GLuint pr_wb, pr_wr, pr_ww, pr_b;

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

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog);
void model_to_world_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * result);
void world_to_view_transform(mat4 * projectedCam, mat4 * result);
void packed_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam, mat4 * result);
void euler_transform(GLfloat x, GLfloat y, GLfloat z, mat4 * result);
mat4 Mat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
			GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
			GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11,
			GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
		);

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog){
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
	glBufferData(GL_ARRAY_BUFFER, size*dim*sizeof(GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(prog, in_shader), dim, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog, in_shader));
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
	pr_wb = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_wr = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_ww = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b = loadShaders("lab3-1.vert","lab3-1.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	buffer_full_setup(wb, WB, "windmill/windmill-balcony.obj", pr_wb);
	buffer_full_setup(wr, WR, "windmill/windmill-roof.obj", pr_wr);
	buffer_full_setup(ww, WW, "windmill/windmill-walls.obj", pr_ww);
	buffer_full_setup(b, B, "windmill/blade.obj", pr_b);

		projectionMatrix = frustum(left, right, bottom, top, near, far);

		vec3 p = SetVector(0,0,10);
		vec3 l = SetVector(0,0,-3);
		vec3 v = SetVector(0,1,0);
		camMatrix = lookAtv(p, l, v);

    // Load and bind the texture
    LoadTGATextureSimple("SkyBox512.tga", &texture);
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

		mat4 projectedCam = Mult(projectionMatrix, camMatrix);

	mat4 packed, packed1;

	packed_transform(SetVector(0,0,-2), SetVector(0,0,t/1000), SetVector(1,1,1), &projectedCam, &packed);
	packed_transform(SetVector(0,0,-2), SetVector(0,0,0), SetVector(1,1,1), &projectedCam, &packed1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(glGetUniformLocation(pr_wb, "pack_mat"), 1, GL_TRUE, packed.m);
	glBindVertexArray(WBVertexArrayObjID);    // Select VAO
	glDrawElements(GL_TRIANGLES, wb->numIndices, GL_UNSIGNED_INT, 0L);
	glUniformMatrix4fv(glGetUniformLocation(pr_wr, "pack_mat"), 1, GL_TRUE, packed.m);
	glBindVertexArray(WRVertexArrayObjID);
	glDrawElements(GL_TRIANGLES, wr->numIndices, GL_UNSIGNED_INT, 0L);
	glUniformMatrix4fv(glGetUniformLocation(pr_ww, "pack_mat"), 1, GL_TRUE, packed.m);
	glBindVertexArray(WWVertexArrayObjID);
	glDrawElements(GL_TRIANGLES, ww->numIndices, GL_UNSIGNED_INT, 0L);
	glUniformMatrix4fv(glGetUniformLocation(pr_b, "pack_mat"), 1, GL_TRUE, packed1.m);
	glBindVertexArray(BVertexArrayObjID);
	glDrawElements(GL_TRIANGLES, b->numIndices, GL_UNSIGNED_INT, 0L);


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

void model_to_world_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * result){
	mat4 rot;
	euler_transform(r.x, r.y, r.z, &rot);
	* result = Mult(rot, S(s.x,s.y,s.z));
	* result = Mult(T(t.x, t.y, t.z),*result);
}
void world_to_view_transform(mat4 * projectedCam, mat4 * result){
	* result = Mult(* projectedCam, * result);
}

mat4 Mat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
			GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
			GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11,
			GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
			)
{
	mat4 m;
	m.m[0] = p0;
	m.m[1] = p1;
	m.m[2] = p2;
	m.m[3] = p3;
	m.m[4] = p4;
	m.m[5] = p5;
	m.m[6] = p6;
	m.m[7] = p7;
	m.m[8] = p8;
	m.m[9] = p9;
	m.m[10] = p10;
	m.m[11] = p11;
	m.m[12] = p12;
	m.m[13] = p13;
	m.m[14] = p14;
	m.m[15] = p15;
	return m;
}

void packed_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam, mat4 * result) {
	model_to_world_transform(t, r, s, result);
	world_to_view_transform(projectedCam, result);
}

void euler_transform(GLfloat x, GLfloat y, GLfloat z, mat4 * result) {
	GLfloat c1 = cos(z);
	GLfloat s1 = sin(z);
	GLfloat c2 = cos(y);
	GLfloat s2 = sin(y);
	GLfloat c3 = cos(x);
	GLfloat s3 = sin(x);

	mat4 rot = Mat4(c2*c3, -c2*s3, s2, 0,
									c1*s3 + c3*s1*s2, c1*c3 - s1*s2*s3, -c2*s1, 0,
									s1*s3 - c1*c3*s2, c3*s1 + c1*s2*s3, c1*c2 , 0,
									0, 0 , 0, 1
									 );
  * result = rot;
}
