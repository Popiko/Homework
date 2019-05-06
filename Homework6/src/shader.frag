#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform int chooseFrag;
 
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int nshiness;


//////////////////////////////////////////////
out vec4 FragColor;

in vec3 LightingColor;

//uniform vec3 objectColor;


void main(){
	if(chooseFrag == 0){
		// Ambient
		//float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * lightColor;

		// Diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = diffuseStrength * diff * lightColor;

		// Specular
		//float specularStrength = 0.5f;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), nshiness);
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, 1.0f);
		//FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	}
	else if(chooseFrag == 1){
		FragColor = vec4(LightingColor * objectColor, 1.0);
		//FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	
}
