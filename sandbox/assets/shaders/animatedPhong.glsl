#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexNormal;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in ivec4 a_boneIDs;
layout(location = 4) in vec4 a_weights;

out vec3 fragmentPos;
out vec3 normal;
out vec2 texCoord;

const int maxBones = 200;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;
uniform mat4 u_bones[maxBones];

void main()
{
	mat4 boneTransform = u_bones[a_boneIDs[0]] * a_weights[0];
	boneTransform += u_bones[a_boneIDs[1]] * a_weights[1];
	boneTransform += u_bones[a_boneIDs[2]] * a_weights[2];
	boneTransform += u_bones[a_boneIDs[3]] * a_weights[3];

	vec4 PosL = boneTransform * vec4(a_vertexPosition, 1.0);
	gl_Position =  u_VP * PosL;
	texCoord = vec2(a_texCoord.x, a_texCoord.y);
	vec4 normalL = boneTransform * vec4(a_vertexNormal, 0.0);
	normal = (u_model * normalL).xyz;
	fragmentPos = (u_model * PosL).xyz;
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 normal;
in vec3 fragmentPos;
in vec2 texCoord;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

uniform sampler2D u_texData;

void main()
{
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * u_lightColour;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_lightPos - fragmentPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColour;
	
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * u_lightColour;  
	
	colour = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, texCoord);
}