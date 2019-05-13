#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

const int NUM_CASCADES = 3;

out vec2 TexCoords;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	vec4 FragPosLightSpace_CSM[NUM_CASCADES];
	float ClipSpacePosZ;
} vs_out;

uniform int csm_flag_vs;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform mat4 lightSpaceMatrix_CSM[NUM_CASCADES];

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.ClipSpacePosZ = gl_Position.z;
	vs_out.FragPos = vec3(model * vec4(position, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;
	vs_out.TexCoords = texCoords;
	if(csm_flag_vs == 0){
		vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	}
	else{
		for(int i = 0; i < NUM_CASCADES; i++){
			vs_out.FragPosLightSpace_CSM[i] = lightSpaceMatrix[i] * vec4(vs_out.FragPos, 1.0);
		}
	}
}