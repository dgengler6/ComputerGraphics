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
in vec4 ex_Pos;

out vec4 out_Color;

uniform material m;
uniform int count;

uniform vec3 light_vecs[10];
uniform vec2 light_int_att[10];
uniform vec3 light_colors[10];
uniform bool is_directional[10];

uniform sampler2D tex;
uniform bool is_textured;

uniform mat4 mw;
uniform mat4 wv;

void main(void)
{
	out_Color = m.color;
	out_Color *= ((is_textured) ? texture(tex, ex_TexCoord) : vec4(1)) ;

	vec3 view_direction = normalize(vec3(inverse(wv) * vec4(0.0, 0.0, 0.0, 1.0) - ex_Pos));
	vec3 n = normalize(ex_Normal);

	vec3 light_result = vec3(0);

	float i_diff = 0;
	float i_spec = 0;
	float i_total = 0;
	float cos_phi = 0;

	for (int i = 0; i < 1 ; i++){
		vec3 light = light_vecs[i];
		float fact = light_int_att[i].x ;
		if (!is_directional[i]) {
			light = vec3(ex_Pos) - light;
			float dist = length(light);
			light = normalize(light);
			if (dist > light_int_att[i].y) fact *= light_int_att[i].y / dist;
		} else {
			light = normalize(light);
		}

		light = - normalize(mat3(wv) * light);

		i_diff = m.k_d * max(dot(light, n),0);

		cos_phi = max(dot(reflect(-light, n), view_direction), 0);
		i_spec = m.k_spec * pow(cos_phi, m.ex);

		i_total =  clamp(i_diff, 0, 1);
		i_total += clamp(i_spec, 0, 1);
		i_total *= fact;
		light_result += clamp((vec3(clamp(i_total,0,1)) * light_colors[i]), 0, 1);
	};


	out_Color *= vec4(light_result, 1);
}
