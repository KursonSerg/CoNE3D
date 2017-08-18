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
out vec3 Normal_worldspace;
out vec3 CameraDirection_worldspace;

uniform mat4 modelMatrix;

void main()
{
	gl_Position = camera.projectionMatrix * camera.viewMatrix * modelMatrix * vec4(inPosition_modelspace, 1.0);

	UV = inUV;
	Position_worldspace = vec3(modelMatrix * vec4(inPosition_modelspace, 1.0));
	Normal_worldspace = normalize(mat3(modelMatrix) * inNormal_modelspace);
	CameraDirection_worldspace = normalize(camera.position - Position_worldspace);
}
#pragma fragment
#version 330 core
#include Defines.glsl
#include Buffers/MaterialBuffer.glsl
#include Buffers/LightBuffer.glsl

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_worldspace;
in vec3 CameraDirection_worldspace;

out vec3 outColor;

uniform sampler2D sDiffuseMap;
uniform sampler2D sNormalMap;
uniform sampler2D sSpecularMap;

vec3 applyLight(Light light, vec3 surfaceNormal, vec3 surfacePosition, vec3 directionToCamera)
{
	vec3 materialDiffuseColor = vec3(material.useDiffuseTexture != 0 ? texture(sDiffuseMap, UV) : material.diffuseColor);
	vec3 materialSpecularColor = vec3(material.useSpecularTexture != 0 ? texture(sSpecularMap, UV) : material.specularColor);
	vec3 materialAmbientColor = material.ambientColor.rgb * materialDiffuseColor;
	float materialShininess = material.shininess > 0.0 ? material.shininess : 8.0;

	float attenuation = 1.0;
	vec3 directionToLight;
	if (light.type == LIGHT_DIRECTIONAL)
	{
		directionToLight = normalize(light.direction);
	}
	else
	{
		// Distance to the light
		directionToLight = normalize(light.position - surfacePosition);
		float distanceToLight = length(light.position - surfacePosition);

		if (light.type == LIGHT_SPOTLIGHT)
		{
			float lightToSurfaceAngle = degrees(acos(dot(-directionToLight, normalize(light.direction))));
			attenuation = clamp((light.coneAngle - lightToSurfaceAngle)/light.coneAngle, 0.0, 1.0);
		}
		attenuation /= 1.0 + distanceToLight;
	}

	// Cosine of the angle between the normal and the light direction, clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(surfaceNormal, directionToLight), 0.0, 1.0);

	// Direction in which the triangle reflects the light
	vec3 reflection = reflect(-directionToLight, surfaceNormal);
	// Cosine of the angle between the Eye vector and the Reflect vector, clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(directionToCamera, reflection), 0.0, 1.0);

	// simulates indirect lighting
	vec3 ambient = materialAmbientColor * light.ambientIntensity * light.color;
	// color of the object
	vec3 diffuse = materialDiffuseColor * light.diffuseIntensity * light.color * cosTheta;
	// reflective highlight, like a mirror
	vec3 specular = materialSpecularColor * light.color * pow(cosAlpha, materialShininess);

	return ambient + attenuation * (diffuse + specular);
}

void main()
{
	vec3 linearColor = vec3(0.0);
	for (uint i = 0u; i < lights.lightsNumber; ++i)
	{
		linearColor += applyLight(lights.light[i], Normal_worldspace, Position_worldspace, CameraDirection_worldspace);
	}

	float gamma = 2.2;
	outColor = pow(linearColor, vec3(1.0/gamma));
}
