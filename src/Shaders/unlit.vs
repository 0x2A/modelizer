R"(
#version 130


uniform mat4 VPMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;
out vec4 vertColor;

void main(void)
{
    gl_Position = VPMatrix * ModelMatrix * gl_Vertex;
	texCoord = gl_MultiTexCoord0.xy;
	vertColor = gl_Color;
}
)"