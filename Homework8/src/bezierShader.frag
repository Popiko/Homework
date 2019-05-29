#version 330 core

in vec3 mycolor;

out vec4 FragColor;

void main(){
	FragColor = vec4(mycolor, 1.0f);
}