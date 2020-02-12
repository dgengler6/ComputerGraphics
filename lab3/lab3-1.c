#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
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
#include <float.h>

// FUnction headers


void buffer_setup(char * in_shader, unsigned int buffer_object, const void * array, GLsizeiptr size , int dim, GLuint prog);
mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot);
void model_to_world_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * result);
void world_to_view_transform(mat4 * projectedCam, mat4 * result);
void packed_transform(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam, mat4 * result);
mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z);
void createModel( Model* result, GLfloat* vArray ,int vASize, GLuint* iArray, int iASize );

void draw_object(vec3 look, vec3 color, mat4 mat, unsigned int id, GLuint pr, Model * m);
void draw_skybox(mat4 mat, unsigned int id, GLuint pr, Model * m);

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
		#define far 100.0
		#define right 0.5
		#define left -0.5
		#define top 0.5
		#define bottom -0.5

		//Creates the differents VBO for the given object
		#define buffer_object(NAME) \
		unsigned int NAME ## VertexBufferObjID; \
		unsigned int NAME ## IndexBufferObjID; \
		unsigned int NAME ## NormalBufferObjID; \
		unsigned int NAME ## TexCoordBufferObjID; \
		glGenBuffers(1, &(NAME ## VertexBufferObjID)); \
		glGenBuffers(1, &(NAME ## IndexBufferObjID)); \
		glGenBuffers(1, &(NAME ## NormalBufferObjID)); \
		glGenBuffers(1, &(NAME ## TexCoordBufferObjID));

		//Create the model based on .obj
		#define buffer_full_setup(MODEL, NAME, MODEL_STRING, PROGRAM) \
		glUseProgram(PROGRAM);\
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

		//Create the model based on user defined arrays
		#define buffer_full_setup_homemade_boi_we_got_this(MODEL, NAME, VERTICES_ARRAY, V_SIZE, INDEXES_ARRAY, I_SIZE, PROGRAM) \
		glUseProgram(PROGRAM);\
		buffer_object(NAME); \
		createModel(MODEL, VERTICES_ARRAY,V_SIZE, INDEXES_ARRAY, I_SIZE); \
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
GLuint pr_wb, pr_wr, pr_ww, pr_b1, pr_b2, pr_b3, pr_b4, pr_g, pr_sb;

// Model and Textures
Model * wb, * wr, * ww, * b, * b1, * b2, * b3 , * b4, * g, * sb;
GLuint sbTexture;

mat4 projectionMatrix;

mat4 camMatrix;

// vertex array object

// Windmill
unsigned int WBVertexArrayObjID, WRVertexArrayObjID, WWVertexArrayObjID, B1VertexArrayObjID,B2VertexArrayObjID,B3VertexArrayObjID, B4VertexArrayObjID;

// Environnement
unsigned int GVertexArrayObjID, SBVertexArrayObjID;


int mouse_x, mouse_y;
int last_mouse_x, last_mouse_y;

int width, height;



vec3 direction;
GLfloat speed;
GLfloat actual_speed;

vec3 p; //= SetVector(15,10,15); // Camera position
vec3 l; //= SetVector(0,5,0); // Look-at point
vec3 v; //= SetVector(0,1,0); //Up vector


//Ground polygon

GLfloat groundVertices[] =
{
	50.0f,0.0f,50.0f,
  -50.0f,0.0f,50.0f,
	50.0f,0.0f,-50.0f,
  -50.0f,0.0f,-50.0f
};

GLuint groundIndex[] = {0,1,2, 1,2,3};


void init(void)
{
	glutPassiveMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);

	// vertex buffer object, used for uploading the geometry
	speed = 0.1;
	actual_speed = speed;

	p = SetVector(15,10,15);
	l = SetVector(0,5,0); // Look-at point
	v = SetVector(0,1,0);

	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");


	// Load and compile shader
	
	pr_wb = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_wr = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_ww = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b1 = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b2 = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b3 = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_b4 = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_g = loadShaders("lab3-1.vert","lab3-1.frag");
	pr_sb = loadShaders("lab3-1sb.vert","lab3-1sb.frag");


	

	printError("init shader");


	// Upload geometry to the GPU:
	buffer_full_setup(wb, WB, "windmill/windmill-balcony.obj", pr_wb);
	buffer_full_setup(wr, WR, "windmill/windmill-roof.obj", pr_wr);
	buffer_full_setup(ww, WW, "windmill/windmill-walls.obj", pr_ww);
	buffer_full_setup(b1, B1, "windmill/blade.obj", pr_b1);
	buffer_full_setup(b2, B2, "windmill/blade.obj", pr_b2);
	buffer_full_setup(b3, B3, "windmill/blade.obj", pr_b3);
	buffer_full_setup(b4, B4, "windmill/blade.obj", pr_b4);
	buffer_full_setup(sb, SB, "skybox.obj", pr_sb);
	


	//LoadModelPlus("windmill/windmill-balcony.obj");

	//malloc Ground
	g = malloc(sizeof(Model));
	memset(g, 0, sizeof(Model));

	buffer_full_setup_homemade_boi_we_got_this(g, G, groundVertices ,3*4, groundIndex, 3*2, pr_g);

	projectionMatrix = frustum(left, right, bottom, top, near, far);
    //glGenBuffers(1, &projectionMatrixBufferObjID);
    // Load and bind the texture
	glUseProgram(pr_sb);
	LoadTGATextureSimple("SkyBox512.tga", &sbTexture);
	glBindTexture(GL_TEXTURE_2D, sbTexture);
	glUniform1i(glGetUniformLocation(pr_sb, "texUnit"), 0);
	//glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);


	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	direction = SetVector(0,0,0);
	actual_speed = speed;

	input_update();

	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
		direction = ScalarMult(Normalize(direction), actual_speed);

	//mat4 look_mat = angle_transform(0, - mouse_x * 2 * M_PI /(float) width, 0); //(0.5f - mouse_y /(float)height) * M_PI
	mat4 look_mat = Mult(Rx((0.5f - mouse_y /(float)height) * M_PI), Ry(- mouse_x * 2 * M_PI /(float) width));

	vec3 look = MultVec3(look_mat, SetVector(0,0,1));

	direction = MultVec3(look_mat, direction);
	p = VectorAdd(p, direction);
	l = VectorAdd(p, look);

	camMatrix = lookAtv(p, l, v);
	mat4 projectedCam = Mult(projectionMatrix, camMatrix);

  // Matrix projections
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	mat4 t_rot = Rx(t/1000);

	//balcony

	mat4 packed;
	packed_transform(SetVector(0,0,0), SetVector(0,-M_PI_2,0), SetVector(1,1,1), &projectedCam, &packed);
	// roof
	mat4 packed1;
	packed_transform(SetVector(0,0.5,0), SetVector(0,0,0), SetVector(1,1,1), &projectedCam, &packed1);
	// walls
	mat4 packed2;
	packed_transform(SetVector(0,0,0), SetVector(0,M_PI,0), SetVector(1,1,1), &projectedCam, &packed2);
	// ground
	mat4 packedg;
	packed_transform(SetVector(0,0,0), SetVector(0,0,0), SetVector(1,1,1), &projectedCam, &packedg);

  //UPLOAD UNIFORM TO SHADERS + DRAW


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//---------------------------------------//
	//             DRAW SKYBOX               //
	//---------------------------------------//
	mat4 camUntranslated = camMatrix;
	{ //SKYBOX COMPUTATIONS
	camUntranslated.m[3] = 0;
	camUntranslated.m[7] = 0;
	camUntranslated.m[11] = 0;
	camUntranslated = Mult(projectionMatrix, camUntranslated);
	//mat4 packedsb;
	//packed_transform(SetVector(0,0,0), SetVector(0,0,0), SetVector(1,1,1), &camUntranslated, &packedsb);
	}
	draw_skybox(camUntranslated, SBVertexArrayObjID, pr_sb, sb);

	//---------------------------------------//
	//             DRAW ALL MODELS           //
	//---------------------------------------//

	glUseProgram(0);
	 // DRAW THERE
	draw_object(look, SetVector(0.5,0.3,0.0), packed, WBVertexArrayObjID, pr_wb, wb);
	draw_object(look, SetVector(1.0,0.0,0.0), packed1, WRVertexArrayObjID, pr_wr, wr);
	draw_object(look, SetVector(1.0,1.0,1.0), packed2, WWVertexArrayObjID, pr_ww, ww);
	draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(0,projectedCam,t_rot), B1VertexArrayObjID, pr_b1, b1);
	draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(1,projectedCam,t_rot), B2VertexArrayObjID, pr_b2, b2);
	draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(2,projectedCam,t_rot), B3VertexArrayObjID, pr_b3, b3);
	draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(3,projectedCam,t_rot), B4VertexArrayObjID, pr_b4, b4);
	draw_object(look, SetVector(0.2,0.7,0.2), packedg, GVertexArrayObjID, pr_g, g);
	


	printError("display");

	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;
	glutSwapBuffers();
}

void draw_skybox(mat4 mat, unsigned int id, GLuint pr, Model * m){
	glDisable(GL_DEPTH_TEST);
	glUseProgram(pr);
	glUniformMatrix4fv(glGetUniformLocation(pr, "pack_mat"), 1, GL_TRUE, mat.m);
	glBindVertexArray(id);    // Select VAO
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void draw_object(vec3 look, vec3 color, mat4 mat, unsigned int id, GLuint pr, Model * m){
	glUseProgram(pr);
	glUniform3f(glGetUniformLocation(pr, "camera_look"), look.x,look.y,look.z);
	glUniform4f(glGetUniformLocation(pr, "plain_color"), color.x, color.y, color.z,1.0f);
	glUniformMatrix4fv(glGetUniformLocation(pr, "pack_mat"), 1, GL_TRUE, mat.m);
	glBindVertexArray(id);
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

}


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutDisplayFunc(display);
	width = 400;
	height = 400;
	glutInitWindowSize(400, 400);
	glutCreateWindow ("Gotta Grind Dat Wheat !");
	

  init ();

  glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
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
	glUseProgram(prog);
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
	return mult_rep(false, 6,S(1,0.7,0.85), time_rot, Rx(i * M_PI_2), Ry(M_PI_2), T(0,9.2,4.8), cam);
}

void createModel( Model* result, GLfloat vArray[] ,int vASize, GLuint iArray[], int iASize ){
	result->numVertices = vASize;
	result->numIndices = iASize;
	result->vertexArray = vArray;
	result->indexArray = iArray;
	result->normalArray = malloc(sizeof(GLfloat) * vASize);
	for(int i = 0 ; i < vASize/3; i+=3){
		result->normalArray[i] = 0.0f;
		result->normalArray[i+1] = 1.0f;
		result->normalArray[i+2] = 0.0f;
	}
  result->texCoordArray = NULL;
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
