#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
uniform sampler2D tex;

void main(void)
{

	vec3 light = normalize(vec3(1.0,0.5,0.0));
	outColor = texture(tex, texCoord/32) * (0 + 1*clamp(dot(normalize(exNormal),light), 0, 1));

}
