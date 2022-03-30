#version 400 core

uniform sampler2D 	mainTex;

uniform float dt;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	 fragColor = texture(mainTex, IN.texCoord + vec2(sin(dt)*0.1,0));	
}