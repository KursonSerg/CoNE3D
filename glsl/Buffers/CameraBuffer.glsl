layout(std140) uniform Camera
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 position;
} camera;
