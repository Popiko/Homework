#include "Camera.h"
#include <iostream>
#include "glm/gtx/quaternion.hpp"
using namespace std;
Camera::Camera(){}
Camera::Camera(GLfloat _pfov, GLfloat _pratio, GLfloat _pnear, GLfloat _pfar, glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp,  Shader& shader) {
	pfov = _pfov;
	pratio = _pratio;
	pnear = _pnear;
	pfar = _pfar;
	cameraPosX = cameraPos[0]; // 这样写不知道可不可以 可以
	cameraPosY = cameraPos[1];
	cameraPosZ = cameraPos[2];
	cout << "CameraPosX " << cameraPosX << endl;
	cout << "CameraPosY " << cameraPosY << endl;
	cout << "CameraPosZ " << cameraPosZ << endl;
	cameraFrontX = cameraFront[0];
	cameraFrontY = cameraFront[1];
	cameraFrontZ = cameraFront[2];
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	cameraRightX = cameraRight[0];
	cameraRightY = cameraRight[1];
	cameraRightZ = cameraRight[2];
	cameraUpX = cameraUp[0];
	cameraUpY = cameraUp[1];
	cameraUpZ = cameraUp[2];
	cameraPos = _cameraPos;
	cameraFront = _cameraFront;
	cameraUp = _cameraUp;
	ourShader = shader;

	sendToShader();
	// 我说刚刚怎么老是显示不出来原来是model没有初始化
	

}
Camera::~Camera() {

}
void Camera::moveForward(GLfloat const distance) {
	cameraPos += distance * cameraFront;
	sendToShader();
	
}
void Camera::moveBack(GLfloat const distance) {
	cameraPos -= distance * cameraFront;
	sendToShader();
}
void Camera::moveRight(GLfloat const distance) {
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
	sendToShader();
}
void Camera::moveLeft(GLfloat const distance) {
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
	sendToShader();
}

void Camera::rotate(GLfloat const pitch, GLfloat const yaw) {
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);

	sendToShader();
}

void Camera::rotate_quaternion_glmfunc(GLfloat const pitch, GLfloat const yaw) {
	glm::vec3 axis = glm::cross(cameraFront, cameraUp);
	//cout << cos(pitch) << endl;
	glm::quat pitch_quat = glm::angleAxis(glm::radians(pitch), axis);
	glm::quat yaw_quat = glm::angleAxis(glm::radians(-yaw), cameraUp);
	glm::quat temp = glm::cross(yaw_quat, pitch_quat);
	temp = glm::normalize(temp);
	cameraFront = glm::rotate(temp, cameraFront);
	sendToShader();



}

void Camera::rotate_quaternion_class(GLfloat const delta_pitch, GLfloat const delta_yaw) {
	Quaternion pitch_quaternion(glm::cross(cameraFront, cameraUp), glm::radians(delta_pitch));
	Quaternion yaw_quaternion(-cameraUp, glm::radians(delta_yaw));
	Quaternion tmp = pitch_quaternion * yaw_quaternion;
	cameraFront = tmp * cameraFront;
	sendToShader();
	
}


void Camera::sendToShader() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	//cout << "Initialize" << endl;
	projection = glm::perspective(glm::radians(pfov), pratio, pnear, pfar);
	glm::mat4  model = glm::mat4(1.0f);
	ourShader.Use();

	GLuint viewLocation = glGetUniformLocation(ourShader.Program, "view");
	GLuint projectionLocation = glGetUniformLocation(ourShader.Program, "projection");
	GLuint modelLocation = glGetUniformLocation(ourShader.Program, "model");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

glm::vec3 Camera::getCameraPos() {
	return cameraPos;
}

glm::vec3 Camera::getCameraFront() {
	return cameraFront;
}


	/*GLfloat pfov, pratio, pnear, pfar;
	GLfloat cameraPosX, cameraPosY, cameraPosZ;
	GLfloat cameraFrontX, cameraFrontY, cameraFrontZ;
	GLfloat cameraRightX, cameraRightY, cameraRightZ;
	GLfloat cameraUpX, cameraUpY, cameraUpZ;*/
