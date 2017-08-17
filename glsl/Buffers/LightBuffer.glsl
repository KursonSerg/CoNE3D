layout(std140) uniform Light
{
	int type;
	float ambientIntensity;
	float diffuseIntensity;
	float coneAngle;
    vec3 position;
    vec3 direction;
    vec3 color;
} light;
