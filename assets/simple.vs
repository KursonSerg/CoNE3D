#version 330 core

uniform mat4 mvpMatrix;

in vec3 position;
in vec3 color;

out vec3 fragmentColor;

void main(void)
{
	gl_Position   = mvpMatrix * vec4(position, 1.0);
	fragmentColor = color;
}
