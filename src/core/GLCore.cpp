#include <chrono>
#include <iostream>

#include <GL/glut.h>
#include <time.h>

#include "core/Camera.hpp"
#include "core/GLCore.hpp"

#include "util/Config.hpp"

//The namespace this code belongs to:
using namespace Core;

static int width;
static int height;

static int windowID;

static std::chrono::high_resolution_clock::time_point lastClock;
static std::chrono::high_resolution_clock::time_point currentClock;

static Camera camera;

void GLCore::init(int argc, char** argv)
{	
	Util::Config::init();

	//Stuff Initialized for the update loop.
	lastClock = std::chrono::high_resolution_clock::now();

	camera = Camera(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, -1.0f);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//GL Initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	//Set the window size and position:
	glutInitWindowSize(Util::Config::convertSettingToInt("window", "width"), Util::Config::convertSettingToInt("window", "height"));
	glutInitWindowPosition(Util::Config::convertSettingToInt("window", "init_x"), Util::Config::convertSettingToInt("window", "init_y"));

	windowID = glutCreateWindow("Solar Shard");

	//Register Window callback functions:
	glutDisplayFunc(GLCore::runLoop);


	//Set variables:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glViewport(0, 0, 300, 300);
	gluPerspective(60, 1.0f, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//Apply Camera Transformation
	//camera.ApplyTransformation();

	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 5.0f, -10.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(5.0f, 0.0f, -10.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(-5.0f, 0.0f, -10.0f);
	glEnd();

	glutSwapBuffers();
}

void GLCore::runLoop(void)
{
	currentClock = std::chrono::high_resolution_clock::now();
	update(std::chrono::duration_cast<std::chrono::duration<float>>(currentClock - lastClock).count());
	lastClock = currentClock;

	draw();

	glutPostRedisplay();
}

void GLCore::uninit(void)
{

}

void GLCore::update(float deltaTime)
{
}