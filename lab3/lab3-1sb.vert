#version 150

//varying
in vec3 in_Position;
in vec2 in_TexCoord;
out vec2 ex_TexCoord;

uniform mat4 pack_mat;

void main(void)
{
	gl_Position = pack_mat * vec4(in_Position, 1.0);
  ex_TexCoord = in_TexCoord;
}
