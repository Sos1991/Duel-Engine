#version 330
#define	VES_POSITION		0
#define	VES_BLENTWEIGHT		1
#define	VES_BLENDINDICES	2
#define	VES_NORMAL			3
#define	VES_DIFFUSE			4
#define	VES_SPECULAR		5
#define	VES_TEXCOORD		6
#define	VES_BINORMAL		14
#define	VES_TANGENT			15

layout(location = VES_POSITION)	in vec3 attrib_position;  
layout(location = VES_NORMAL)	in vec3	attrib_normal;
layout(location = VES_TEXCOORD)	in vec2 attrib_texcoord;  

// auto parameter.
uniform mat4 Auto_WorldViewProjMatrix;

out	Varing
{
	vec4	hpos;	// homogeneous space(device space)
	vec2	texcoord;
	vec3	vnormal;
	vec2	depth;	// hpos.zw
} VS_StateOutput;


void main(void)
{
	vec4 pos = vec4(attrib_position, 1.0f);
	gl_Position = pos;
	VS_StateOutput.texcoord = attrib_texcoord;
}