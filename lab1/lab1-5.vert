#version 150

in  vec3 in_Position;
in vec3 in_Color;
out vec3 ex_Color;
//uniform mat4 translationMatrix;
//uniform mat4 rotationMatrix;

uniform mat4 matrix;

void main(void)
{	
	ex_Color = vec3(in_Color);
    
	gl_Position = matrix*   vec4(in_Position, 1.0);
}
//translationMatrix * rotationMatrix *