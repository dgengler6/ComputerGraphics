#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
in vec3 exPos;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
in float exTime;

void main(void)
{
	vec4 t0 = texture(tex0, texCoord/10 + 0.01*sin(exTime/1000.0));
	vec4 t1 = texture(tex1, texCoord);
	vec4 t2 = texture(tex2, texCoord);
	vec4 t3 = texture(tex3, texCoord);
	vec4 t4 = texture(tex4, texCoord);
	vec3 light = normalize(vec3(1.0,0.5,0.0));

	float st = 1 - dot(normalize(exNormal),vec3(0,1,0));
	if(exPos.y <= 0.0 ){
		outColor =  t0;
	}
	if(exPos.y > 0.0 && exPos.y<0.1)
		outColor = mix(t0,t1,vec4(exPos.y/0.1));
	if(exPos.y > 0.1 && exPos.y<0.4){
		outColor = t1;
	}
	if(exPos.y > 0.4 && exPos.y<0.8)
		outColor = mix(t1,t3,vec4((exPos.y-0.4)/0.4));
	if(exPos.y >= 0.8 ){
		outColor = t3;

	}
	if(exPos.y >=1.5 ){
		if(st>0.7){
			outColor = t2;
		}else{
			outColor = mix(outColor,t2,vec4(st*1.3));
		}

	}
	if(exPos.y >= 25 ){
		outColor = t2;

	}
	if(exPos.y >= 35 ){
		outColor = mix(t2,t4,vec4((exPos.y-35)/20));

	}

	outColor *=  (0 + 1*clamp(dot(normalize(exNormal),light), 0, 1));
}
