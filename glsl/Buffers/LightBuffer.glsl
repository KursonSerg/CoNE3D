struct Light
{
	int type;
	float ambientIntensity;
	float diffuseIntensity;
	float coneAngle;
    vec3 position;
    vec3 direction;
    vec3 color;
};
layout(std140) uniform Lights
{
	Light light[MAX_LIGHTS];
	uint lightsNumber;
} lights;
