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
	mat4 u_camPos;
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
			
layout(location = 0) out vec4 FragColor;

in vec3 FragPos;  
in vec2 TexCoords;
in vec3 Normal;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

uniform sampler2D u_texData;
uniform sampler2D u_specularTexData;

void main()
{

	vec3 color = texture(u_texData, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.25 * color;  
	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color; 
    
    // specular
	vec3 specularTex = 0.6 * texture(u_specularTexData, TexCoords).rgb;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularTex * spec; 
    
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;

    // attenuation
    float distance    = length(u_lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 