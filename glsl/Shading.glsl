#pragma vertex
#version 330 core
#include Defines.glsl
#include Buffers/CameraBuffer.glsl

layout(location = VERTEX_POSITION_LOCATION) in vec3 inPosition_modelspace;
layout(location = VERTEX_UV_LOCATION) in vec2 inUV;
layout(location = VERTEX_NORMAL_LOCATION) in vec3 inNormal_modelspace;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform mat4 uModel;
uniform vec3 uLightPosition_worldspace;

void main()
{
	mat4 ModelView = camera.uView * uModel;

	// Output position of the vertex, in clip space
	gl_Position = camera.uProjection * ModelView * vec4(inPosition_modelspace, 1.0);

	// Position of the vertex, in worldspace
	Position_worldspace = (uModel * vec4(inPosition_modelspace, 1.0)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0, 0, 0).
	vec3 Position_cameraspace = (ModelView * vec4(inPosition_modelspace, 1.0)).xyz;
	EyeDirection_cameraspace = -Position_cameraspace;

	// Vector that goes from the vertex to the light, in camera space.
	// Model matrix is ommited because it's identity.
	vec3 LightPosition_cameraspace = (camera.uView * vec4(uLightPosition_worldspace, 1.0)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// Normal of the the vertex, in camera space
	// Only correct if model matrix does not scale the model! Use its inverse transpose if not.
	Normal_cameraspace = (ModelView * vec4(inNormal_modelspace, 0.0)).xyz;

	UV = inUV;
}
#pragma fragment
#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 outColor;

uniform sampler2D sDiffuseMap;

uniform vec3 uLightPosition_worldspace;
uniform vec3 uLightColor;
uniform float uLightPower;

void main()
{
	// Material properties
	vec3 MaterialDiffuseColor = texture(sDiffuseMap, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

	// Distance to the light
	float distance = length(uLightPosition_worldspace - Position_worldspace);

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(Normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection_cameraspace);
	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

	outColor =
		// Ambient: simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse: color of the object
		MaterialDiffuseColor * uLightColor * uLightPower * cosTheta / (distance * distance) +
		// Specular: reflective highlight, like a mirror
		MaterialSpecularColor * uLightColor * uLightPower * pow(cosAlpha, 5) / (distance * distance);
}
