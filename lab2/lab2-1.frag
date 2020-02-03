#version 150

in vec4 ex_normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform float time;

void main(void)
{
	vec4 topcolor, bottomcolor;
	float shadow = dot(normalize(ex_normal),vec4(0.0,0.0,-1.0,0.0));
	vec4 shadowVec = vec4(shadow); // inColor;
	vec4 c1 = vec4(0.0,0.0,sin(time/1000)/2+0.5,1.0);
	vec4 c2 = vec4(sin(time/1000)/2+0.5,1.0,0.0,1.0);
	vec4 red = vec4(1.0,0.0,0.0,1.0);
	vec4 green = vec4(0.0,1.0,0.0,1.0);
	vec4 yellow = vec4(1.0,1.0,0.0,1.0);

	//if(sin(time/100)>0){
		//topcolor = c1;
		//bottomcolor = c2;
	//}else{
		//topcolor = c2;
		//bottomcolor = c1;
	//}
	out_Color = shadowVec;
	float y = ex_TexCoord.y;
	 y+= sin(ex_TexCoord.x*5 + time/10000)/20;
	if(y*2-1>sin(time/500)+0.35 ){
		 out_Color+= red;
	}else if (y*2-1<sin(time/500)-0.35){
		out_Color += green ;
	}else{
		out_Color += yellow ;
	}

	

}
