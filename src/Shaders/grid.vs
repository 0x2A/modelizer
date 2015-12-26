R"(
#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 vertColor;

uniform mat4 MVPMatrix;

void main(void)
{
    gl_Position = MVPMatrix * vec4(position,1);
}
)"