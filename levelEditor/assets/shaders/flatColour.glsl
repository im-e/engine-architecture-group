#region Vertex

#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexColour;

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

uniform mat4 u_model;
out vec3 fragmentColour;

void main()
{
	fragmentColour = a_vertexColour;
	gl_Position =  u_VP * u_model * vec4(a_vertexPosition,1);
}

#region Tessellation Control
#version 450 core
layout(vertices = 3) out;

in vec3 fragmentColour[];
out vec3 fragmentColourTC[];

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
   float eyeToVertexDist1 = distance(u_camPos, fragmentColour[1]);
   float eyeToVertexDist2 = distance(u_camPos, fragmentColour[2]);
   if (gl_InvocationID==0)
   {
		  fragmentColourTC[gl_InvocationID]  = fragmentColour[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }

   // pass through position and normal information
   fragmentColourTC[gl_InvocationID]  = fragmentColour[gl_InvocationID];
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

in vec3 fragmentColourTC[];
out vec3 fragmentColourTE;

void main()
{
	fragmentColourTE = interpolate3D(fragmentColourTC[0], fragmentColourTC[1], fragmentColourTC[2]);

	gl_Position = u_VP * vec4(fragmentColourTE, 1.0);
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

in vec3 fragmentColourTE[];
out vec3 fragmentColourG;

void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      fragmentColourG = fragmentColourTE[i] ;
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

in vec3 fragmentColourG;

void main()
{
	colour = vec4(fragmentColourG, 1.0);
}