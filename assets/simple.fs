#version 330 core

in vec2 fragmentTexCoord;

out vec4 color;

uniform sampler2D fragmentTexture;

void main()
{
    color = texture(fragmentTexture, fragmentTexCoord);
}