#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangentVector;
layout (location = 4) in vec3 aBitangentVector;

out vec3 FragPos;
out vec2 TexCoords;
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
    
    vec3 T = normalize(mat3(u_model) * aTangentVector);
    vec3 B = normalize(mat3(u_model) * aBitangentVector);
    vec3 N = normalize(mat3(u_model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPosition = TBN * u_lightPos;
    TangentViewPosition  = TBN * u_viewPos;
    TangentFragmentPosition  = TBN * FragPos;
    
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

float heightScale = 0.1;

uniform sampler2D u_texData;
uniform sampler2D u_normalTexData;
uniform sampler2D u_parallaxTexData;

vec2 CalculateParallaxDisplacement(vec2 textureCoordinates, vec3 viewDirection)
{
	float height = texture(u_parallaxTexData, textureCoordinates).r; //get height data
	vec2 displacementWeight = viewDirection.xy / viewDirection.z * (height * heightScale); //calculate how far a texture shall be shifted
	return textureCoordinates - displacementWeight; //shift texture accordingly
}

void main()
{
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(TangentViewPosition - TangentFragmentPosition);
    vec2 texCoords = TexCoords;
    
    texCoords = CalculateParallaxDisplacement(TexCoords,  viewDir);  
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;

    // obtain normal from normal map
    vec3 normal = texture(u_normalTexData, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // get diffuse color
    vec3 color = texture(u_texData, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPosition - TangentFragmentPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    colour = vec4(ambient + diffuse + specular, 1.0);
} 