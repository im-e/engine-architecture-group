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
    //vec4 col = texture(u_colourTexture, TexCoords).rgb;
    //colour = vec4(col, 1.0);
    colour = vec4(1.0, 0.2, 0.5, 1.0);
}