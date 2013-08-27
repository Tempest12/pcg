#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "math/Vector3f.hpp"

namespace Core
{
	class Camera
	{
	//Variables:
	public:
		Math::Vector3f position;

		Math::Vector3f focalPoint;

		Math::Vector3f upVector;

	protected:
	private:

	//Methods:
	public:

		Camera(void);
		Camera(float x, float y, float z, float focalX, float focalY, float focalZ);

		void ApplyTransformation(void);

	protected:
	private:
	};
}

#endif