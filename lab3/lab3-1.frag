#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
in float ex_Light;
out vec4 out_Color;
uniform float time;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, ex_TexCoord*2);

	if (int(time/1000) %2 == 1) {
		vec3 n = ex_Normal;
		float k_d = 0.5;
		float k_spec = 0.5;

		float i_a = 0.2;
		vec3 light_1 = normalize(vec3(1,0,-1));
		float i_1 = 1;

		float alpha = 15;
		float i_amb = k_d * i_a;
		float i_diff = k_d * i_1 * dot(light_1, n); //for each light
		float cos_phi = max(dot(reflect(light_1, n), vec3(0,0,1)), 0);
		float i_spec = k_spec * i_1 * pow(cos_phi, alpha);

		float i_total = i_amb + i_diff + i_spec;
		out_Color *= i_total;
	} else {
		out_Color *= ex_Light;
	}
	//float shadow = dot(normalize(ex_Normal),vec4(0.0,0.0,-1.0,0.0));
	//vec4 shadowVec = vec4(shadow); // inColor;



}