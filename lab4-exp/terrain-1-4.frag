#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 exNormal;
in vec3 exPos;
uniform sampler2D tex;

void main(void)
{
	vec3 light = normalize(vec3(1.0,0.5,0.0));
	vec4 t = texture(tex, texCoord);
	outColor = t * (0 + 1*clamp(dot(normalize(exNormal),light), 0, 1));
}
