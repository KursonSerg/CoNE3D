layout(std140) uniform CameraBuffer
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec3 position;
} camera;
