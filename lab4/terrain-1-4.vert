#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 exNormal;
out vec3 exPos;
out float exTime;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform float time;

void main(void)
{
	texCoord = inTexCoord;
	exPos = inPosition;
	exNormal = inNormal;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);

}
