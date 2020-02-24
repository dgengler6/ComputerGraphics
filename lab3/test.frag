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
in vec3 ex_Pos;

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

uniform ivec3 debug;

vec3 n = normalize(ex_Normal);
vec3 view_direction = normalize(vec3(inverse(wv) * vec4(0.0, 0.0, 0.0, 1.0) - vec4(ex_Pos,1)));

float shade(int i);
float shade_debug(vec3 vec, bool is_directional, float k_d, float k_spec, float ex, float intens, float attenu);

void main(void)
	{
	out_Color = m.color;
	vec3 d = debug.xyz / 4;

	vec3 light_result = vec3(0);

	for (int i = -1; i < count; i++){
		float i_total;
		if (i == -1)
			i_total = shade_debug(vec3(d.x,d.y,d.z), false, m.k_d, m.k_spec, m.ex, 1.3, 100);
		else
			i_total = shade(i);

		light_result += clamp(vec3(i_total) * ((i == -1) ? vec3(1,1,1) : light_colors[i]), 0, 1);
	}
	out_Color *= vec4(clamp(light_result, 0, 1), 1);
}

float shade(int i){
	return shade_debug(light_vecs[i], is_directional[i], m.k_d, m.k_spec, m.ex, light_int_att[i].x, light_int_att[i].y);
}

float shade_debug(vec3 vec, bool is_directional, float k_d, float k_spec, float ex, float intens, float attenu){
	vec3 light = vec;

	float fact = intens;

	if (!is_directional) {
			light = ex_Pos - light;
			float dist = length(light);
			if (dist > attenu) fact *= attenu / dist ;
	}

	light = - normalize(mat3(wv) * light);

	float i_diff = max(dot(light, n),0); //MAX 1

	float cos_phi = max(dot(reflect(-light, n), view_direction), 0); //MAX 1
	float i_spec = 1.0 * pow(cos_phi, ex);

	float i_total = 0;
	i_total += k_d * i_diff;
	i_total += k_spec * i_spec;
	i_total = clamp(i_total * fact, 0, 1);
	return i_total;
}
