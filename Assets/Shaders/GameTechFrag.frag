#version 450 core

layout(binding = 0) uniform FragBufferObject{
	vec4 		objectColour;
	vec3	lightPos;
	float	lightRadius;
	vec4	lightColour;
	vec3	cameraPos;
	bool hasTexture;
} fbo;

layout(binding = 1) uniform	sampler2D 	mainTex;
layout(binding = 1) uniform	sampler2DShadow shadowTex;


layout(location = 0) in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
} IN;

layout(location = 0) out vec4 fragColor;

void main(void)
{
	float shadow = 1.0; // New !
	
	if( IN.shadowProj.w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN.shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( fbo.lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( fbo.cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );
	
	vec4 albedo = IN.colour;
	
	if(fbo.hasTexture) {
	 albedo *= texture(mainTex, IN.texCoord);
	}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * fbo.lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += fbo.lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;

//fragColor.rgb = IN.normal;

	//fragColor = IN.colour;
	
	//fragColor.xy = IN.texCoord.xy;
	
	//fragColor = IN.colour;
}