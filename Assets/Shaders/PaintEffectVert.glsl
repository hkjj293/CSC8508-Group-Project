#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

uniform mat4 modelMatrix 	= mat4(1.0f);

out Vertex
{
	vec2 texCoord;
	vec3 position;
} OUT;


void main(void)
{
	OUT.texCoord = texCoord;
	OUT.position = (modelMatrix * vec4(position,1.0)).xyz;
	vec2 screenCoord = texCoord * 2.0;
	screenCoord = vec2(screenCoord.x - 1.0, screenCoord.y - 1.0);
	gl_Position	= vec4(screenCoord.x,screenCoord.y,0.0,1.0);
}