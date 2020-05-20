#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangentVector;

uniform mat4 u_model;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 fragNormal;
out vec3 TangentLightPosition;
out vec3 TangentViewPosition;
out vec3 TangentFragmentPosition;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

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
    
    gl_Position =  u_model * vec4(aPos, 1.0);
}

#region Tessellation Control

#version 450 core
layout (vertices =3) out;

in vec3 FragPos[];
in vec3 fragNormal[];
in vec3 TangentLightPosition[];
in vec3 TangentViewPosition[];
in vec3 TangentFragmentPosition[];
in vec2 TexCoords[];

out vec3 fragPosTC[];
out vec3 normalTC[];
out vec3 TangentLightPositionTC[];
out vec3 TangentViewPositionTC[];
out vec3 TangentFragmentPositionTC[];
out vec2 texCoordTC[];

layout(std140) uniform CameraPosition
{
	vec3 u_camPos;
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
   float eyeToVertexDist0 = distance(u_camPos, FragPos[0]);
   float eyeToVertexDist1 = distance(u_camPos, FragPos[1]);
   float eyeToVertexDist2 = distance(u_camPos, FragPos[2]);
   if (gl_InvocationID==0)
   {
		  fragPosTC[gl_InvocationID]  = FragPos[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);  
          gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1); 
          gl_TessLevelInner[0] = gl_TessLevelOuter[2]; 

   }

   // pass through position and normal information
   fragPosTC[gl_InvocationID]  = FragPos[gl_InvocationID];
   normalTC[gl_InvocationID] = fragNormal[gl_InvocationID];
   texCoordTC[gl_InvocationID] = TexCoords[gl_InvocationID];
   TangentLightPositionTC[gl_InvocationID] = TangentLightPosition[gl_InvocationID];
   TangentViewPositionTC[gl_InvocationID] = TangentViewPosition[gl_InvocationID];
   TangentFragmentPositionTC[gl_InvocationID] = TangentFragmentPosition[gl_InvocationID];
}

#region Tessellation Eval

#version 450 core

layout(triangles, fractional_even_spacing, ccw) in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

in vec3 fragPosTC[];
in vec3 normalTC[];
in vec3 TangentLightPositionTC[];
in vec3 TangentViewPositionTC[];
in vec3 TangentFragmentPositionTC[];
in vec2 texCoordTC[];

out vec3 fragPosTE;
out vec3 normalTE;
out vec3 TangentLightPositionTE;
out vec3 TangentViewPositionTE;
out vec3 TangentFragmentPositionTE;
out vec2 texCoordTE;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

void main()
{
	fragPosTE = interpolate3D(fragPosTC[0], fragPosTC[1], fragPosTC[2]);
	normalTE = interpolate3D(normalTC[0], normalTC[1], normalTC[2]);
	texCoordTE = interpolate2D(texCoordTC[0], texCoordTC[1], texCoordTC[2]);
	TangentLightPositionTE = interpolate3D(TangentLightPositionTC[0], TangentLightPositionTC[1], TangentLightPositionTC[2]);
	TangentViewPositionTE = interpolate3D(TangentViewPositionTC[0], TangentViewPositionTC[1], TangentViewPositionTC[2]);
	TangentFragmentPositionTE = interpolate3D(TangentFragmentPositionTC[0], TangentFragmentPositionTC[1], TangentFragmentPositionTC[2]);
	gl_Position = u_VP * vec4(fragPosTE, 1.0);
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
vec3 getNormal();

in vec3 fragPosTE[];
in vec3 normalTE[];
in vec3 TangentLightPositionTE[];
in vec3 TangentViewPositionTE[];
in vec3 TangentFragmentPositionTE[];
in vec2 texCoordTE[];

out vec3 fragPosG;
out vec3 normalG;
out vec3 TangentLightPositionG;
out vec3 TangentViewPositionG;
out vec3 TangentFragmentPositionG;
out vec2 texCoordG;

void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      fragPosG = fragPosTE[i] ;
      normalG = getNormal() ; 
	  texCoordG = texCoordTE[i];
	  TangentLightPositionG = TangentLightPositionTE[i];
	  TangentViewPositionG = TangentViewPositionTE[i];
	  TangentFragmentPositionG = TangentFragmentPositionTE[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}

vec3 getNormal()
{
    vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a, b));
}


#region Fragment

#version 440 core
			
layout(location = 0) out vec4 colour;

in vec3 fragPosG;
in vec3 TangentLightPositionG;
in vec3 TangentViewPositionG;
in vec3 TangentFragmentPositionG;
in vec2 texCoordG;

uniform sampler2D u_texData;
uniform sampler2D u_normalTexData;

void main()
{
    vec3 normal = texture(u_normalTexData, texCoordG).rgb; //get normals from normal map
	normal = normalize(normal * 2.0f - 1.0f); //transform normals to be in range <-1; 1> (normal in tangent space)

    // get diffuse color
    vec3 color = texture(u_texData, texCoordG).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPositionG - TangentFragmentPositionG);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(TangentViewPositionG - TangentFragmentPositionG);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    colour = vec4(ambient + diffuse + specular, 1.0);
} 