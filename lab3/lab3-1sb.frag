#version 150

in vec3 ex_Normal;
in vec2 ex_TexCoord;
out vec4 out_Color;
uniform sampler2D texUnit;

void main(void)
{
<<<<<<< HEAD
	//out_Color = texture(texUnit, ex_TexCoord);
	out_Color = plain_color;
		vec3 n = ex_Normal;
		float k_d = 0.5;
		float k_spec = 0.2;

		float i_a = 0.2;
		vec3 light_1 = normalize(vec3(-1,0.5,-1));
		float i_1 = 1;

		float alpha = 3;
		float i_amb = k_d * i_a;
		float i_diff = k_d * i_1 * dot(light_1, n); //for each light
		float cos_phi = max(dot(reflect(light_1, n), camera_look), 0);
		float i_spec = k_spec * i_1 * pow(cos_phi, alpha);

		float i_total = i_amb + i_diff + i_spec;
		out_Color *= i_total;

=======
	out_Color = texture(texUnit, ex_TexCoord);
>>>>>>> a405636cf4e85c09a1ee7732f3aa49dcf3a1ceb6
}
