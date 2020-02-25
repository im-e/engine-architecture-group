#region Vertex

#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_model;

out vec3 posVS; 
out vec3 normVS;
out vec2 texCoord;

void main()
{
		texCoord = vec2(a_texCoord.x, a_texCoord.y);
        normVS = aNormals ; 
		posVS = (u_model * vec4(aPos, 1.0)).xyz; 
		gl_Position = u_model *vec4(aPos, 1.0); 
}
				

#region Tessellation Control

// first step of tesselation shader
// tesselation control shader add/deletes control points and determines the tesselatation level
// patch has three control points here (three vertices for each triangle)


#version 450 core
layout (vertices =3) out;


// vectors stored as arrays - each patch has three vertices, each with an xyz pos and xyz norm value 
//posVS = position from Vertex Shader, posTC = position from this Tesselation Control shader

in vec2 texCoord[];
in vec3 posVS[] ;
in vec3 normVS[] ;

out vec3 posTC[] ;
out vec3 normTC[] ;
out vec3 fragPosCS[];
out vec2 tcTexCoord[];

layout(std140) uniform CameraPosition
{
	vec3 u_camPos;
};

float GetTessLevel(float Distance0, float Distance1)
{
	float lambda = -0.5f;
	float alpha = 15f;
	float AverageDist = (Distance0 + Distance1) / 2.0;
	float expValue = ceil(round(exp(lambda*AverageDist) * alpha));
	if(expValue < 1.0f)
	{
		expValue = 1.0f;
		return expValue;
	}
	else
	{
		expValue = ceil(round(exp(lambda*AverageDist) * alpha));
		return expValue;
	}
}

void main()
{
  // tesselation level; we wil find this automatically later but for now hardcoded value
   int tessLevel = 1;

   // gpu can calculate each control point in parallel
   // tesselation level same for entire patch so only need to set it once (for the first control point)
   // gl_invocation called for each control point
   if (gl_InvocationID==0)
   {
		  fragPosCS[gl_InvocationID] = posVS[gl_InvocationID];
		 // fragPosCS[0] = posVS[0];
		  //fragPosCS[1] = posVS[1];
		 // fragPosCS[2] = posVS[2];
		  float eyeToVertexDist0 = distance(u_camPos, posVS[0]);
		  float eyeToVertexDist1 = distance(u_camPos, posVS[1]);
		  float eyeToVertexDist2 = distance(u_camPos, posVS[2]);
		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0); 
          gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
          gl_TessLevelInner[0] = gl_TessLevelOuter[2]; 

   }

   // pass through position and normal information
   posTC[gl_InvocationID]  = posVS[gl_InvocationID] ;
   normTC[gl_InvocationID] = normVS[gl_InvocationID] ;

}


#region Tessellation Eval

#version 450 core

// reading in a triangle, split tesselated triangels evenly in a counter-clockwise direction (ccw)
layout(triangles, fractional_even_spacing, ccw) in;

// forward declare functions to perfrom interpolation with the barycentric coordinates from the Primitive Generator
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) ;
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) ;


// unifrom matrices to perform transformations
// previously this would have been done in vertex shader
layout(std140) uniform Matrices
{
	mat4 u_VP;
};

// read in vector arrays from previous shader
in vec3 posTC[] ;
in vec3 normTC[] ;
in vec2 tcTexCoord[];
in vec3 fragPosCS[];

// pass along the interpolated values
out vec3 normES ;
out vec3 fragPos;
out vec2 teTexCoord;

void main()
{
    // interpolate the normal and xyz position using the linear interpolation function
    // use 3D because they are in three dimensions; 2D also included for uv texture coordinates

	teTexCoord = interpolate2D(tcTexCoord[0], tcTexCoord[1], tcTexCoord[2]);
   normES = interpolate3D(normTC[0], normTC[1], normTC[2]) ;
   vec3 posES = interpolate3D(posTC[0], posTC[1], posTC[2]) ;
   fragPos = interpolate3D(fragPosCS[0], fragPosCS[1], fragPosCS[2]);

   // transform vertex to clip space  - NOTE: WE NEED TO DO IT HERE NOW and not in vertex shader
   gl_Position = u_VP * vec4(posES, 1.0);
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
layout(triangles) in ;
layout(triangle_strip, max_vertices = 3) out ;
vec3 getNormal() ;

in vec3 fragPos[] ;
//in vec3 normals[] ;
in vec2 teTexCoord[];

out vec3 gNormals ;
out vec3 gWorldPos_FS_in ;
out vec2 gTexCoord;

void main()
{
  
   for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      gWorldPos_FS_in = fragPos[i] ;
      gNormals = getNormal() ; 
	  gTexCoord = teTexCoord[i];
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

#version 330 core
out vec4 FragColor;

in vec3 gNormals ;
in vec3 gWorldPos_FS_in ;
in vec2 gTexCoord;
                                                                       
layout(std140) uniform TessLightUBO
{
	vec3 u_lightPos;
	vec3 u_ambient;
	vec3 u_diffuse;
	vec3 u_specular;
	vec3 u_viewPos; 
	vec3 u_lightColour;
};

//uniform sampler2D texture1;
//uniform DirLight dirLight;
//uniform Material mat ;
//uniform vec3 viewPos ;

uniform sampler2D u_texData;

void main()
{   
    
  
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * u_lightColour;
	
	vec3 norm = normalize(gNormals);
	vec3 lightDir = normalize(u_lightPos - gWorldPos_FS_in);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColour;
	
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_viewPos - gWorldPos_FS_in);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * u_lightColour;  
	
	FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(u_texData, gTexCoord);

	
}