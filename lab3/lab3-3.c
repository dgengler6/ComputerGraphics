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

typedef struct material {
	float k_d;   //Diffuse
	float k_spec;//Specular
	float ex;
	vec3 color;
} material;

// FUnction headers

mat4 bladeMatrix(int i, mat4 cam, mat4 time_rot);
mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z);
mat4 model_to_world(const vec3 t, const vec3 r, const vec3 s);
mat4 model_to_view(const vec3 t, const vec3 r, const vec3 s, mat4 * camMatrix);
mat4 model_to_proj(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam);

void camera_movement(float alpha, float beta);
void draw_object(vec3 look, vec3 color, mat4 mat, GLuint shader, Model * m);
void draw_object1(GLuint shader, Model * m, material mater, mat4 mw);
void draw_skybox(mat4 mat, GLuint shader, Model * m);

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

		#define near 1.0
		#define far 400.0
		#define right 0.5
		#define left -0.5
		#define top 0.5
		#define bottom -0.5

// Globals
// Data would normally be read from files

// Reference to shader program
GLuint shader1, shader2;

// Model and Textures
Model * wb, * wr, * ww, * b,* g, * sb, * r;
GLuint sbTexture;

mat4 projectionMatrix;

mat4 camMatrix;

// vertex array object

int mouse_x, mouse_y;
int last_mouse_x, last_mouse_y;

int width, height;

vec3 direction; vec3 look;
GLfloat speed;
GLfloat actual_speed;

vec3 p; //= SetVector(15,10,15); // Camera position
vec3 l; //= SetVector(0,5,0); // Look-at point
vec3 v; //= SetVector(0,1,0); //Up vector

material wood = 			{0.4f, 0.3f, 50.0f, {0.5,0.3,0.0}};
material blade_wood = {0.5f, 0.4f, 60.0f, {0.5,0.4,0.0}};
material stone = 			{0.6f, 0.05f, 20.0f, {1.0,1.0,1.0}};
material brick = 			{0.5f, 0.1f, 20.0f, {1.0,0.0,0.0}};
material ground = 		{0.6f, 0.0f, 10.0f, {0.2,0.7,0.2}};
material metal =      {0.3f, 1.0f, 20.0f, {0.7,0.7,0.7}};
//Ground polygon

GLfloat groundVertices[] =
{
	50.0f,0.0f,50.0f,
  -50.0f,0.0f,50.0f,
	50.0f,0.0f,-50.0f,
  -50.0f,0.0f,-50.0f
};

#define light_nb 3

vec4 dir_lights[] = {
	{1,0,1,1},
	{1,0,0,0.5},
	{0,1,0,0.5}
};


vec4 ambient = {1,1,1,0.7 }; //{RGB,I}

GLfloat lights[3 * light_nb];

GLuint groundIndex[] = {0,1,2, 1,2,3};

vec3 lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

                                 {0.0f, 1.0f, 0.0f}, // Green light

                                 {0.0f, 0.0f, 1.0f}, // Blue light

                                 {1.0f, 1.0f, 1.0f} }; // White light

GLint isDirectional[] = {0,0,1,1};


vec3 lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

                                       {0.0f, 5.0f, 10.0f}, // Green light, positional

                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X

                                       {0.0f, 0.0f, -1.0f} }; // White light along Z


