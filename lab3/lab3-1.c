#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>
  #define RIGHTKEY 'd'
  #define LEFTKEY 'q'
  #define DOWNKEY 'a'
  #define UPKEY 'e'
  #define FORWARDKEY 'z'
  #define BACKKEY 's'
#else 
  #define RIGHTKEY 'd'
  #define LEFTKEY 'a'
  #define DOWNKEY 'q'
  #define UPKEY 'e'
  #define FORWARDKEY 'w'
  #define BACKKEY 's'
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"
#include <stdarg.h>

// FUnction headers


void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog);
mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot);
void model_to_world_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * result);
void world_to_view_transform(mat4 * projectedCam, mat4 * result);
void packed_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam, mat4 * result);
mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z);

void input_update(void);
void mouse_motion (int x, int y);
void special_key_func(unsigned char key, int x, int y);
void reshape(int w, int h);

void advanced_mult_rep(mat4 * result, int i, int n, ...);
mat4 mult_rep(bool from_left, int n, ...);

mat4 Mat4(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,
			GLfloat p4, GLfloat p5, GLfloat p6, GLfloat p7,
			GLfloat p8, GLfloat p9, GLfloat p10, GLfloat p11,
			GLfloat p12, GLfloat p13, GLfloat p14, GLfloat p15
		);

	//Macro
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

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint pr_wb, pr_wr, pr_ww, pr_b1, pr_b2, pr_b3, pr_b4;

// Model and Textures
Model * wb, * wr, * ww, * b, * b1, * b2, * b3 , * b4;
GLuint texture;

mat4 projectionMatrix;

mat4 camMatrix;

// vertex array object
unsigned int WBVertexArrayObjID, WRVertexArrayObjID, WWVertexArrayObjID, B1VertexArrayObjID,B2VertexArrayObjID,B3VertexArrayObjID,B4VertexArrayObjID;

int mouse_x, mouse_y;
int last_mouse_x, last_mouse_y;

int width, height;

vec3 direction;
GLfloat speed;
GLfloat actual_speed;

vec3 p; //= SetVector(15,10,15); // Camera position
vec3 l; //= SetVector(0,5,0); // Look-at point
vec3 v; //= SetVector(0,1,0); //Up vector

void init(void)
{
	//glutSpecialFunc(special_key_func);
	//glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);

	// vertex buffer object, used for uploading the geometry
	speed = 0.1;
	actual_speed = speed;

	p = SetVector(15,10,15);
	l = SetVector(0,5,0); // Look-at point
	v = SetVector(0,1,0);

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
	printf("%f, %f\n", mouse_x/(float)width, mouse_y/(float)height);
	printError("pre display");

	direction = SetVector(0,0,0);
	actual_speed = speed;

	input_update();

	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
		direction = ScalarMult(Normalize(direction), actual_speed);



	mat4 look_mat = angle_transform(0, - mouse_x * 2 * M_PI /(float) width, (0.5f - mouse_y /(float)height) * M_PI);
	vec3 look = MultVec3(look_mat, SetVector(0,0,1));
	direction = MultVec3(look_mat, direction);
	p = VectorAdd(p, direction);
	l = VectorAdd(p, look);
	camMatrix = lookAtv(p, l, v);

  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
  mat4 t_rot = Rx(t/1000);

	mat4 projectedCam = Mult(projectionMatrix, camMatrix);
  //balcony
  mat4 packed;
	packed_transform(SetVector(0,0,0), SetVector(0,-M_PI_2,0), SetVector(1,1,1), &projectedCam, &packed);
  // roof
  mat4 packed1;
	packed_transform(SetVector(0,0.5,0), SetVector(0,0,0), SetVector(1,1,1), &projectedCam, &packed1);
  // walls
  mat4 packed2;
	packed_transform(SetVector(0,0,0), SetVector(0,M_PI,0), SetVector(1,1,1), &projectedCam, &packed2);
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


	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;
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
	width = 400;
	height = 400;
	glutInitWindowSize(400, 400);
	glutCreateWindow ("Gotta Grind Dat Wheat !");
	glutDisplayFunc(display);
    init ();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}



void special_key_func(unsigned char key, int x, int y){
	printf("%d\n", (int) key);
	/*switch (key) {
		case GLUT_KEY_CONTROL:
		break;
		case GLUT_KEY_LEFT_SHIFT:
		break;
	}*/
}

void reshape(int w, int h){
	width = w;
	height = h;
}

void mouse_motion (int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void input_update(void){

	if (glutKeyIsDown(FORWARDKEY))
		direction.z += 1;

	if (glutKeyIsDown(BACKKEY))
		direction.z -= 1;

	if (glutKeyIsDown(LEFTKEY))
		direction.x += 1;

	if (glutKeyIsDown(RIGHTKEY))
		direction.x -= 1;

	if (glutKeyIsDown(UPKEY))
		direction.y += 1;

	if (glutKeyIsDown(DOWNKEY))
		direction.y -= 1;

	if (glutKeyIsDown('o'))
		actual_speed *= 2;

	if (glutKeyIsDown('p'))
		actual_speed /= 2;

}

void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog){
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
	glBufferData(GL_ARRAY_BUFFER, size*dim*sizeof(GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(prog, in_shader), dim, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog, in_shader));
}

void model_to_world_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * result){
	* result = Mult(angle_transform(r.x, r.y, r.z), S(s.x,s.y,s.z));
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

mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z) {
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
  return rot;
}

mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot){
	return mult_rep(false, 5, time_rot, Rx(i * M_PI_2), Ry(M_PI_2), T(0,9.2,4.8), cam);
}

/**
* @param result: if you want to put a base Matrix, and also the result matrix
* @param i:			 where you want to put the result base matrix in the multiplication, -1 if you don't want do it
* @param n:			 number of matrix you want to multiply
**/
void advanced_mult_rep(mat4 * result, int i, int n, ...){
	va_list valist;
	va_start(valist, n);

	if (i == -1) * result = IdentityMatrix();
	int k = 0;
	mat4 below = IdentityMatrix();

	while (k++ <= n) {
		if (k < i)
			below = Mult((mat4) va_arg(valist, mat4), below);
		else if (k > i)
			* result = Mult((mat4) va_arg(valist, mat4), * result);
		else
			* result = Mult(* result, below);
	}

	va_end(valist);
}

mat4 mult_rep(bool from_left, int n, ...){
	va_list valist;
	va_start(valist, n);

	mat4 result = IdentityMatrix();

	if (from_left)
		while (n-- > 0)
			result = Mult(result, (mat4) va_arg(valist, mat4));
	else
		while (n-- > 0)
			result = Mult((mat4) va_arg(valist, mat4), result);

	va_end(valist);
	return result;
}
