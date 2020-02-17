#version 150

struct material{
	float k_d;
	float k_spec;
	float ex;
	vec4 color;
};

in vec3 ex_Normal;
in vec3 ex_Surface;
out vec4 out_Color;
uniform vec3 camera_look;
uniform material m;
uniform int count;
uniform vec3 lights[10];
uniform vec4 ambient;

uniform mat4 wv;

void main(void)
{
	vec3 n = normalize(ex_Normal);
	out_Color = m.color * vec4(ambient.xyz, 0.5);
	float i_diff = 0;
	float i_spec = 0;

	for (int i = 0; i < count; i++){
		vec3 light = mat3(wv) * lights[i];
		i_diff += max(dot(light, n),0);
		//i_diff += dot(light, n);
		float cos_phi = max(dot(reflect(-light, n), -ex_Surface), 0);
		i_spec += 1.0 * pow(cos_phi, m.ex);
	}

	float i_total = m.k_d * (ambient.w + clamp(i_diff, 0, 1)) + m.k_spec * clamp(i_spec, 0, 1);
	//float i_total = m.k_d * (ambient.w + i_diff) + m.k_spec * i_spec;
	out_Color *= vec4(i_total);
}
