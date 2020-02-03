#version 150

//varying
in  vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 matrix;
uniform mat4 vingthuit;

void main(void)
{

	gl_Position = vingthuit * vec4(in_Position, 1.0);
  ex_Normal = mat3(vingthuit) * in_Normal;
  ex_TexCoord = in_TexCoord;
}
//translationMatrix * rotationMatrix * matrix*
