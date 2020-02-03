#version 150

in vec4 ex_normal;
out vec4 out_Color;

void main(void)
{
	float shadow = dot(normalize(ex_normal),vec4(0.0,0.0,-1.0,0.0));
	out_Color = vec4(shadow,shadow,shadow, shadow); // inColor;
}
