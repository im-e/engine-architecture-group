#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexColour;

layout(std140) uniform Matrices
{
	mat4 u_VP;
	mat4 cameraPosition;
};

uniform mat4 u_model;
out vec3 fragmentColour;

void main()
{
	fragmentColour = a_vertexColour;
	gl_Position =  u_VP * u_model * vec4(a_vertexPosition,1);
}

#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 fragmentColour;

void main()
{
	colour = vec4(fragmentColour, 1.0);
}