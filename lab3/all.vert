#version 150

//varying
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec3 ex_Surface;

uniform mat4 mw;
uniform mat4 wv;
uniform mat4 vp;

mat4 pack_mat = vp * wv * mw;
mat4 mv = wv * mw;

void main(void)
{
	gl_Position =  pack_mat * vec4(in_Position, 1.0);
  ex_Normal = inverse(transpose(mat3(mv))) * in_Normal;
	//ex_Normal = normalize(mat3(mv) * in_Normal);
	ex_Surface = normalize(vec3(mv * vec4(in_Position, 1.0)));
	ex_TexCoord = in_TexCoord;
}
