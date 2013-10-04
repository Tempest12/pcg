#include <iostream>
#include <string>

#include <GL/glut.h>

#include "core/GLCore.hpp"
#include "main/Main.hpp"

int main(int argc, char** argv)
{
	int result = 0;

	Core::GLCore::init(argc, argv);

	glutMainLoop();

	return result;	
}

void Main::die(const std::string& errorMessage)
{
	std::cout << errorMessage << std::endl;
	std::cout << "Exiting" << std::endl;

	exit(1);
}