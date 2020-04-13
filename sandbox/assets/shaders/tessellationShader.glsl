#region Vertex
#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 u_model;

out vec3 posVS;
out vec3 normVS;
out vec2 texCoordsVS;

void main()
{
	texCoordsVS = aTexCoords;
	normVS = aNormals;
	posVS = (u_model * vec4(aPos, 1.0)).xyz;
	gl_Position = u_model * vec4(aPos, 1.0);
}

#region Tessellation Control
#version 450 core

layout (vertices = 3) out;

in vec3 posVS[];
in vec3 normVS[];
in vec2 texCoords[];

out vec3 posTC[];
out vec3 normTC[];
out vec3 fragPosTC[];
out vec2 texCoordsTC[];

layout(std140) uniform CameraPosition
{
	vec3 u_cameraPosition;
};

float GetTessLevel(float, float);

void main()
{
	if(gl_InvocationID == 0)
	{
		fragPosTC[gl_InvocationID] = posVS[gl_InvocationID];
		float eyeToVertexDistance0 = distance(u_cameraPosition, posVS[0]);
		float eyeToVertexDistance1 = distance(u_cameraPosition, posVS[1]);
		float eyeToVertexDistance2 = distance(u_cameraPosition, posVS[2]);
		gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDistance1, eyeToVertexDistance2);
		gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDistance2, eyeToVertexDistance0);
		gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDistance0, eyeToVertexDistance1);
		gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}
	posTC[gl_InvocationID] = posVS[gl_InvocationID];
	normTC[gl_InvocationID] = normVS[gl_InvocationID];
}

float GetTessLevel(float Distance0, float Distance1)
{
	float lambda = -0.5f;
	float alpha = 15f;
	float AvgDist = (Distance0 + Distance1) / 2.0;
	float expValue = ceil(round(exp(lambda*AvgDist) * alpha));
	if(expValue < 1.0f)
	{
		expValue = 1.0f;
		return expValue;
	}
	else
	{
		expValue = ceil(round(exp(lambda*AvgDist) * alpha));
		return expValue;
	}
}

#region Tessellation Eval
#version 450 core

layout (triangles, fractional_even_spacing, ccw) in;
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);

layout(std140) uniform Matrices
{
	mat4 u_VP;
};

in vec3 posTC[];
in vec3 normTC[];
in vec3 fragPosTC[];
in vec2 texCoordsTC[];

out vec3 normTE;
out vec3 posTE;
out vec3 fragPosTE;
out vec2 texCoordsTE;

void main()
{
	posTE = interpolate3D(posTC[0], posTC[1], posTC[2]);
	normTE = interpolate3D(normTC[0], normTC[1], normTC[2]);
	fragPosTE = interpolate3D(fragPosTC[0], fragPosTC[1], fragPosTC[2]);
	texCoordsTE = interpolate2D(texCoordsTC[0], texCoordsTC[1], texCoordsTC[2]);
	gl_Position = u_VP * vec4(posTE, 1.0);
}

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

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

vec3 getNormal();

in vec3 posTE[];
in vec3 normTE[];
in vec2 texCoordsTE[];

out vec3 posG;
out vec3 normG;
out vec2 texCoordsG;

void main()
{
	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		posG = posTE[i];
		normG = getNormal();
		texCoordsG = texCoordsTE[i];
		EmitVertex();
	}
	EndPrimitive();
}

vec3 getNormal()
{
	vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	return normalize(cross(a, b));
}

#region Fragment
#version 330 core

in vec3 posG;
in vec3 normG;
in vec2 texCoordsG;

out vec4 FragColor;

layout(std140) uniform TessLightUBO
{
	vec3 u_lightPos;
	vec3 u_ambient;
	vec3 u_diffuse;
	vec3 u_specular;
	vec3 u_lightColour;
	vec3 u_cameraPos;
};

uniform sampler2D u_textureData;

void main()
{
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * u_lightColour;
	vec3 norm = normalize(normG);
	vec3 lightDir = normalize(u_lightPos - posG);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColour;
	
	float specularStrength = 0.8;
	vec3 viewDir = normalize(u_cameraPos - posG);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * u_lightColour;
	FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(u_textureData, texCoordsG);
}