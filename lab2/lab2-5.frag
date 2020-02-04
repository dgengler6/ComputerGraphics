#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform float time;
uniform sampler2D texUnit;

void main(void)
{
	vec3 n = ex_Normal;
	float k_d = 1;

	float i_a = 0.2;
	vec3 light_1 = normalize(vec3(1,0,-1));
	float i_1 = 1;

	float i_amb = k_d * i_a;
	float i_diff = k_d * i_1 * dot(light_1, n); //for each light

	float i_total = i_amb + i_diff;

  out_Color = texture(texUnit, ex_TexCoord*2) * i_total;

}
