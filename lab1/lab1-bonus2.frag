#version 150

in vec4 ex_normal;

out vec4 out_Color;


void main(void)
{
	vec3 n = vec3(ex_normal.x, ex_normal.y, ex_normal.z);
	float k_d = 1;
	float k_spec = 0.5;

	float i_a = 0.2;
	vec3 light_1 = normalize(vec3(0,0.4,-1));
	float i_1 = 1;

	float alpha = 50;
	float i_amb = k_d * i_a;
	float i_diff = k_d * i_1 * dot(light_1, n); //for each light
	float cos_phi = max(dot(reflect(light_1, n), vec3(0,0,1)), 0);
	float i_spec = k_spec * i_1 * pow(cos_phi, alpha);

	float i_total = i_amb + i_diff + i_spec;
	out_Color = vec4(i_total); // inColor;
}
