#version 330 core

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 fragmentTexCoord;

void main()
{
    gl_Position   = mvpMatrix * vec4(position, 1.0);
    fragmentTexCoord = texCoord;
}