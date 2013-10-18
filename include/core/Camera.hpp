#ifndef _CORE_CAMERA_HPP
#define _CORE_CAMERA_HPP

#include "math/Vector3f.hpp"

namespace Core
{
	class Camera
	{
	//Variables:
	public:

		float panSpeed;
		float runSpeed;
		float walkSpeed;

		float pitch;
		float yaw;

		Math::Vector3f direction;
		Math::Vector3f focalPoint;
		Math::Vector3f position;
		Math::Vector3f upVector;

	protected:
	private:

	//Methods:
	public:

		Camera(void);
		Camera(float x, float y, float z);

		void applyTransformation(void);

		void calculateDirection(void);

		void moveBackward(bool running);
		void moveDown(bool running);
		void moveForward(bool running);
		void moveLeft(bool running);
		void moveRight(bool running);
		void moveUp(bool running);

		void panHorizontally(float magnitude);
		void panVertically(float magnitude);

	protected:
	private:
	};
}

#endif