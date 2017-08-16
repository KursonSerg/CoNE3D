#pragma vertex
#version 330 core
#include Locations.glsl
#include Buffers/CameraBuffer.glsl

layout(location = VERTEX_POSITION_LOCATION) in vec3 inPosition_modelspace;
layout(location = VERTEX_UV_LOCATION) in vec2 inUV;
layout(location = VERTEX_NORMAL_LOCATION) in vec3 inNormal_modelspace;
layout(location = VERTEX_TANGENT_LOCATION) in vec3 inTangent_modelspace;
layout(location = VERTEX_BITANGENT_LOCATION) in vec3 inBitangent_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

uniform mat4 modelMatrix;
uniform vec3 lightPosition_worldspace;

void main()
{
	mat4 ModelView = camera.viewMatrix * modelMatrix;
	mat3 ModelView3x3 = mat3(ModelView);

	// Output position of the vertex, in clip space
	gl_Position = camera.projectionMatrix * ModelView * vec4(inPosition_modelspace, 1.0);

	// Position of the vertex, in worldspace
	Position_worldspace = (modelMatrix * vec4(inPosition_modelspace, 1.0)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0, 0, 0).
	vec3 Position_cameraspace = (ModelView * vec4(inPosition_modelspace, 1.0)).xyz;
	EyeDirection_cameraspace = -Position_cameraspace;

	// Vector that goes from the vertex to the light, in camera space.
	// Model matrix is ommited because it's identity.
	vec3 LightPosition_cameraspace = (camera.viewMatrix * vec4(lightPosition_worldspace, 1.0)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	UV = inUV;

	vec3 Tangent_cameraspace = ModelView3x3 * inTangent_modelspace;
	vec3 Bitangent_cameraspace = ModelView3x3 * inBitangent_modelspace;
	Normal_cameraspace = ModelView3x3 * inNormal_modelspace;

	mat3 TBN = transpose(mat3(
		Tangent_cameraspace,
		Bitangent_cameraspace,
		Normal_cameraspace
	)); // You can use dot products instead of building this matrix and transposing it

	LightDirection_tangentspace = normalize(TBN * LightDirection_cameraspace);
	EyeDirection_tangentspace = normalize(TBN * EyeDirection_cameraspace);
}
#pragma fragment
#version 330 core
#include Buffers/MaterialBuffer.glsl

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;

out vec3 outColor;

uniform sampler2D sDiffuseMap;
uniform sampler2D sNormalMap;
uniform sampler2D sSpecularMap;

uniform vec3 lightPosition_worldspace;
uniform vec3 lightColor;
uniform float lightPower;

void main()
{
	// Material properties
	vec3 MaterialDiffuseColor = (material.useDiffuseTexture != 0 ? texture(sDiffuseMap, UV) : material.diffuseColor).rgb;
	vec3 MaterialAmbientColor = material.ambientColor.rgb;
	vec3 MaterialSpecularColor = (material.useSpecularTexture != 0 ? texture(sSpecularMap, UV) : material.specularColor).rgb;

	// Local normal, in tangent space
	vec3 TextureNormal_tangentspace = normalize(texture(sNormalMap, UV).rgb * 2.0 - 1.0);

	// Distance to the light
	float distance = length(lightPosition_worldspace - Position_worldspace);

	// Normal of the computed fragment, in camera space
	vec3 n = material.useNormalTexture != 0 ? TextureNormal_tangentspace : normalize(Normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = material.useNormalTexture != 0 ? LightDirection_tangentspace : normalize(LightDirection_cameraspace);
	// Cosine of the angle between the normal and the light direction, clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = material.useNormalTexture != 0 ? EyeDirection_tangentspace : normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);
	// Cosine of the angle between the Eye vector and the Reflect vector, clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

	outColor =
		// Ambient: simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse: color of the object
		MaterialDiffuseColor * lightColor * lightPower * cosTheta / (distance * distance) +
		// Specular: reflective highlight, like a mirror
		MaterialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (distance * distance);
}
