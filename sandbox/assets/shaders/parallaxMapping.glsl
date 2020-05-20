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
 
	
	// vec3 T = normalize(mat3(u_model) * aTangentVector);
    // vec3 B = normalize(mat3(u_model) * aBitangentVector);
    // vec3 N = normalize(mat3(u_model) * aNormal);
    // mat3 TBN = transpose(mat3(T, B, N));

    //TangentLightPosition = TBN * u_lightPos;
    //TangentViewPosition  = TBN * u_viewPos;
    //TangentFragmentPosition  = TBN * FragPos;
    //
    //gl_Position =  u_VP * u_model * vec4(aPos, 1.0);
}

#region Tessellation Control
#version 450 core
layout (vertices = 3) out;
in vec3 FragPos[];
in vec2 TexCoords[];
in vec3 TangentLightPosition[];
in vec3 TangentViewPosition[];
in vec3 TangentFragmentPosition[];

out vec3 FragPosTC[];
out vec2 TexCoordsTC[];
out vec3 TangentLightPositionTC[];
out vec3 TangentViewPositionTC[];
out vec3 TangentFragmentPositionTC[];

layout (std140) uniform Camera
{
	vec3 u_camPos;
	vec3 u_camForward;
};

float GetTessLevel(float Distance0, float Distance1)
{
	float avgDist = (Distance0 + Distance1) / 2.0f;
	float tess = 0.0f;


	float exponent = -0.105f * avgDist;
	tess = exp(exponent) * 15.0f;
	if(tess < 1.0f)
	{
		return 1.0f;
	}

	return tess;
}

void main()
{
   float eyeToVertexDist1 = distance(u_camPos, FragPos[1]);
   float eyeToVertexDist2 = distance(u_camPos, FragPos[2]);
   if (gl_InvocationID==0)
   {
		  FragPosTC[gl_InvocationID]  = FragPos[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }

   // pass through position and normal information
   FragPosTC[gl_InvocationID]  = FragPos[gl_InvocationID];
   TexCoordsTC[gl_InvocationID] = TexCoords[gl_InvocationID];
   TangentLightPositionTC[gl_InvocationID] = TangentLightPosition[gl_InvocationID];
   TangentViewPositionTC[gl_InvocationID] = TangentViewPosition[gl_InvocationID];
   TangentFragmentPositionTC[gl_InvocationID] = TangentFragmentPosition[gl_InvocationID];
}

#region Tessellation Eval

#version 450 core

layout(triangles, fractional_even_spacing, ccw) in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

in vec3 FragPosTC[];
in vec2 TexCoordsTC[];
in vec3 TangentLightPositionTC[];
in vec3 TangentViewPositionTC[];
in vec3 TangentFragmentPositionTC[];

out vec3 FragPosTE;
out vec2 TexCoordsTE;
out vec3 TangentLightPositionTE;
out vec3 TangentViewPositionTE;
out vec3 TangentFragmentPositionTE;

void main()
{
	FragPosTE = interpolate3D(FragPosTC[0], FragPosTC[1], FragPosTC[2]);
	TexCoordsTE = interpolate2D(TexCoordsTC[0], TexCoordsTC[1], TexCoordsTC[2]);
	TangentLightPositionTE = interpolate3D(TangentLightPositionTC[0], TangentLightPositionTC[1], TangentLightPositionTC[2]);
	TangentViewPositionTE = interpolate3D(TangentViewPositionTC[0], TangentViewPositionTC[1], TangentViewPositionTC[2]);
	TangentFragmentPositionTE = interpolate3D(TangentFragmentPositionTC[0], TangentFragmentPositionTC[1], TangentFragmentPositionTC[2]);
	gl_Position = u_VP * vec4(FragPosTE, 1.0);
}

//basic linear interpolation
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
   	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

#region Geometry

#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 FragPosTE[];
in vec2 TexCoordsTE[];
in vec3 TangentLightPositionTE[];
in vec3 TangentViewPositionTE[];
in vec3 TangentFragmentPositionTE[];

out vec3 FragPosG;
out vec2 TexCoordsG;
out vec3 TangentLightPositionG;
out vec3 TangentViewPositionG;
out vec3 TangentFragmentPositionG;


void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      FragPosG = FragPosTE[i] ;
	  TexCoordsG = TexCoordsTE[i];
	  TangentLightPositionG = TangentLightPositionTE[i];
	  TangentViewPositionG = TangentViewPositionTE[i];
	  TangentFragmentPositionG = TangentFragmentPositionTE[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}
				
#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 FragPosG;
in vec2 TexCoordsG;
in vec3 TangentLightPositionG;
in vec3 TangentViewPositionG;
in vec3 TangentFragmentPositionG;

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
    vec3 viewDir = normalize(TangentViewPositionG - TangentFragmentPositionG);
    vec2 texCoords = TexCoordsG;
    
    texCoords = CalculateParallaxDisplacement(TexCoordsG,  viewDir);  

    // obtain normal from normal map
    vec3 normal = texture(u_normalTexData, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    // get diffuse color
    vec3 color = texture(u_texData, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPositionG - TangentFragmentPositionG);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular    
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    colour = vec4(ambient + diffuse + specular, 1.0);
} 