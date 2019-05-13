#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform int chooseVs;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 mycolor;
out vec2 texcoord;

out vec3 FragPos;
out vec3 Normal;

////////////////////
out vec3 LightingColor;


uniform float ambientStrengthG;
uniform float diffuseStrengthG;
uniform float specularStrengthG;
uniform int nshinessG;

uniform vec3 lightPosG;
uniform vec3 viewPosG;
uniform vec3 lightColorG;



void main() {
	if(chooseVs == 0){
		gl_Position = projection * view * model * vec4(position,1.0f);
		FragPos = vec3(model * vec4(position, 1.0f));
		//Normal = normal;
		Normal = mat3(transpose(inverse(model))) * normal;
	}
	else{
		gl_Position = projection * view * model * vec4(position, 1.0f);

		vec3 Position = vec3(model * vec4(position, 1.0));
		vec3 Normal = mat3(transpose(inverse(model))) * normal;

		// ambient
		vec3 ambient = ambientStrengthG * lightColorG;

		// diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPosG - Position);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diffuseStrengthG * diff * lightColorG;

		// specular
		vec3 viewDir = normalize(viewPosG - Position);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), nshinessG);
		vec3 specular = specularStrengthG * spec * lightColorG;

		LightingColor = ambient + diffuse + specular;
	}
	
}