#ifndef _Quaternion_H_
#define _Quaternion_H_

// glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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

class Quaternion {
public:
	Quaternion();
	Quaternion(glm::vec3 vec, GLfloat offset);
	Quaternion(GLfloat w, glm::vec3 vec);
	~Quaternion();

	const Quaternion operator*(const Quaternion& q) const;
	const glm::vec3 operator*(const glm::vec3& V) const;
	const Quaternion Conjugate() const;
//private:
	GLfloat w, x, y, z;
	glm::vec3 vector;
	
};

#endif
