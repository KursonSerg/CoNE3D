layout(std140) uniform Material
{
	vec4 diffuseColor;
	vec4 specularColor;
	vec4 ambientColor;

	int useDiffuseTexture;
	int useSpecularTexture;
	int useNormalTexture;
} material;
