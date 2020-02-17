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

uniform ivec3 debug;

void main(void)
{
	vec3 d = debug.xyz / 4;
	int i = 0;

	vec3 n = normalize(ex_Normal);

	vec4 point = vec4(d.x,d.y,d.z,1);
	//vec4 point2 = point + vec4(0,0,3,1);

	vec3 seg = vec3(point);
		//vec3 seg2 = vec3(point2);
		
		seg = mat3(wv) * vec3(mw * ex_Pos - point);


//		seg2 = mat3(wv) * vec3(mw * ex_Pos - point2);

		//vec3(ex_Pos - point);   <=>   //vec3(ex_Pos) - vec3(point);

		//mat3(mw) * vec3(ex_Pos) - vec3(point);    <XXXXX>		//vec3(mw * ex_Pos) - vec3(point); //BETTER
		//vec3(mw * ex_Pos - point) <=> vec3(mw * ex_Pos) - vec3(point)

		//vec3(wv * mw * ex_Pos - point) <XXXXX> mat3(wv) * vec3(mw * ex_Pos - point)
		//vec3(mw * ex_Pos - point) <=> mat3(wv) * vec3(mw * ex_Pos - point) BUT NO WITH I_DIFF


//	float dist2 = length(seg2);

	float i_diff = max(dot(n, -normalize(seg)), 0);

	float dist = length(seg);
//	float i_diff2 = max(dot(n, normalize(seg2)), 0);

	//vec3(mw * ex_Pos - point) <XXXXXX> mat3(wv) * vec3(mw * ex_Pos - point)


  //out_Color = vec4(vec3(20 / dist * i_diff), 1);

	out_Color = vec4(vec3(i_diff), 1);

	if (dist > 10) out_Color *= vec4(vec3(10/dist),1);

	// if (gl_FrontFacing) // is the fragment part of a front face?
	// 		{
	// 			out_Color = vec4(1,0,0,1);
	// 		}
	// 	else // fragment is part of a back face
	// 		{
	// 			out_Color = vec4(0,0,1,1);
	// 	 }

	//out_Color *= vec4(i_diff,0,0,1);
	//out_Color *= vec4(0,0,i_diff2,1);
}