void init(void)
{
	glutPassiveMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);

	// vertex buffer object, used for uploading the geometry
	speed = 0.3;
	actual_speed = speed;

	p = SetVector(15,10,15);
	l = SetVector(0,5,0); // Look-at point
	v = SetVector(0,1,0);

	for (int i = 0; i < light_nb; i++) {
			vec3 l = SetVector(dir_lights[i].x, dir_lights[i].y, dir_lights[i].z);
			vec3 temp = ScalarMult(Normalize(l), dir_lights[i].w);
			lights[3*i]   = temp.x;
			lights[3*i+1] = temp.y;
			lights[3*i+2] = temp.z;
	}



	dumpInfo();

	// GL inits
	glClearColor(1,0.2,1,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");


	// Load and compile shader
	shader1 = loadShaders("light_colored.vert","light_colored.frag");
	shader2 = loadShaders("lab3-1sb.vert","lab3-1sb.frag");

	printError("init shader");

	// Upload geometry to the GPU:

	wb = LoadModelPlus("windmill/windmill-balcony.obj");
	wr = LoadModelPlus("windmill/windmill-roof.obj");
	ww = LoadModelPlus("windmill/windmill-walls.obj");
	b = LoadModelPlus("windmill/blade.obj");
	sb = LoadModelPlus("skybox.obj");
	r = LoadModelPlus("bunny.obj");
	g = LoadDataToModel(groundVertices, NULL, NULL, NULL, groundIndex, 3*4, 3*2);


	projectionMatrix = frustum(left, right, bottom, top, near, far);

    // Load and bind the texture
  LoadTGATextureSimple("SkyBox512.tga", &sbTexture);
  glBindTexture(GL_TEXTURE_2D, sbTexture);
  glUniform1i(glGetUniformLocation(shader2, "texUnit"), 0);
  glActiveTexture(GL_TEXTURE0);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	direction = SetVector(0,0,0);
	actual_speed = speed;
	float alpha = mouse_x * 2 * M_PI /(float)width;
	float beta = - (0.5f - mouse_y /(float)height) * M_PI;

	input_update();

	camera_movement(alpha, beta);

	//mat4 projectedCam = Mult(projectionMatrix, camMatrix);

  // Matrix projections
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	mat4 t_rot = Rx(t/1000);

	//balcony
	mat4 wb_mw = model_to_world(SetVector(0,0,0), SetVector(0,-M_PI_2,0), SetVector(1,1,1));
	// roof
	mat4 wr_mw = model_to_world(SetVector(0,0.4,0), SetVector(0,0,0), SetVector(1,1,1));
	// walls
	mat4 ww_mw = model_to_world(SetVector(0,0,0), SetVector(0,M_PI,0), SetVector(1,1,1));
	// ground
	mat4 g_mw = model_to_world(SetVector(0,0,0), SetVector(0,0,0), SetVector(1,1,1));

	mat4 b1_mw = bladeMatrix(0 , IdentityMatrix(), t_rot);
	mat4 b2_mw = bladeMatrix(1 , IdentityMatrix(), t_rot);
	mat4 b3_mw = bladeMatrix(2 , IdentityMatrix(), t_rot);
	mat4 b4_mw = bladeMatrix(3 , IdentityMatrix(), t_rot);

  //UPLOAD UNIFORM TO SHADERS + DRAW

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---------------------------------------//
	//             DRAW SKYBOX               //
	//---------------------------------------//

	mat4 camUntranslated = camMatrix;
	 //SKYBOX COMPUTATIONS
	camUntranslated.m[3] = 0;
	camUntranslated.m[7] = 0;
	camUntranslated.m[11] = 0;
	camUntranslated = Mult(projectionMatrix, camUntranslated);

	draw_skybox(camUntranslated, shader2, sb);

	//---------------------------------------//
	//             DRAW ALL MODELS           //
	//---------------------------------------//

	// draw_object(look, SetVector(0.5,0.3,0.0), packed, shader1, wb);
	// draw_object(look, SetVector(1.0,0.0,0.0), packed1, shader1, wr);
	// draw_object(look, SetVector(1.0,1.0,1.0), packed2, shader1, ww);
	// draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(0,projectedCam,t_rot), shader1, b);
	// draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(1,projectedCam,t_rot), shader1, b);
	// draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(2,projectedCam,t_rot), shader1, b);
	// draw_object(look, SetVector(0.5,0.4,0.0), bladeMatrix(3,projectedCam,t_rot), shader1, b);
	// draw_object(look, SetVector(0.2,0.7,0.2), packedg, shader1, g);

	// draw_object1(shader1, wb, wood, wb_mw);
	draw_object1(shader1, wb, metal, wb_mw);
	draw_object1(shader1, wr, brick, wr_mw);
	draw_object1(shader1, ww, stone, ww_mw);
	// draw_object1(shader1, b, blade_wood, b1_mw);
	// draw_object1(shader1, b, blade_wood, b2_mw);
	// draw_object1(shader1, b, blade_wood, b3_mw);
	// draw_object1(shader1, b, blade_wood, b4_mw);
	draw_object1(shader1, b, metal, b1_mw);
	draw_object1(shader1, b, metal, b2_mw);
	draw_object1(shader1, b, metal, b3_mw);
	draw_object1(shader1, b, metal, b4_mw);
	draw_object1(shader1, g, ground, g_mw);

	mat4 r_mw;
	material lapinoux;
	lapinoux.ex = 20.0f;
	lapinoux.k_d = 0.5;
	lapinoux.k_spec= 0.1f;
	for(int i = 0; i< 8;i++){
		r_mw = Mult(Ry(i*M_PI_4 - t/1000),T(8 + sin(t/1000),2+sin(t/100 + i),0));
		lapinoux.color = SetVector(0.5,0.1*i,0.1);
		draw_object1(shader1, r, lapinoux, r_mw);
	}
	

	printError("display");

	last_mouse_x = mouse_x;
	last_mouse_y = mouse_y;
	glutSwapBuffers();
}

