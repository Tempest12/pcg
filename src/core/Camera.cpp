#include <math.h>
#include <GL/glut.h>

#include "core/Camera.hpp"
#include "math/Vector3f.hpp"
#include "math/Util.hpp"

//This namespace this codes belongs to:
using namespace Core;

Camera::Camera(void)
{
	this->horizontalAngle = 270.0f;
	this->verticalAngle = 90.0f;

	this->direction = Math::Vector3f();
	this->focalPoint = Math::Vector3f();
	this->position = Math::Vector3f();
	this->upVector = Math::Vector3f();

	this->calculateDirection();
}

Camera::Camera(float x, float y, float z)
{
	this->horizontalAngle = 270.0f;
	this->verticalAngle = 90.0f;

	this->position = Math::Vector3f(x, y, z);
	this->direction = Math::Vector3f();
	this->focalPoint = Math::Vector3f();
	this->upVector = Math::Vector3f(0.0f, 1.0f, 0.0f);

	this->calculateDirection();
}

void Camera::applyTransformation(void)
{
	this->focalPoint.copy(&this->position);
	this->focalPoint.add(&this->direction);

	//std::cout << "Poision: " << this->position << std::endl;
	//std::cout << "Focal Point: " << this->focalPoint << std::endl << std::endl;

	gluLookAt(this->position.x  , this->position.y  , this->position.z,
			  this->focalPoint.x, this->focalPoint.y, this->focalPoint.z,
			  this->upVector.x  , this->upVector.y  , this->upVector.z);
}

void Camera::calculateDirection(void)
{
	this->direction.x = sin(Math::Util::degreesToRadians(this->verticalAngle)) * cos(Math::Util::degreesToRadians(this->horizontalAngle));
	this->direction.y = cos(Math::Util::degreesToRadians(this->verticalAngle));
	this->direction.z = sin(Math::Util::degreesToRadians(this->verticalAngle)) * sin(Math::Util::degreesToRadians(this->horizontalAngle));
}

void Camera::moveBackward(bool running)
{
	if(running)
	{
		this->direction.scale(this->runSpeed);
		this->position.subtract(&this->direction);
		this->direction.scale(1.0f / this->runSpeed);
	}
	else
	{
		this->direction.scale(this->walkSpeed);
		this->position.subtract(&this->direction);
		this->direction.scale(1.0f / this->walkSpeed);
	}
}

void Camera::moveDown(bool running)
{
	if(running)
	{
		this->upVector.scale(this->runSpeed);
		this->position.subtract(&this->upVector);
		this->upVector.scale(1.0f / this->runSpeed);
	}
	else
	{
		this->upVector.scale(this->walkSpeed);
		this->position.subtract(&this->direction);
		this->upVector.scale(1.0f / this->walkSpeed);
	}
}

void Camera::moveForward(bool running)
{
	if(running)
	{
		this->direction.scale(this->runSpeed);
		this->position.add(&this->direction);
		this->direction.scale(1.0f / this->runSpeed);
	}
	else
	{
		this->direction.scale(this->walkSpeed);
		this->position.add(&this->direction);
		this->direction.scale(1.0f / this->walkSpeed);
	}
}

void Camera::moveLeft(bool running)
{
	Math::Vector3f left = Math::Vector3f(this->upVector);
	
	left.crossProduct(&this->direction);
	left.normalize();

	if(running)
	{
		left.scale(this->runSpeed);
	}
	else
	{
		left.scale(this->walkSpeed);
	}

	this->position.add(&left);
}

void Camera::moveRight(bool running)
{
	Math::Vector3f right = Math::Vector3f(this->direction);
	
	right.crossProduct(&this->upVector);
	right.normalize();

	if(running)
	{
		right.scale(this->runSpeed);
	}
	else
	{
		right.scale(this->walkSpeed);
	}

	this->position.add(&right);
}

void Camera::moveUp(bool running)
{
	if(running)
	{
		this->upVector.scale(this->runSpeed);
		this->position.add(&this->upVector);
		this->upVector.scale(1.0f / this->runSpeed);
	}
	else
	{
		this->upVector.scale(this->walkSpeed);
		this->position.add(&this->direction);
		this->upVector.scale(1.0f / this->walkSpeed);
	}
}

void Camera::panHorizontally(float magnitude)
{
	this->horizontalAngle -= magnitude * this->panSpeed;
	this->horizontalAngle = fmod(this->horizontalAngle, 360.0f);
	this->calculateDirection();
}

void Camera::panVertically(float magnitude)
{
	this->verticalAngle -= magnitude * this->panSpeed;
	this->verticalAngle = fmod(this->verticalAngle, 360.0f);
	this->calculateDirection();
}

void Camera::setSpeeds(float walkSpeed, float runSpeed, float panSpeed)
{
	this->walkSpeed = walkSpeed;
	this->runSpeed = runSpeed;
	this->panSpeed = panSpeed;
}