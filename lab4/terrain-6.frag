#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
in vec3 exPos;
uniform usampler2D splatmap;
uniform ivec2 terrainSize;
uniform ivec2 splatSize;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform float time;
in float exTime;

void main(void)
{

	vec2 sizeFactor = vec2(1,1);
	vec3 light = normalize(vec3(1.0,0.5,0.0));
	ivec2 t = ivec2(round(splatSize * texCoord));
	uint index = texture(splatmap, texCoord / float(splatSize)).r;
	vec4 tex = vec4(0);
	switch(index)
	{
		case 0u:
			tex = texture(tex0, texCoord);
		break;
		case 1u:
			tex = texture(tex1, texCoord);
		break;
		case 2u:
			tex = texture(tex2, texCoord);
		break;
		case 3u:
			tex = texture(tex3, texCoord);
		break;
		case 4u:
			tex = texture(tex4, texCoord);
		break;
		default:
			tex = vec4(vec3(1040746656.0 - float(index)),1);
	}

	outColor = tex;
	uint test = texture(splatmap, texCoord).r;
	// outColor = vec4((texture(splatmap, texCoord/256) * 40u));
	// outColor = vec4(vec3(test*40u + 0.5),1);

	outColor *=  (0 + 1*clamp(dot(normalize(exNormal),light), 0, 1));
}
