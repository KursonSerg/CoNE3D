layout(std140) uniform CameraBuffer
{
    mat4 uProjection;
    mat4 uView;
    vec3 uPosition;
} camera;
