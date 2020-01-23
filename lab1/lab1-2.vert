#version 150

in  vec3 in_Position;
in vec3 in_Color;
out vec3 ex_Color;
uniform mat4 myMatrix;

void main(void)
{	
	ex_Color = vec3(in_Color);
    
	gl_Position =myMatrix* vec4(in_Position, 1.0);
}
