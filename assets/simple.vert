#version 330 core

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 fragmentTexCoord;

void main()
{
    gl_Position      = P * V * M * vec4(position, 1.0);
    fragmentTexCoord = texCoord;
}
