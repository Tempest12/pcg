#include <GL/glut.h>

#include "core/Camera.hpp"

#include "math/Vector3f.hpp"

//This namespace this codes belongs to:
using namespace Core;

Camera::Camera(void)
{
	this->position = Math::Vector3f();

	this->focalPoint = Math::Vector3f();

	this->upVector = Math::Vector3f();
}

Camera::Camera(float x, float y, float z, float focalX, float focalY, float focalZ)
{
	this->position = Math::Vector3f(x, y, z);

	this->focalPoint = Math::Vector3f(focalX, focalY, focalZ);

	this->upVector = Math::Vector3f(0.0f, 1.0f, 0.0f);
}

void Camera::ApplyTransformation(void)
{
	gluLookAt(this->position.x  , this->position.y  , this->position.z,
			  this->focalPoint.x, this->focalPoint.y, this->focalPoint.z,
			  this->upVector.x  , this->upVector.y  , this->upVector.z);
}