R"(
#version 130


uniform mat4 VPMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;
out vec4 vertColor;
out vec3 vertNormal;
out vec3 vertPos;

void main(void)
{
    gl_Position = VPMatrix * ModelMatrix * gl_Vertex;
	
	mat4 mvMatrix = ViewMatrix * ModelMatrix;
	vertNormal = (mvMatrix * vec4(gl_Normal,0)).xyz;
	vec4 vPos = mvMatrix * gl_Vertex;
	vertPos = vPos.xyz / vPos.w;
	texCoord = gl_MultiTexCoord0.xy;
	vertColor = gl_Color;
}
)"