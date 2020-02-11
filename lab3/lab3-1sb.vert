#version 150

//varying
in  vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;

uniform mat4 pack_mat;

void main(void)
{
	gl_Position = pack_mat * vec4(in_Position, 1.0);
  ex_Normal = mat3(pack_mat) * in_Normal;
  ex_TexCoord = in_TexCoord;
}
//translationMatrix * rotationMatrix * matrix*
