#version 450 core

layout(binding = 0) uniform UniformBufferObject{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
	mat4 shadowMatrix;
} ubo;

layout(binding = 0) uniform ColourUniformBufferObject {
	vec4 objectColour;
	bool hasVertexColours;
} colourBuff;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;


layout(location = 0) out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} OUT;

void main(void)
{
	mat4 mvp 		  = (ubo.projMatrix * ubo.viewMatrix * ubo.modelMatrix);
	mat3 normalMatrix = transpose ( inverse ( mat3 ( ubo.modelMatrix )));

	OUT.shadowProj 	=  ubo.shadowMatrix * vec4 ( position,1);
	OUT.worldPos 	= ( ubo.modelMatrix * vec4 ( position ,1)). xyz ;
	OUT.normal 		= normalize ( normalMatrix * normalize ( normal ));
	
	OUT.texCoord	= texCoord;
	OUT.colour		= colourBuff.objectColour;

	if(colourBuff.hasVertexColours) {
		OUT.colour		= colourBuff.objectColour * colour;
	}
	gl_Position		= mvp * vec4(position, 1.0);
}