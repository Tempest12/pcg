#ifndef _MATH_UTIL_HPP
#define _MATH_UTIL_HPP

#define _USE_MATH_DEFINES
#include <math.h>

namespace Math
{
	class Util
	{
	//Variables:
	public:
	protected:
	private:

	//Functions:
	public:
	protected:
	private:

	//Static Functions:
	public:
		
		double static inline degreesToRadians(double degrees)
		{
			return (degrees * M_PI) / 180.0;
		}
		float static inline degreesToRadians(float degrees)
		{
			return (degrees * (float)M_PI) / 180.0f;
		}


		double static inline radiansToDegrees(double radians)
		{
			return (radians * 180.0) / M_PI;
		}

		float static inline radiansToDegrees(float radians)
		{
			return (radians * 180.f) / (float)M_PI;
		}

	protected:
	private:

	};
}

#endif
