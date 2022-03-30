#version 450 core
layout(binding = 0) uniform uniformBuffer{
 mat4 VPMatrix;
} ub;

layout( push_constant ) uniform constants{
	mat4 MMatrix;
} PushConstants;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 UVPosition;
layout(location = 3) in vec3 Normal;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 UV;
layout(location = 2) out vec3 normal;


void main() {
    gl_Position = ub.VPMatrix * PushConstants.MMatrix * vec4(position, 1.0);
    fragColor = vec4(1,0,1,1);
    UV = UVPosition;
    normal = Normal;
}