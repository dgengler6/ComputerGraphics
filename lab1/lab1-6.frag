#version 150

in vec3 ex_normal;
out vec4 out_Color;

void main(void)
{
	
	out_Color = vec4(ex_normal, 1.0); // inColor;
}
