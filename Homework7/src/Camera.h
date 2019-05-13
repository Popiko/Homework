#ifndef  _CAMERA_H_
#define _CAMERA_H_
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string.h>

#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "Shader.h"
#include "Quaternion.h"

// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera {
public:
	Camera();
	Camera(GLfloat _pfov, GLfloat _pratio, GLfloat _pnear, GLfloat _pfar, glm::vec3 _cameraPos, glm::vec3 _cameraFront, glm::vec3 _cameraUp, Shader& shader);
	~Camera();
	void moveForward(GLfloat const distance);
	void moveBack(GLfloat const distance);
	void moveRight(GLfloat const distance);
	void moveLeft(GLfloat const distance);

	void rotate(GLfloat const pitch, GLfloat const yaw); // 欧拉角
	void rotate_quaternion_glmfunc(GLfloat const delta_pitch, GLfloat const delta_yaw); // 四元数法 内置函数
	void rotate_quaternion_class(GLfloat const delta_pitch, GLfloat const delta_yaw); // 四元数法 类实现
	glm::vec3 getCameraPos(); // 获取摄像机位置
	glm::vec3 getCameraFront(); // 获取摄像机前向位置
	glm::mat4 getViewMatrix(); // 获取viewMatrix
private:
	GLfloat pfov, pratio, pnear, pfar;
	GLfloat cameraPosX, cameraPosY, cameraPosZ;
	GLfloat cameraFrontX, cameraFrontY, cameraFrontZ;
	GLfloat cameraRightX, cameraRightY, cameraRightZ;
	GLfloat cameraUpX, cameraUpY, cameraUpZ;
	glm::mat4 view;
	glm::mat4 projection;
	Shader ourShader;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	void sendToShader();
};




#endif // ! _CAMERA_H_
