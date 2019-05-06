#include "Quaternion.h"
#include <iostream>
using namespace std;

Quaternion::Quaternion() {

}

Quaternion::Quaternion(GLfloat _w, glm::vec3 vec) {
	w = _w;
	vector = vec;
	x = vec[0];
	y = vec[1];
	z = vec[2];


}
	
Quaternion::Quaternion(glm::vec3 vec, GLfloat offset) {
	w = cos(offset/2);
	//cout << w << endl;
	vector = vec * sin(offset / 2);
	x = vector[0];
	y = vector[1];
	z = vector[2];
	//cout << x << " " << y << " " << z << endl;
}
Quaternion::~Quaternion() {

}


const Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion r;

	r.w = w*q.w + glm::dot(vector, q.vector);
	
	r.vector = vector * q.w + q.vector * w + glm::cross(vector, q.vector);
	r.x = r.vector[0];
	r.y = r.vector[1];
	r.z = r.vector[2];

	return r;
}



const glm::vec3 Quaternion::operator*(const glm::vec3& V) const
{
	Quaternion p;
	p.w = 0;
	p.vector = V;

	const Quaternion& q = (*this);
	return (q * p * q.Conjugate()).vector;
	
}

const Quaternion Quaternion::Conjugate() const
{
	Quaternion q;
	q.w = w;
	q.vector = -vector;
	return q;
}
