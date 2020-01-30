#region Vertex

#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoords;
out vec3 vNormal ;


void main()
{
    vTexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vNormal = aNormal ;
}

#region Geometry

#version 440 core

layout (triangles) in;
layout (points, max_vertices = 3) out;


in vec2 vTexCoords[];
in vec3 vNormal[];

out vec2 gTexCoords; 
out vec3 gNormal; 
out vec4 gSurfaceNormal;

float normLength = 0.2f;

uniform float time;

vec3 getSurfaceNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 5.0f;
	vec3 tempPos = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return (position + vec4(tempPos, 0.0));
}


void main() {    
	vec3 surfaceNormal = getSurfaceNormal();
	vec4 surfaceNormal4 = vec4(surfaceNormal, 0);
    gl_Position = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position)/3;
	gTexCoords = (vTexCoords[0] + vTexCoords[1] + vTexCoords[2])/3;
	EmitVertex();
	gl_Position = gl_Position + surfaceNormal4 * normLength;
	gl_Position = explode(gl_Position,surfaceNormal);
    EmitVertex();
    EndPrimitive();
}

#region Fragment

#version 440 core

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

in vec2 gTexCoords;
in vec3 gNormal ;
in vec4 gSurfaceNormal;

void main()
{    
	FragColor = vec4(gSurfaceNormal);
	 // FragColor = texture(texture_diffuse1, gTexCoords);
}