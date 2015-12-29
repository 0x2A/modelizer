R"(
#version 130

uniform sampler2D diffuseTexture;
uniform float hasTexture;

uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec4 ambientColor;
uniform vec4 emissiveColor;
uniform float shininess;


in vec2 texCoord;
in vec4 vertColor;
in vec3 vertNormal;
in vec3 vertPos;

out vec4 colorOut;


const vec3 lightPos = vec3(0.25, 0.25, 1.0); //set light position constant so it moves with camera

void main(void)
{
	vec3 normal = normalize(vertNormal);
	vec3 lightDir = normalize(lightPos - vertPos);
	float lambertian = max(dot(lightDir,normal), 0.0);
	
	vec3 viewDir = normalize(-vertPos);
	
	 // this is blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    float specular = pow(specAngle, shininess);
	
	vec4 texture = mix(vec4(1.0f), texture2D(diffuseTexture, texCoord), hasTexture);
	vec4 albedo = vertColor * diffuseColor * texture;
	
	vec4 finalColor = /*ambientColor + */lambertian * albedo + specular * specularColor + emissiveColor;
	finalColor = min(finalColor, vec4(1.0));
    colorOut = vec4(finalColor.xyz,texture.a);
}
)"