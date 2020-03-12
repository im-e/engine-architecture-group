#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 TexCoords;

uniform sampler2D u_colourTexture;

void main()
{           
    vec4 colour = texture(u_colourTexture, TexCoords).rgb;
    colour = vec4(colour, 1.0);
    colour = vec4(1.0, 0.0, 1.0, 1.0);
}