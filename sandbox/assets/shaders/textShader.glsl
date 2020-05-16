#region Vertex

#version 440 core
			
layout(location = 0) in vec2 a_vertexPosition;
layout(location = 1) in vec2 a_textureCoord;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

out vec2 textureCoord;

uniform mat4 u_model;

void main()
{
	textureCoord = a_textureCoord;
	gl_Position =  u_VP * u_model * vec4(a_vertexPosition, 0.0f, 1.0f);
}

#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 textureCoord;

uniform sampler2D u_texData;

void main()
{
	colour = texture(u_texData, textureCoord);
	//colour = vec4(1.0, 1.0, 1.0, 1.0);
}