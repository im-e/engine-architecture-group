#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;

out vec2 texCoord;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;

void main()
{
	texCoord = vec2(a_texCoord.x, a_texCoord.y);
	gl_Position =  u_VP * u_model * vec4(a_vertexPosition, 1.0);
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 texCoord;

uniform sampler2D u_cubemap;

void main()
{
	colour = texture(u_cubemap, texCoord);
}