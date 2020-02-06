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

// vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
// { 
//     const float minLayers = 8;
//     const float maxLayers = 32;
//     float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
//     float layerDepth = 1.0 / numLayers;
//     float currentLayerDepth = 0.0;
// 
//     vec2 P = viewDir.xy / viewDir.z * heightScale; 
//     vec2 deltaTexCoords = P / numLayers;
//   
//     vec2  currentTexCoords     = texCoords;
//     float currentDepthMapValue = texture(u_parallaxTexData, currentTexCoords).r;
//       
//     while(currentLayerDepth < currentDepthMapValue)
//     {
//         currentTexCoords -= deltaTexCoords;
//         currentDepthMapValue = texture(u_parallaxTexData, currentTexCoords).r;  
//         currentLayerDepth += layerDepth;  
//     }
//     
//     vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
// 
//     float afterDepth  = currentDepthMapValue - currentLayerDepth;
//     float beforeDepth = texture(u_parallaxTexData, prevTexCoords).r - currentLayerDepth + layerDepth;
// 
//     float weight = afterDepth / (afterDepth - beforeDepth);
//     vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
// 
//     return finalTexCoords;
// }
// 
// void main()
// {
//     vec3 viewDir = normalize(TangentViewPosition - TangentFragmentPosition);
//     vec2 texCoords = TexCoords;
//     
//     texCoords = ParallaxMapping(TexCoords,  viewDir);       
//     if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0);
//         
// 		//discard;
// 
//     vec3 normal = texture(u_normalTexData, TexCoords).rgb; //get normals from normal map
// 	normal = normalize(normal * 2.0f - 1.0f); //transform normals to be in range <-1; 1> (normal in tangent space)
// 
//     // get diffuse color
//     vec3 color = texture(u_texData, TexCoords).rgb;
//     // ambient
//     vec3 ambient = 0.1 * color;
//     // diffuse
//     vec3 lightDir = normalize(TangentLightPosition - TangentFragmentPosition);
//     float diff = max(dot(lightDir, normal), 0.0);
//     vec3 diffuse = diff * color;
//     // specular
//     // vec3 viewDir = normalize(TangentViewPosition - TangentFragmentPosition);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     vec3 halfwayDir = normalize(lightDir + viewDir);  
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
// 
//     vec3 specular = vec3(0.2) * spec;
//     colour = vec4(ambient + diffuse + specular, 1.0);
// } 


///////////////////////////////////////////////////////////


// vec2 CalculateParallaxDisplacement(vec2 textureCoordinates, vec3 viewDirection)
// {
// 	float height = texture(u_parallaxTexData, textureCoordinates).r; //get height data
// 	vec2 displacementWeight = viewDirection.xy / viewDirection.z * (height * heightScale); //calculate how far a texture shall be shifted
// 	return textureCoordinates - displacementWeight; //shift texture accordingly
// }
// 
// void main()
// {
//     vec3 viewDirection = normalize(TangentViewPosition - TangentFragmentPosition);
// 	vec2 texCoords = TexCoords;
// 
// 	texCoords = CalculateParallaxDisplacement(TexCoords, viewDirection);
// 
//     vec3 normal = texture(u_normalTexData, TexCoords).rgb; //get normals from normal map
// 	normal = normalize(normal * 2.0f - 1.0f); //transform normals to be in range <-1; 1> (normal in tangent space)
// 
//     // get diffuse color
//     vec3 color = texture(u_texData, TexCoords).rgb;
//     // ambient
//     vec3 ambient = 0.1 * color;
//     // diffuse
//     vec3 lightDir = normalize(TangentLightPosition - TangentFragmentPosition);
//     float diff = max(dot(lightDir, normal), 0.0);
//     vec3 diffuse = diff * color;
//     // specular
//     vec3 viewDir = normalize(TangentViewPosition - TangentFragmentPosition);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     vec3 halfwayDir = normalize(lightDir + viewDir);  
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
// 
//     vec3 specular = vec3(0.2) * spec;
//     colour = vec4(ambient + diffuse + specular, 1.0);
// }