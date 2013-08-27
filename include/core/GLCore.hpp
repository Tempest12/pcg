#ifndef _CORE_HPP
#define _CORE_HPP

#include <chrono>

namespace Core
{
	class Camera;

	class GLCore
	{
	//Variables:
	public:
	protected:
	private:

	//Functions:
	public:
		static void init(int argc, char** argv);
		static void draw(void);
		static void runLoop(void);
		static void uninit(void);
		static void update(float time);

	protected:
	private:
	};
}
#endif