#version 150

in vec4 ex_Normal;
out vec4 out_Color;

void main(void)
{
	float shadow = dot(normalize(ex_Normal),vec4(0.0,0.0,-1.0,0.0));
	out_Color = vec4(shadow);
}
