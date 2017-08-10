#pragma vertex
#version 330 core
#include Defines.glsl
#include Buffers/CameraBuffer.glsl

uniform mat4 uModel;

layout(location = VERTEX_POSITION_LOCATION) in vec3 inPosition;
layout(location = VERTEX_UV_LOCATION) in vec2 inUV;

out vec2 UV;

void main()
{
    gl_Position = camera.uProjection * camera.uView * uModel * vec4(inPosition, 1.0);
    UV = inUV;
}
#pragma fragment
#version 330 core

in vec2 UV;

out vec4 outColor;

uniform sampler2D sDiffuseMap;

void main()
{
    outColor = texture(sDiffuseMap, UV);
}
