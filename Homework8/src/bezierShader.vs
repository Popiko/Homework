#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int animation;

out vec3 mycolor;

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0f);
	if(animation == 0){
		mycolor = vec3(1.0f, 0.0f, 0.0f);
	}
	else{
		mycolor = vec3(0.0f, 0.0f, 1.0f);
	}
	
}
	