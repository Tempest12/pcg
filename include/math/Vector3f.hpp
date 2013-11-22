#ifndef _MATH_VECTOR3F_HPP
#define _MATH_VECTOR3F_HPP

#include <iostream>

#include <math.h>

namespace Math
{
	class Vector3f
	{
	//Variables
	public:
		float x;
		float y;
		float z;

	protected:
	private:


	//Functions:
	public:
		//Constructors
		Vector3f(void);
		Vector3f(float all);
		Vector3f(float x, float y, float z);
		Vector3f(const Vector3f* that);
		Vector3f(const Vector3f& that);
		~Vector3f(void);

		//Methods:
		void add(Vector3f* that);
		void add(Vector3f &that);

		void crossProduct(Vector3f* that);		
		
		void copy(Vector3f* that);
		
		float distance(float x, float y, float z);
		float distance(Vector3f* that);
		float distanceSquared(float x, float y, float z);
		float distanceSquared(Vector3f* that);

		float dotProduct(Vector3f* that);
		float dotProductSquared(Vector3f* that);

		void divideByScalar(float scalar);

		bool equals(Vector3f* that);

		float magnitude(void);
		float magnitudeSquared(void);

		void normalize(void);

		void scale(float scalar);

		void subtract(Vector3f* that);

		void zeroOut(void);

		//Additional operators:
		bool operator==(Vector3f* that)
		{
			return ((this -> x == that -> x) && (this -> y == that -> y) && (this -> z == that -> z));
		} 
		
	    /*std::ostream&  operator<<(std::ostream& stream)
		{
			stream << "X: " << this->x << " Y: " << this->y << " Z: " << this->z << std::endl;

			return stream;
		}*/

		//Static Functions:
	protected:
	private:
	};
}

std::ostream&  operator<<(std::ostream& stream, const Math::Vector3f& vect);

#endif