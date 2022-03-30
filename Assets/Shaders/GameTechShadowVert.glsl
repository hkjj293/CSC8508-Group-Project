#version 400 core

uniform mat4 mvpMatrix 		= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

out Vertex
{
	vec2 texCoord;
} OUT;

void main(void)
{
	OUT.texCoord = texCoord;
	gl_Position		= mvpMatrix * vec4(position, 1.0);
}