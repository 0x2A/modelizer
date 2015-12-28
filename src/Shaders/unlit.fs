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

out vec4 colorOut;

void main(void)
{
    colorOut = vertColor * diffuseColor * mix(vec4(1.0f), texture2D(diffuseTexture, texCoord), hasTexture);
}
)"