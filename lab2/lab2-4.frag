#version 150

in vec4 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform float time;
uniform sampler2D texUnit;

void main(void)
{
	vec4 topcolor, bottomcolor;
	float shadow = dot(normalize(ex_Normal),vec4(0.0,0.0,-1.0,0.0));

	vec4 shadowVec = vec4(shadow); // inColor;
	vec4 c1 = vec4(0.0,0.0,sin(time/1000)/2+0.5,1.0);
	vec4 c2 = vec4(sin(time/1000)/2+0.5,1.0,0.0,1.0);
	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 green = vec4(0.0,1.0,0.0,1.0);
	vec4 yellow = vec4(1.0,1.0,0.0,1.0);

  out_Color = texture(texUnit, ex_TexCoord*2);

}
