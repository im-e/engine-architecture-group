#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 fragPosV;
out vec3 normalV;
out vec2 texCoordsV;


layout (std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;

void main()
{
    fragPosV = vec3(u_model * vec4(aPos, 1.0));
    normalV = mat3(transpose(inverse(u_model))) * aNormal;  
    texCoordsV = aTexCoords;
    
    gl_Position = u_VP * vec4(fragPosV, 1.0);
}

#region Tessellation Control
#version 450 core
layout(vertices = 3) out;

in vec3 fragPosV[];
in vec3 normalV[];
in vec2 texCoordsV[];

out vec3 fragPosTC[];
out vec3 normalTC[];
out vec2 texCoordsTC[];

layout(std140) uniform CameraPosition
{
	vec3 u_camPos;
	vec3 u_camForward;
	float u_cutOff;
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
   normalTC[gl_InvocationID] = normalV[gl_InvocationID];
   texCoordsTC[gl_InvocationID] = texCoordsV[gl_InvocationID];
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
in vec2 texCoordsTC[];

out vec3 fragPosTE;
out vec3 normalTE;
out vec2 texCoordTE;

void main()
{
	fragPosTE = interpolate3D(fragPosTC[0], fragPosTC[1], fragPosTC[2]);
	normalTE = interpolate3D(normalTC[0], normalTC[1], normalTC[2]);
	texCoordTE = interpolate2D(texCoordsTC[0], texCoordsTC[1], texCoordsTC[2]);

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
			
layout(location = 0) out vec4 FragColor;

in vec3 fragPosG;  
in vec2 texCoordG;
in vec3 normalG;

layout(std140) uniform CameraPosition
{
	vec3 u_camPos;
	vec3 u_camForward;
	float u_cutOff;
};

layout(std140) uniform Light
{
	vec3 u_lightPos; 
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

vec3 position = u_camPos;
vec3 direction = u_camForward;
float outerCutOff;

uniform sampler2D u_texData;
uniform sampler2D u_specularTexData;

void main()
{
	vec3 lightDir = normalize(position - fragPosG);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(direction)); 
    
    if(theta > u_cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
	{
	vec3 color = texture(u_texData, texCoordG).rgb;
    // ambient
    vec3 ambient = 0.25 * color;  
	
    // diffuse 
    vec3 norm = normalize(normalG);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color; 
    
    // specular
	vec3 specularTex = 0.6 * texture(u_specularTexData, texCoordG).rgb;
    vec3 viewDir = normalize(u_viewPos - fragPosG);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularTex * spec; 
    
	float constant = 1.0;
	float linear = 0.09;
	float quadratic = 0.032;

    // attenuation
    float distance    = length(position - fragPosG);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    
 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
	}
	else
	{
		vec3 color = texture(u_texData, texCoordG).rgb;
		// ambient
		vec3 ambient = 0.25 * color; 
        FragColor = vec4(ambient, 1.0);
	}
} 