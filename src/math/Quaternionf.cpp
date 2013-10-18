#include <math.h>

#include "math/Quaternionf.hpp"

using namespace Math;

Quaternionf::Quaternionf(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}

Quaternionf::Quaternionf(Quaternionf* that)
{
	this->x = that->x;
	this->y = that->y;
	this->z = that->z;
	this->w = that->w;
}

void Quaternionf::conjugate(void)
{
	this->x *= -1.0f;
	this->y *= -1.0f;
	this->z *= -1.0f;
}

float Quaternionf::length(void)
{
	return sqrt((this->x * this->x) + (this->y * this->y) +
		        (this->z * this->z) + (this->w * this->w));
}

void Quaternionf::multiply(Quaternionf* that)
{
	float oldX = this->x;
	float oldY = this->y;
	float oldZ = this->z;
	float oldW = this->w;

	this->x = (oldW * that->x) + (oldX * that->w) + (oldY * that->z) - (oldZ * that->y);
	this->y = (oldW * that->y) - (oldX * that->z) + (oldY * that->w) + (oldZ * that->x);
	this->z = (oldW * that->z) + (oldX * that->y) - (oldY * that->x) + (oldZ * that->w);
	this->w = (oldW * that->w) - (oldX * that->x) - (oldY * that->y) - (oldZ * that->z);
}

void Quaternionf::normalize(void)
{
	float length = this->length();

	this->x /= length;
	this->y /= length;
	this->z /= length;
	this->w /= length;
}