#ifndef _CORE_HPP
#define _CORE_HPP

#include <chrono>

namespace Core
{
	//Variables:
	static std::chrono::high_resolution_clock::time_point lastClock;
	static std::chrono::high_resolution_clock::time_point currentClock;

	static int windowID;


	//Functions:

	void init(void);

	void draw(void);

	void runLoop(void);

	void uninit(void);

	void update(float time);
}

#endif