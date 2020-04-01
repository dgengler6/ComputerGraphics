// Lab 4, terrain generation

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

#define ACTIONKEY 'r'

#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include "loadobj.h"
#include "LoadTGA.h"
#include <stdarg.h>

mat4 projectionMatrix;
mat4 camMatrix;

int mouse_x, mouse_y;
int last_mouse_x, last_mouse_y;

vec3 direction; vec3 look;
GLfloat speed;
GLfloat actual_speed;

vec3 p;

int width, height;

vec3 debug_pos = {0.0,0.0,0.0};
GLfloat debug_val = 0.1;

void mouse_motion (int x, int y);
void input_update(void);
void camera_movement(float alpha, float beta);
mat4 mult_rep(bool from_left, int n, ...);
mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z);
mat4 model_to_world(const vec3 t, const vec3 r, const vec3 s);

bool check_border(int index, int width){
	return (index >=0 && ((index + 1) % width)!=0 )|| index == 0;
}

bool is_first_triangle(float x, float z, vec3 v1, vec3 v4 ){
	 GLfloat d1x = x - v1.x;
	 GLfloat d1z = z - v1.z;

	 GLfloat d2x = v4.x - x;
	 GLfloat d2z = v4.z - z;

	 GLfloat dist = sqrt(d1x * d1x + d1z * d1z);
	 GLfloat dist2 = sqrt(d2x * d2x + d2z * d2z);

	 return dist < dist2;
}

GLfloat height_from_triangle(vec3 a, vec3 ab, vec3 n, GLfloat d){
	GLfloat mu = (-d - DotProduct(n,a))/DotProduct(n, ab);
	return a.y + mu * ab.y;
}

bool find_height(float x, float z, Model* terrain, TextureData *tex){

    int xf = (int)(floor(x));

    int zf = (int)(floor(z));
		zf = zf >= 0 ? zf : 0;
		zf = zf < tex->height - 1 ? zf : tex->height - 2;

		xf = xf >= 0 ? xf : 0;
		xf = xf < tex->width - 1 ? xf : tex->width - 2;

		int index = (xf + zf * (tex->width-1))*6;

		int top_left = terrain->indexArray[index + 0];
		int bot_left = terrain->indexArray[index + 1];
		int top_right = terrain->indexArray[index + 2];
		int bot_right = terrain->indexArray[index + 5];

		vec3 v1 = SetVector(terrain->vertexArray[top_left*3], terrain->vertexArray[top_left*3 + 1], terrain->vertexArray[top_left*3 + 2]);
		vec3 v2 = SetVector(terrain->vertexArray[bot_left*3], terrain->vertexArray[bot_left*3 + 1], terrain->vertexArray[bot_left*3 + 2]);
		vec3 v3 = SetVector(terrain->vertexArray[top_right*3], terrain->vertexArray[top_right*3 + 1], terrain->vertexArray[top_right*3 + 2]);
		vec3 v4 = SetVector(terrain->vertexArray[bot_right*3], terrain->vertexArray[bot_right*3 + 1], terrain->vertexArray[bot_right*3 + 2]);

		vec3 n = SetVector(0,0,0);
		GLfloat d = 0;
		if (is_first_triangle(x,z,v1,v4)) {
			n = CalcNormalVector(v1, v2, v3);
			d = - DotProduct(n, v1);
		} else {
			n = CalcNormalVector(v4, v3, v2);
			d = - DotProduct(n, v4);
		}

		debug_pos.y = height_from_triangle(SetVector(x,0,z), SetVector(0,1,0), n, d);

		return true;
}

