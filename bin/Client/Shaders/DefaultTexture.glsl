// Basic Texture Shader

#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in uint a_TextureIndex;

uniform mat4 u_ViewProjection = mat4( 1.f );
uniform mat4 u_Model = mat4( 1.f );

out vec4 v_Colour;
out vec2 v_Texcoord;
flat out uint v_TexIndex;

void main()
{
	v_Colour = a_Colour;
	v_Texcoord = a_Texcoord;
	v_TexIndex = a_TextureIndex;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

#type fragment
#version 450

layout(location = 0) out vec4 colour;

in vec4 v_Colour;
in vec2 v_Texcoord;
flat in uint v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
    colour = texture(u_Textures[v_TexIndex], v_Texcoord ) * v_Colour;
}
