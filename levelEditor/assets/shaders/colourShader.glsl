#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 fragPos;

void main()
{
    TexCoords = aTexCoords;
	fragPos = aPos;
    gl_Position = vec4(fragPos, 1.0); 
}
				
#region Tessellation Control
#version 450 core
layout(vertices = 3) out;

in vec2 TexCoords[];
in vec3 fragPos[];
out vec2 TexCoordsTC[];
out vec3 fragPosTC[];

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
   float eyeToVertexDist1 = distance(u_camPos, fragPos[1]);
   float eyeToVertexDist2 = distance(u_camPos, fragPos[2]);
   if (gl_InvocationID==0)
   {
		  fragPosTC[gl_InvocationID]  = fragPos[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }

   // pass through position and normal information
   fragPosTC[gl_InvocationID]  = fragPos[gl_InvocationID];
   TexCoordsTC[gl_InvocationID] = TexCoords[gl_InvocationID];
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

in vec2 TexCoordsTC[];
in vec3 fragPosTC[];
out vec2 TexCoordsTE;
out vec3 fragPosTE;

void main()
{
	fragPosTE = interpolate3D(fragPosTC[0], fragPosTC[1], fragPosTC[2]);
	TexCoordsTE = interpolate2D(TexCoordsTC[0], TexCoordsTC[1], TexCoordsTC[2]);
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

in vec3 fragPosTE[];
in vec2 TexCoordsTE[];
out vec3 fragPosG;
out vec2 TexCoordsG;

void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      fragPosG = fragPosTE[i];
	  TexCoordsG = TexCoordsTE[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}

#region Fragment

#version 440 core

out vec4 colour;

in vec2 TexCoordsG;
in vec3 fragPosG;

uniform sampler2D u_colourTexture;

void main()
{           
    vec3 col = texture(u_colourTexture, TexCoordsG).rgb;
    colour = vec4(col, 1.0);
}