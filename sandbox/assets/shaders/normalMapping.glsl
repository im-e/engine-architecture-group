#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangentVector;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 fragNormal;
out vec3 TangentLightPosition;
out vec3 TangentViewPosition;
out vec3 TangentFragmentPosition;

layout (std140) uniform Matrices
{
	mat4 u_VP;
};

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

uniform mat4 u_model;

void main()
{
    FragPos = vec3(u_model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(u_model)));
	vec3 tangent = normalize(normalMatrix*aTangentVector);
	vec3 normal = normalize(normalMatrix*aNormal);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);

	mat3 TBNMatrix = transpose(mat3(tangent, bitangent, normal));

	TangentLightPosition = TBNMatrix * u_lightPos;
	TangentViewPosition = TBNMatrix * u_viewPos;
	TangentFragmentPosition = TBNMatrix * FragPos;
    
    gl_Position =  u_VP * u_model * vec4(aPos, 1.0);
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 FragPos;  
in vec2 TexCoords;
in vec3 TangentLightPosition;
in vec3 TangentViewPosition;
in vec3 TangentFragmentPosition;

// uniform sampler2D diffuseMap;
uniform sampler2D u_texData;
uniform sampler2D u_normalTexData;

void main()
{
    vec3 normal = texture(u_normalTexData, TexCoords).rgb; //get normals from normal map
	normal = normalize(normal * 2.0f - 1.0f); //transform normals to be in range <-1; 1> (normal in tangent space)

    // get diffuse color
    vec3 color = texture(u_texData, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPosition - TangentFragmentPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(TangentViewPosition - TangentFragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    colour = vec4(ambient + diffuse + specular, 1.0);
} 