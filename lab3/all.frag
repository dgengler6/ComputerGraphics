#version 150

struct material{
	float k_d;
	float k_spec;
	float ex;
	vec4 color;
};

in vec3 ex_Normal;
in vec3 ex_Surface;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform material m;
uniform int count;

uniform vec3 light_vecs[10];
uniform vec2 light_int_att[10];
uniform vec3 light_colors[10];
uniform bool is_directional[10];

uniform sampler2D tex;
uniform bool is_textured;

uniform mat4 wv;

void main(void)
{
	out_Color = m.color;
	out_Color *= ((is_textured) ? texture(tex, ex_TexCoord) : vec4(1)) ;

	float i_diff = 0;
	float i_spec = 0;
	float i_total = 0;
	float cos_phi = 0;

	for (int i = 0; i < 1; i++){
		vec3 light = mat3(wv) * light_vecs[i];
		if (!is_directional[i]) light = ex_Surface - light;

		i_diff = max(dot(light, ex_Normal),0);
		cos_phi = max(dot(reflect(-light, ex_Normal), -ex_Surface), 0);
		i_spec = 1.0 * pow(cos_phi, m.ex);
		i_total = m.k_d * clamp(i_diff, 0, 1) + m.k_spec * clamp(i_spec, 0, 1);

		out_Color += clamp((vec4(i_total) * vec4(light_colors[i],1)), vec4(0), vec4(1));
	};

}
