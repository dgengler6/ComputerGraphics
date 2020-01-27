#version 150

in vec3 in_Position;
in vec3 in_Normal;
out vec4 ex_normal;
//uniform mat4 translationMatrix;
//uniform mat4 rotationMatrix;

uniform mat4 matrix;
uniform float time;

void main(void)
{
  float x = in_Position.x + sin(time/200 + in_Position.y*4)/5;
	gl_Position = matrix * vec4(x, in_Position.y, in_Position.z, 1.0);
  ex_normal =  matrix * vec4(in_Normal,1.0);
}
//translationMatrix * rotationMatrix * matrix*
