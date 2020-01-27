#version 150

in  vec3 in_Position;
in vec3 in_Normal;
out vec3 ex_normal;
//uniform mat4 translationMatrix;
//uniform mat4 rotationMatrix;

uniform mat4 matrix;

void main(void)
{	
    
	gl_Position =  matrix *  vec4(in_Position, 1.0);
    ex_normal = in_Normal;
}
//translationMatrix * rotationMatrix * matrix* 