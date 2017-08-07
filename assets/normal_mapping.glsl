#pragma vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Position_worldspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

layout(std140) uniform shader_data
{ 
	vec4 camera_position;
	vec4 light_position;
	vec4 light_diffuse;
};

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =	MVP * vec4(vertexPosition_modelspace, 1.0);

	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(vertexPosition_modelspace, 1.0)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0.0, 0.0, 0.0).
	vec3 vertexPosition_cameraspace = (V * M * vec4(vertexPosition_modelspace, 1.0)).xyz;
	EyeDirection_cameraspace = -vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1.0)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// UV of the vertex. No special space for this one.
	UV = vertexUV;

	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
	vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
	vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirection_tangentspace = normalize(TBN * LightDirection_cameraspace);
	EyeDirection_tangentspace =	normalize(TBN * EyeDirection_cameraspace);
}
#pragma fragment
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D sDiffuseMap;
uniform sampler2D sNormalMap;
//uniform sampler2D sSpecularMap;

uniform vec3 LightPosition_worldspace;
uniform float LightPower;

void main()
{
	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(1.0, 1.0, 1.0);

	// Material properties
	vec3 MaterialDiffuseColor = texture(sDiffuseMap, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3); //texture(sSpecularMap, UV).rgb * 0.3;

	// Local normal, in tangent space
	vec3 TextureNormal_tangentspace = normalize(texture(sNormalMap, UV).rgb * 2.0 - 1.0);

	// Distance to the light
	float distance = length(LightPosition_worldspace - Position_worldspace);

	// Normal of the computed fragment, in camera space
	vec3 n = TextureNormal_tangentspace;
	// Direction of the light (from the fragment to the light)
	vec3 l = LightDirection_tangentspace;
	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//	- light is at the vertical of the triangle -> 1
	//	- light is perpendicular to the triangle -> 0
	//	- light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	// Eye vector (towards the camera)
	vec3 E = EyeDirection_tangentspace;
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//	- Looking into the reflection -> 1
	//	- Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	color =
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}
