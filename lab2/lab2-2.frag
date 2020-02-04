#version 150

in vec4 ex_normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
  out_Color = texture(texUnit, ex_TexCoord*2);
}
