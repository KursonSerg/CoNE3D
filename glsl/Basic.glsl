#pragma vertex
#version 330 core
#include Defines.glsl
#include Buffers/CameraBuffer.glsl

uniform mat4 modelMatrix;

layout(location = VERTEX_POSITION_LOCATION) in vec3 inPosition;
layout(location = VERTEX_UV_LOCATION) in vec2 inUV;

out vec2 UV;

void main()
{
    gl_Position = camera.projectionMatrix * camera.viewMatrix * modelMatrix * vec4(inPosition, 1.0);
    UV = inUV;
}
#pragma fragment
#version 330 core
#include Buffers/MaterialBuffer.glsl

in vec2 UV;

out vec4 outColor;

uniform sampler2D sDiffuseMap;

void main()
{
    outColor = material.useDiffuseTexture != 0 ? texture(sDiffuseMap, UV) : material.diffuseColor;
}
