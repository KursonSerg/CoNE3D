#pragma vertex
#version 330 core
#include Buffers/CameraBuffer.glsl

uniform mat4 M;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 fragmentTexCoord;

void main()
{
    gl_Position      = camera.projectionMatrix * camera.viewMatrix * M * vec4(position, 1.0);
    fragmentTexCoord = texCoord;
}
#pragma fragment
#version 330 core

in vec2 fragmentTexCoord;

out vec4 color;

uniform sampler2D sDiffuseMap;

void main()
{
    color = texture(sDiffuseMap, fragmentTexCoord);
}
