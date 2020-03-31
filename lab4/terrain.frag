#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
uniform sampler2D tex;

void main(void)
{	

	vec3 light = vec3(1.0,0.0,0.0);
	outColor = texture(tex, texCoord)*dot(normalize(exNormal),light);

}
