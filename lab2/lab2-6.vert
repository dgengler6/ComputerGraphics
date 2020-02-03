#version 150

//varying
in  vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out float ex_Light;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 matrix;
uniform mat4 vingthuit;
uniform float time;

void main(void)
{
	gl_Position = vingthuit * vec4(in_Position, 1.0);
  ex_Normal = mat3(vingthuit) * in_Normal;
  ex_TexCoord = in_TexCoord;

  if (int(time/1000) % 2 == 0) {
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
    ex_Light = i_total;
  }
}
//translationMatrix * rotationMatrix * matrix*
