#version 400 core

uniform sampler2D 	hitTex;
uniform vec3		hitPos;
uniform int			isInit;

in Vertex
{
	vec2 texCoord;
	vec3 position;
} IN; 

out vec4 fragColor;

void main(void)
{
	if(isInit == 0){
		vec3 d = IN.position - hitPos;
		float AngleZ = abs(atan(d.y/d.x));
		float AngleY = abs(atan(d.x/d.z));
		float AngleX = abs(atan(d.z/d.y));
		float s = 0;
		float radius = 1.0;
		int r = 5;
		for(int i = 1; i <= r ; i++){
			s+=(sin(i*AngleZ)+sin(i*AngleY)+sin(i*AngleX))*0.333;
		}
		s = s / float(r);
		if(abs(length(d)) > (s + 2) * radius) discard;
		//vec4 colour = texture(hitTex, IN.texCoord);
		//colour = colour + vec4(1,0,0,0); //if(distance(hitPos,IN.position)<1.0f)  
		fragColor = vec4(texture(hitTex, IN.texCoord).xyz,1.0f);//*0.9f + vec4(1,0,0,1) * 0.1f;
		fragColor.r = clamp(fragColor.r,0,1);
	}else{
		fragColor = vec4(texture(hitTex, IN.texCoord).xyz,0.0f);
	}
	
	
}