#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 fragPosV;
out vec2 texCoordsV;
out vec3 normalV;

layout (std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;

void main()
{
    fragPosV = aPos;
    normalV = aNormal;
    texCoordsV = aTexCoords;
    gl_Position = u_VP * u_model * vec4(aPos, 1.0); 
}

#region Tessellation Control

#version 450 core
layout (vertices = 3) out;

in vec3 fragPosV[];
in vec3 normalV[];
in vec2 texCoordV[];

out vec3 fragPosTC[];
out vec3 normalTC[];
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
	tess = 5.0f;
	return tess;
}

void main()
{
   float eyeToVertexDist1 = distance(u_camPos, fragPosV[1]);
   float eyeToVertexDist2 = distance(u_camPos, fragPosV[2]);
   if (gl_InvocationID==0)
   {
		  fragPosTC[gl_InvocationID]  = fragPosV[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }

   // pass through position and normal information
   fragPosTC[gl_InvocationID]  = fragPosV[gl_InvocationID];
   normalTC[gl_InvocationID] = normal[gl_InvocationID];
   texCoordTC[gl_InvocationID] = texCoord[gl_InvocationID];
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

in vec3 fragPosTC[];
in vec3 normalTC[];
in vec2 texCoordTC[];

out vec3 fragPosTE;
out vec3 normalTE;
out vec2 texCoordTE;

void main()
{
	fragPosTE = interpolate3D(fragPosTC[0], fragPosTC[1], fragPosTC[2]);
	normalTE = interpolate3D(normalTC[0], normalTC[1], normalTC[2]);
	texCoordTE = interpolate2D(texCoordTC[0], texCoordTC[1], texCoordTC[2]);

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
in vec2 texCoordTE[];

out vec3 fragPosG;
out vec3 normalG;
out vec2 texCoordG;

void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      fragPosG = fragPosTE[i] ;
      normalG = getNormal() ; 
	  texCoordG = texCoordTE[i];
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
in vec2 texCoordG;
in vec3 normalG;

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

uniform sampler2D u_texData;

void main()
{           
    vec3 color = texture(u_texData, texCoordG).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(u_lightPos - fragPosG);
    vec3 normal = normalize(normalG);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(u_viewPos - fragPosG);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    colour = vec4(ambient + diffuse + specular, 1.0);
}