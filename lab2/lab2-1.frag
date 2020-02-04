#version 150

in vec4 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform float time;

void main(void)
{
	float shadow = dot(normalize(ex_Normal),vec4(0.0,0.0,-1.0,0.0));
	vec4 shadowVec = vec4(shadow);

	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 green = vec4(0.0,1.0,0.0,1.0);
	vec4 yellow = vec4(1.0,1.0,0.0,1.0);

	out_Color = shadowVec;
	float y = ex_TexCoord.y;
	y += sin(ex_TexCoord.x*5 + time/10000)/20;
	if(y*2-1>sin(time/500)+0.35 )
		 out_Color+= red;
	else if (y*2-1<sin(time/500)-0.35)
		out_Color += green ;
	else
		out_Color += yellow ;

}
