#version 330 core

uniform mat4 mvpMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 fragmentColor;
out vec2 fragmentTexCoord;

void main()
{
    gl_Position   = mvpMatrix * vec4(position, 1.0);
    fragmentColor = color;
    fragmentTexCoord = texCoord;
}