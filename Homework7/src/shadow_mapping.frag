#version 330 core
out vec4 FragColor;


const int NUM_CASCADES = 3;

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	vec4 FragPosLightSpace_CSM[NUM_CASCADES];
	float ClipSpacePosZ;
} fs_in;


uniform int csm_flag_fs;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D shadowMap_CSM[NUM_CASCADES];
uniform float gCascadeEndClipSpace[NUM_CASCADES];

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float bias;
uniform int pcf_flag;
uniform int over_sample_flag;

float ShadowCalculationCSM(int index, vec4 fragPosLightSpace){
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	projCoords = projCoords*0.5 + 0.5;

	float closestDepth = texture(shadowMap_CSM[index], projCoords.xy).r;
	float currentDepth = projCoords.z;

	if(closestDepth < currentDepth + 0.00001){
		return 0.5;
	}
	else {
		return 1.0;
	}

}

float ShadowCalculation(vec4 fragPosLightSpace){
	
	// divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	//float bias = 0.05;
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = 0.0;
	if(pcf_flag == 1){
		vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
			}    
		}
		shadow /= 9.0;
	}
	else{
		shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	}
	
	//float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	if(over_sample_flag == 1){
		if(projCoords.z > 1.0)
			shadow = 0.0;
	}
	
	return shadow;

}

void main(){
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb; // what is diffuseTexture
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0);
	//vec3 lightColor = vec3(1.0f, 0.0f, 0.0f);


	// Ambient
	vec3 ambient = 0.15 * color;

	// Diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir); //blinn-phong
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	// calculate the shadow
	if(csm_flag_fs == 0){
		float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
		vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
		/*if(shadow > 0.0){
			FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}*/
		//else{
			FragColor = vec4(lighting, 1.0f);
		//}
	}
	else{
		float shadow = 0.0;
		int kk = 0;
		for(kk = 0; kk < NUM_CASCADES; kk++){
			if(fs_in.ClipSpacePosZ <= gCascadeEndClipSpace[kk]){
				shadow = ShadowCalculationCSM(kk, fs_in.FragPosLightSpace_CSM[kk]);
				break;
			}
		}
		vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
		/*if(shadow > 0.5){
			if(kk  == 0){
				float r_intensity = 1.0f;
				FragColor = vec4(r_intensity, 0.0f, 0.0f, 1.0f);
			}
			else if(kk  == 1){
			
				FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else if(kk  == 2){
			
				FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
			}
		}*/
		
		//else{
			FragColor = vec4(lighting, 1.0f);
		//}
	}
	
	

}