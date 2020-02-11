#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, ex_TexCoord);
	out_Color.a = 0;
	//out_Color = vec3(0.5,0.2,1);
}
