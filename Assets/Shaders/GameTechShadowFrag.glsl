#version 400 core

uniform sampler2D 	mainTex;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	vec4 colour = texture(mainTex, IN.texCoord);
	if(colour.a < 0.5f) discard;
	fragColor = colour;
}