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


#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;
mat4 camMatrix;

int mouse_x, mouse_y;
int last_mouse_x, last_mouse_y;

vec3 direction; vec3 look;
GLfloat speed;
GLfloat actual_speed;

int x = 0;
int y = 0;
int z = 0;

vec3 p;

int width, height;

void mouse_motion (int x, int y);
void input_update(void);
void camera_movement(float alpha, float beta);

bool check_border(int index, int width){
	return (index >=0 && ((index + 1) % width)!=0 )|| index == 0;
}

vec3 get_vec_from_array(int x, int z, Model* terrain){
    int index = (x + z + sqrt(terrain->numVertices))*3;
    return SetVector(terrain->vertexArray[index],terrain->vertexArray[index+1],terrain->vertexArray[index+2]);
}

bool find_height(float x, float z, Model* terrain){
    int xf = (int)(floor(x));
    int xc = (int)(ceil(x));
    int zf = (int)(floor(z));
    int zc = (int)(ceil(z));
    printf("Yo");
    vec3 yeet = get_vec_from_array(xf,zf,terrain); 
    printf("%f",yeet.x);

}

Model* GenerateTerrain(TextureData *tex)
{

	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLfloat *normalTriangleArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
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
			vec3 norm2 = CalcNormalVector(v2,v3,v4);

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
			float n2x= 0.0,n2y= 1.0,n2z= 0.0,n3x= 0.0,n3y= 1.0,n3z= 0.0,n4x= 0.0,n4y= 1.0,n4z= 0.0,n5x= 0.0,n5y= 1.0,n5z= 0.0,n6x= 0.0,n6y= 1.0,n6z = 0.0;
			int index = (x + z * (tex->width-1))*6;

			float n1x = normalTriangleArray[index + 0];
			float n1y = normalTriangleArray[index + 1];
			float n1z = normalTriangleArray[index + 2];
			
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


			float resX = (n1x + n2x + n3x + n4x + n5x + n6x)/6 ;
			float resY = (n1y + n2y + n3y + n4y + n5y + n6y)/6 ;
			float resZ = (n1z + n2z + n3z + n4z + n5z + n6z)/6 ;
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
	LoadTGATextureSimple("44-terrain-2.tga", &tex1);

// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
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

	mat4 total, modelView;
	//mat4 camMatrix;

	printError("pre display");

	glUseProgram(program);

	// Build matrix

	// vec3 cam = {0, 5, 8};
	// vec3 lookAtPoint = {2, 0, 2};
	// camMatrix = lookAt(cam.x, cam.y, cam.z,
				// lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				// 0.0, 1.0, 0.0);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

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

	camMatrix = Mult(look_mat, T(-p.x, -p.y, -p.z));
}

void mouse_motion (int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void input_update(void){

	if (glutKeyIsDown('l')) {

		if (glutKeyIsDown(FORWARDKEY))
			z++;

		if (glutKeyIsDown(BACKKEY))
			z--;

		if (glutKeyIsDown(LEFTKEY))
			x++;

		if (glutKeyIsDown(RIGHTKEY))
			x--;

		if (glutKeyIsDown(DOWNKEY))
			y++;

		if (glutKeyIsDown(UPKEY))
			y--;
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

        if (glutKeyIsDown(UPKEY))
            find_height(1.2,3.4,tm);
	}
	if (glutKeyIsDown('o'))
		actual_speed *= 2;

	if (glutKeyIsDown('p'))
		actual_speed /= 2;

	if (glutKeyIsDown('l')) printf("(%d, %d, %d)\n", x/4, y/4, z/4);
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