void camera_movement(float alpha, float beta){

	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
		direction = ScalarMult(Normalize(direction), actual_speed);

	mat4 look_mat = Mult(Rx(beta), Ry(alpha));
	look = MultVec3(Transpose(look_mat), SetVector(0,0,1));

	direction = MultVec3(Transpose(look_mat), direction);
	p = VectorSub(p, direction);

	camMatrix = Mult(look_mat, T(-p.x, -p.y, -p.z));
}

void draw_skybox(mat4 mat, GLuint shader, Model * m){
	glDisable(GL_DEPTH_TEST);
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "pack_mat"), 1, GL_TRUE, mat.m);
	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	DrawModel(m, shader, "in_Position", "in_Normal", "in_TexCoord");

	glEnable(GL_DEPTH_TEST);
}

void draw_object(vec3 look, vec3 color, mat4 mat, GLuint shader, Model * m){
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "camera_look"), look.x,look.y,look.z);
	glUniform4f(glGetUniformLocation(shader, "plain_color"), color.x, color.y, color.z, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "pack_mat"), 1, GL_TRUE, mat.m);
	DrawModel(m, shader, "in_Position", "in_Normal", "in_TexCoord");
}

void draw_object1(GLuint shader, Model * m, material mater, mat4 mw){
	glUseProgram(shader);
	glUniform1f(glGetUniformLocation(shader, "m.k_d"), mater.k_d);
	glUniform1f(glGetUniformLocation(shader, "m.k_spec"), mater.k_spec);
	glUniform1f(glGetUniformLocation(shader, "m.ex"), mater.ex);
	glUniform4f(glGetUniformLocation(shader, "m.color"), mater.color.x,mater.color.y,mater.color.z, 1);
	glUniform4f(glGetUniformLocation(shader, "ambient"), ambient.x,ambient.y,ambient.z, ambient.w);

	glUniform1i(glGetUniformLocation(shader, "count"), light_nb);
	glUniform3fv(glGetUniformLocation(shader, "lights"), 3 * light_nb, lights);
	glUniformMatrix4fv(glGetUniformLocation(shader, "mw"), 1, GL_TRUE, mw.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "wv"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(shader, "vp"), 1, GL_TRUE, projectionMatrix.m);
	DrawModel(m, shader, "in_Position", "in_Normal", NULL);
}

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	width = 700;
	height = 700;
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Gotta Grind Dat Wheat !");
	glutDisplayFunc(display);

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

	if (glutKeyIsDown(DOWNKEY))
		direction.y += 1;

	if (glutKeyIsDown(UPKEY))
		direction.y -= 1;

	if (glutKeyIsDown('o'))
		actual_speed *= 2;

	if (glutKeyIsDown('p'))
		actual_speed /= 2;
}

mat4 model_to_world(const vec3 t, const vec3 r, const vec3 s){
	return mult_rep(false, 3 , S(s.x,s.y,s.z), angle_transform(r.x, r.y, r.z), T(t.x, t.y, t.z));
}

mat4 model_to_view(const vec3 t, const vec3 r, const vec3 s, mat4 * camMatrix){
	return Mult( *camMatrix, model_to_world(t,r,s));
}

mat4 model_to_proj(const vec3 t, const vec3 r, const vec3 s, mat4 * projectedCam) {
	return Mult(* projectedCam, model_to_world(t, r, s));
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
