#version 400 core
uniform sampler2D 	hitTex;
in Vertex
{
	vec2 texCoord;
	vec3 position;
} IN; 

out vec4 fragColor;

void main(void)
{
	fragColor = vec4(texture(hitTex, IN.texCoord).xyz,0.0f);
}