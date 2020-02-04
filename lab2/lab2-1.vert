#version 150

in  vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
out vec4 ex_Normal;
out vec2 ex_TexCoord;

uniform mat4 matrix;

void main(void)
{
	gl_Position =  matrix *  vec4(in_Position, 1.0);
  ex_Normal = matrix * vec4(in_Normal,1.0);
  ex_TexCoord= in_TexCoord;
}
