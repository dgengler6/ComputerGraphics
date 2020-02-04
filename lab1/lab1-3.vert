#version 150

in  vec3 in_Position;
uniform mat4 rotationMatrix;

void main(void)
{
	gl_Position =  rotationMatrix * vec4(in_Position, 1.0);
}
