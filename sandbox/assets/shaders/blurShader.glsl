#region Vertex

#version 440 core
			
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 fragmentPos;


void main()
{
    TexCoords = aTexCoords;
	fragmentPos = aPos;
    gl_Position = vec4(fragmentPos, 1.0); 
}

#region Tessellation Control
#version 450 core
layout(vertices = 3) out;

in vec2 TexCoords[];
in vec3 fragmentPos[];
out vec2 TexCoordsTC[];
out vec3 fragmentPosTC[];

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
	tess = 5.0f;
	return tess;
}

void main()
{
   float eyeToVertexDist1 = distance(u_camPos, fragmentPos[1]);
   float eyeToVertexDist2 = distance(u_camPos, fragmentPos[2]);
   if (gl_InvocationID==0)
   {
		  fragmentPosTC[gl_InvocationID]  = fragmentPos[gl_InvocationID];

		   // Calculate the tessellation levels
          gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2); 
          gl_TessLevelOuter[1] = gl_TessLevelOuter[0]; 
          gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
          gl_TessLevelInner[0] = gl_TessLevelOuter[0]; 

   }

   // pass through position and normal information
   fragmentPosTC[gl_InvocationID]  = fragmentPos[gl_InvocationID];
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
in vec3 fragmentPosTC[];
out vec2 TexCoordsTE;
out vec3 fragmentPosTE;

void main()
{
	fragmentPosTE = interpolate3D(fragmentPosTC[0], fragmentPosTC[1], fragmentPosTC[2]);

	gl_Position = u_VP * vec4(fragmentPosTE, 1.0);
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

in vec2 TexCoordsTE[];
in vec3 fragmentPosTE[];
out vec2 TexCoordsG;
out vec3 fragmentPosG;

void main()
{
	for(int i = 0 ; i < 3; i++)
   {
      gl_Position = gl_in[i].gl_Position ;
      fragmentPosG = fragmentPosTE[i];
	  TexCoordsG = TexCoordsTE[i];
      EmitVertex() ;
  }
     EndPrimitive() ;
}
				
#region Fragment

#version 440 core

out vec4 colour;

in vec2 TexCoordsG;
in vec3 fragmentPosG;

uniform sampler2D u_colourTexture;

const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_colourTexture, TexCoordsG.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    colour = vec4(col, 1.0);
} 