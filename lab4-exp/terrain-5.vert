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
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	exPos = inPosition;
	exNormal = inNormal;
	exTime= time;
	vec3 pos = inPosition;
	if(exPos.y <= 0 )
		pos = vec3(inPosition.x,inPosition.y + 0.3*sin(time/1000 + inPosition.x/2+inPosition.z/4),inPosition.z);
	gl_Position = projMatrix * mdlMatrix * vec4(pos, 1.0);
	
	
}
