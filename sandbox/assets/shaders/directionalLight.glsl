#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;


layout (std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;

void main()
{
    FragPos = vec3(u_model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = u_VP * vec4(FragPos, 1.0);
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform sampler2D u_texData;
uniform sampler2D u_specularTexData;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

layout(std140) uniform Settings
{
    vec3 u_direction;
    vec3 u_ambient;
    vec3 u_diffuse;
    vec3 u_specular;
};

void main()
{
    // ambient
    vec3 ambient = u_ambient * texture(u_texData, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(u_lightPos - FragPos);
    vec3 lightDir = normalize(-u_direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_diffuse * diff * texture(u_texData, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.2);
    vec3 specular = u_specular * spec * texture(u_specularTexData, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    colour = vec4(result, 1.0);
}  