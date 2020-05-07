#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0); 
}
				
#region Fragment

#version 440 core

out vec4 colour;

in vec2 TexCoords;

uniform sampler2D u_colourTexture;

void main()
{           
	colour = texture(u_colourTexture, TexCoords);
    float average = 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
    colour = vec4(average, average, average, 1.0);
}