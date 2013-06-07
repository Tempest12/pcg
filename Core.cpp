#include <iostream>

#include <GL/glut.h>
#include <time.h>

#include "Core.hpp"

void Core::init(int argc, char** argv)
{
	//Stuff Initialized for the update loop.
	lastClock = std::chrono::high_resolution_clock::now();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//GL Initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);



	windowID = glutCreateWindow("Solar Shard");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Core::draw(void)
{

}

void Core::runLoop(void)
{
	currentClock = std::chrono::high_resolution_clock::now();
	update(std::chrono::duration_cast<std::chrono::duration<float>>(currentClock - lastClock).count());
	lastClock = currentClock;

}

void Core::uninit(void)
{

}

void Core::update(float deltaTime)
{
}