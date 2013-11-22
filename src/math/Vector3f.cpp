//Author: Shane del Solar
//Version: 2.00.ss

#include <math.h>

#include "math/Vector3f.hpp"

//This namespace this code belongs to:
using namespace Math;


//Constructors
Vector3f::Vector3f(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3f::Vector3f(float all)
{
	this->x = all;
	this->y = all;
	this->z = all;
}

Vector3f::Vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f::Vector3f(const Vector3f* that)
{
	this->x = that->x;
	this->y = that->y;
	this->z = that->z;
}

Vector3f::Vector3f(const Vector3f& that)
{
	this->x = that.x;
	this->y = that.y;
	this->z = that.z;
}

Vector3f::~Vector3f(void)
{
	return;
}


//Methods:
void Vector3f::add(Vector3f* that)
{
    this->x += that->x;
    this->y += that->y;
    this->z += that->z;
}

void Vector3f::add(Vector3f& that)
{
	this->x += that.x;
	this->y += that.y;
	this->z += that.z;
}

void Vector3f::copy(Vector3f* that)
{
	this->x = that->x;
	this->y = that->y;
	this->z = that->z;
}

void Vector3f::crossProduct(Vector3f* that)
{
	float tempX = this->y * that->z - this->z * that->y;
    float tempY = this->z * that->x - this->x * that->z;
    float tempZ = this->x * that->y - this->y * that->x;
    x = tempX;
    y = tempY;
    z = tempZ;
}

void Vector3f::divideByScalar(float scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
}

float Vector3f::distance(float x, float y, float z)
{
	return sqrt(this->distanceSquared(x, y, z));
}

float Vector3f::distance(Vector3f* that)
{
	return sqrt(this->distanceSquared(that));
}

float Vector3f::distanceSquared(float x, float y, float z)
{
	float diffX = this->x - x;
	float diffY = this->y - y;
	float diffZ = this->z - z;

	return (diffX * diffX) + (diffY * diffY) + (diffZ * diffZ);
}

float Vector3f::distanceSquared(Vector3f* that)
{
	float diffX = this->x - that->x;
	float diffY = this->y - that->y;
	float diffZ = this->z - that->z;

	return (diffX * diffX) + (diffY * diffY) + (diffZ * diffZ);
}

float Vector3f::dotProduct(Vector3f* that)
{
	return (this->x * that->x) + (this->y * that->y) + (this->z * that->z);
}

float Vector3f::dotProductSquared(Vector3f* that)
{
	float num = (this->x * that->x) + (this->y * that->y) + (this->z * that->z);
	return num * num;
}

bool Vector3f::equals(Vector3f* that)
{
	return ((this->x == that->x) && (this->y == that->y) && (this->z == that->z));
}

float Vector3f::magnitude(void)
{
	return sqrt((x * x) + (y * y) + (z * z));
}


float Vector3f::magnitudeSquared(void)
{
	return (x * x) + (y * y) + (z * z);
}

void Vector3f::normalize(void)
{
	float length = this->magnitude();
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

void Vector3f::scale(float scalar)
{
        x *= scalar;
        y *= scalar;
        z *= scalar;
}  

void Vector3f::subtract(Vector3f* that)
{        
        this->x -= that->x;
        this->y -= that->y;
        this->z -= that->z;
}

void Vector3f::zeroOut(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

//Operators:
std::ostream& operator<<(std::ostream& stream, const Math::Vector3f& vect)
{
	return stream << "X: " << vect.x << " Y: " << vect.y << " Z: " << vect.z << std::endl;
}