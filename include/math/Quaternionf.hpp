#ifndef _MATH_QUATERNIONF_HPP
#define _MATH_QUATERNIONF_HPP

namespace Math
{
	class Quaternionf
	{

	//Variables:
	public:

		float x;
		float y;
		float z;
		float w;

	protected:
	private:

	//Functions:
	public:

		Quaternionf(void);
		Quaternionf(Quaternionf* that);
		~Quaternionf(void);

		void conjugate(void);

		float length(void);

		void multiply(Quaternionf* that);

		void normalize(void);


	protected:
	private:

	};
}

#endif