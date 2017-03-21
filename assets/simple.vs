#version 330 core

uniform mat4 mvp_matrix;

in vec3 position;
in vec3 color;

out vec3 fragmentColor;

void main(void)
{
	gl_Position   = mvp_matrix * vec4(position, 1.0);
	fragmentColor = color;
}