Model* GenerateTerrain(TextureData *tex)
{

	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLfloat *normalTriangleArray = malloc(sizeof(GLfloat) * 3 * triangleCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 50.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{

			int top_left = x + z * tex->width;
			int bot_left = x + (z+1) * tex->width;
			int top_right = x+1 + z * tex->width;
			int bot_right = x+1 + (z+1) * tex->width;

			int index = (x + z * (tex->width-1))*6;

		// Triangle 1
			indexArray[index + 0] = top_left;
			indexArray[index + 1] = bot_left;
			indexArray[index + 2] = top_right;

		// Triangle 2
			indexArray[index + 3] = top_right;
			indexArray[index + 4] = bot_left;
			indexArray[index + 5] = bot_right;


			vec3 v1 = SetVector(vertexArray[top_left*3], vertexArray[top_left*3 + 1], vertexArray[top_left*3 + 2]);
			vec3 v2 = SetVector(vertexArray[bot_left*3], vertexArray[bot_left*3 + 1], vertexArray[bot_left*3 + 2]);
			vec3 v3 = SetVector(vertexArray[top_right*3], vertexArray[top_right*3 + 1], vertexArray[top_right*3 + 2]);
			vec3 v4 = SetVector(vertexArray[bot_right*3], vertexArray[bot_right*3 + 1], vertexArray[bot_right*3 + 2]);

			vec3 norm1 = CalcNormalVector(v1,v2,v3);
			vec3 norm2 = CalcNormalVector(v4,v3,v2);

			normalTriangleArray[index + 0] = norm1.x;
			normalTriangleArray[index + 1] = norm1.y;
			normalTriangleArray[index + 2] = norm1.z;

			normalTriangleArray[index + 3] = norm2.x;
			normalTriangleArray[index + 4] = norm2.y;
			normalTriangleArray[index + 5] = norm2.z;

		}

	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
			GLfloat n2x= 0.0, n2y= 1.0, n2z= 0.0;
			GLfloat n3x= 0.0, n3y= 1.0, n3z= 0.0;
			GLfloat n4x= 0.0, n4y= 1.0, n4z= 0.0;
			GLfloat n5x= 0.0, n5y= 1.0, n5z= 0.0;
			GLfloat n6x= 0.0, n6y= 1.0, n6z= 0.0;

			int index = (x + z * (tex->width-1))*6;

			GLfloat n1x = normalTriangleArray[index + 0];
			GLfloat n1y = normalTriangleArray[index + 1];
			GLfloat n1z = normalTriangleArray[index + 2];

			bool good_x = check_border(x-1,tex->width);
			bool good_z = z > 0 ;
			index = (x-1 + (z-1) * (tex->width-1))*6;
			if( good_x && good_z){
				n2x = normalTriangleArray[index + 3];
				n2y = normalTriangleArray[index + 4];
				n2z = normalTriangleArray[index + 5];
			}

			index = (x-1 + (z) * (tex->width-1))*6;
			if(good_x){
				n3x = normalTriangleArray[index + 0];
				n3y = normalTriangleArray[index + 1];
				n3z = normalTriangleArray[index + 2];
				n4x = normalTriangleArray[index + 3];
				n4y = normalTriangleArray[index + 4];
				n4z = normalTriangleArray[index + 5];
			}


			index = (x + (z-1) * (tex->width-1))*6;
			if(good_z){
				n5x = normalTriangleArray[index + 0];
				n5y = normalTriangleArray[index + 1];
				n5z = normalTriangleArray[index + 2];
				n6x = normalTriangleArray[index + 3];
				n6y = normalTriangleArray[index + 4];
				n6z = normalTriangleArray[index + 5];
			}


			GLfloat resX = (n1x + n2x + n3x + n4x + n5x + n6x)/6 ;
			GLfloat resY = (n1y + n2y + n3y + n4y + n5y + n6y)/6 ;
			GLfloat resZ = (n1z + n2z + n3z + n4z + n5z + n6z)/6 ;
			vec3 vRes = SetVector(resX,resY,resZ);
			vec3 vResNorm = Normalize(vRes);

// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = vResNorm.x;
			normalArray[(x + z * tex->width)*3 + 1] = vResNorm.y;
			normalArray[(x + z * tex->width)*3 + 2] = vResNorm.z;
	}



	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits

	glutPassiveMotionFunc(mouse_motion);

	speed = 0.3;
	actual_speed = speed;

	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	projectionMatrix = perspective(90, 1,
	                      0.2, 5000);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("textures/grid-decimal-512.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);

	m = LoadModelPlus("groundsphere.obj");

	printError("init terrain");
}

void display(void)
{


	direction = SetVector(0,0,0);
	actual_speed = speed;
	float alpha = mouse_x * 2 * M_PI /(float)width;
	float beta = - (0.5f - mouse_y /(float)height) * M_PI;

	input_update();

	camera_movement(alpha, beta);



	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	printError("pre display");

	glUseProgram(program);

	mat4 tm_mv = IdentityMatrix();
	mat4 total = Mult(camMatrix, tm_mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	glUseProgram(program);

	mat4 m_mw = model_to_world(debug_pos, SetVector(0,0,0), SetVector(debug_val,0.1,debug_val));
	total = Mult(camMatrix, m_mw);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");

	printError("display 2");

	glutSwapBuffers();
}


void camera_movement(float alpha, float beta){

	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
		direction = ScalarMult(Normalize(direction), actual_speed);

	mat4 look_mat = Mult(Rx(beta), Ry(alpha));
	look = MultVec3(InvertMat4(look_mat), SetVector(0,0,1));

	direction = MultVec3(InvertMat4(look_mat), direction);
	p = VectorSub(p, direction);
	p = SetVector(debug_pos.x, debug_pos.y + 0.2, debug_pos.z - 0.2);

	camMatrix = Mult(look_mat, T(-p.x, -p.y, -p.z));
}

void mouse_motion (int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void input_update(void){

	if (glutKeyIsDown('l')) {

		if (glutKeyIsDown(FORWARDKEY))
			debug_pos.z += 1;

		if (glutKeyIsDown(BACKKEY))
			debug_pos.z -= 1;

		if (glutKeyIsDown(LEFTKEY))
			debug_pos.x += 1;

		if (glutKeyIsDown(RIGHTKEY))
			debug_pos.x -= 1;

		if (glutKeyIsDown(DOWNKEY))
			debug_pos.y += 1;

		if (glutKeyIsDown(UPKEY))
			debug_pos.y -= 1;
		find_height(debug_pos.x, debug_pos.z, tm, &ttex);

	} else {
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

    if (glutKeyIsDown(ACTIONKEY))
          find_height(1.2,3.4,tm, &ttex);
	}
	if (glutKeyIsDown('o'))
		actual_speed *= 4;

	if (glutKeyIsDown('p'))
		actual_speed /= 4;

}

mat4 model_to_world(const vec3 t, const vec3 r, const vec3 s){
	return mult_rep(false, 3 , S(s.x,s.y,s.z), angle_transform(r.x, r.y, r.z), T(t.x, t.y, t.z));
}

mat4 angle_transform(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat c1 = cos(z);
	GLfloat s1 = sin(z);
	GLfloat c2 = cos(y);
	GLfloat s2 = sin(y);
	GLfloat c3 = cos(x);
	GLfloat s3 = sin(x);

	mat4 rot = {{c2*c3, -c2*s3, s2, 0,
									c1*s3 + c3*s1*s2, c1*c3 - s1*s2*s3, -c2*s1, 0,
									s1*s3 - c1*c3*s2, c3*s1 + c1*s2*s3, c1*c2 , 0,
									0, 0 , 0, 1
								}};
  return rot;
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

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	width = 700;
	height = 700;
	glutInitWindowSize(width, height);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);


	glutMainLoop();
	exit(0);
}
