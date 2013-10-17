#include <chrono>
#include <iostream>
#include <random>
#include <vector>


#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>


#include "core/Camera.hpp"
#include "core/GLCore.hpp"
#include "core/World.hpp"
#include "math/Vector3f.hpp"
#include "terrain/Generator.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"
#include "util/Log.hpp"

//The namespace this code belongs to:
using namespace Core;

static int width;
static int height;

static int windowID;

static std::chrono::high_resolution_clock::time_point lastClock;
static std::chrono::high_resolution_clock::time_point currentClock;

static Camera* camera;

static int oldMouseX = 0;
static int oldMouseY = 0;

static World* world;

static bool wired;

static char backwardKey;
static char downKey;
static char forwardKey;
static char leftKey;
static char rightKey;
static char upKey;
static char wiredKey;




void GLCore::init(int argc, char** argv)
{	
	Util::Config::init();
	//Util::Config::printSections();
	Util::Log::init(false, 0);

	//Stuff Initialized for the update loop.
	lastClock = std::chrono::high_resolution_clock::now();

	camera = new Camera(0.0f, 0.0f, 6.0f);

	backwardKey = Util::Config::convertSettingToChar("controls", "backward");
	downKey = Util::Config::convertSettingToChar("controls", "down");
	forwardKey = Util::Config::convertSettingToChar("controls", "forward");
	leftKey = Util::Config::convertSettingToChar("controls", "left");
	rightKey = Util::Config::convertSettingToChar("controls", "right");
	upKey = Util::Config::convertSettingToChar("controls", "up");
	wiredKey = Util::Config::convertSettingToChar("controls", "wired");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//GL Initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	//Set the window size and position:
	width = Util::Config::convertSettingToInt("window", "width");
    height = Util::Config::convertSettingToInt("window", "height");
    glutInitWindowSize(width, height);
	glutInitWindowPosition(Util::Config::convertSettingToInt("window", "init_x"), Util::Config::convertSettingToInt("window", "init_y"));

	windowID = glutCreateWindow("Solar Shard");
	glewInit();

	//std::cout << GLEW_OK << " and " << error << std::endl;

	//Register Window callback functions:
	glutDisplayFunc(GLCore::runLoop);
	glutKeyboardFunc(GLCore::keyboard);
	glutMouseFunc(GLCore::mouseClick);
	glutMotionFunc(GLCore::mouseActiveMotion);
	glutPassiveMotionFunc(GLCore::mousePassiveMotion);
	glutSpecialFunc(GLCore::functionKeys);

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

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	wired = false;

	world = new World();
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float ambientColour[] = {0.21f, 0.21f, 0.21f, 1.0f};
	float lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};
	float specularColour[] = {0.2f, 0.2f, 0.2f, 1.0f};
	float diffuse[] = {0.33f, 0.33f, 0.33f, 1.0f};
	float shininess = 0.5f;

	float whiteLight[] = {0.5f, 0.5f, 0.5f, 1.0f};

	//Apply Camera Transformation
	camera->applyTransformation();

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColour);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColour);

	world->draw(&camera->position, wired);

	glutSwapBuffers();
}

void GLCore::functionKeys(int keyCode, int positionX, int positionY)
{

}

void GLCore::keyboard(unsigned char keyCode, int positionX, int positionY)
{
	keyCode = tolower(keyCode);
	int modifierKeys = glutGetModifiers();

	if(keyCode == 27)//KeyCode for escape.
	{
		uninit(0);
	}
	else if(keyCode == backwardKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveBackward(true);
		}
		else
		{
			camera->moveBackward(false);
		}
	}
	else if(keyCode == downKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveDown(true);
		}
		else
		{
			camera->moveDown(false);
		}
	}
	else if(keyCode == forwardKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveForward(true);
		}
		else
		{
			camera->moveForward(false);
		}
	}
	else if(keyCode == leftKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveLeft(true);
		}
		else
		{
			camera->moveLeft(false);
		}
	}
	else if(keyCode == rightKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveRight(true);
		}
		else
		{
			camera->moveRight(false);
		}
	}
	else if(keyCode == upKey)
	{
		if(modifierKeys == GLUT_ACTIVE_SHIFT)
		{
			camera->moveUp(true);
		}
		else
		{
			camera->moveUp(false);
		}
	}
	else if(keyCode == wiredKey)
	{
		wired = !wired;
	}
}

void GLCore::mouseClick(int buttonCode, int buttonState, int positionX, int positionY)
{
}

void GLCore::mouseActiveMotion(int positionX, int positionY)
{
	camera->panHorizontally(oldMouseX - positionX);
	camera->panVertically(oldMouseY - positionY);

	oldMouseX = positionX;
	oldMouseY = positionY;
}

void GLCore::mousePassiveMotion(int positionX, int positionY)
{
	oldMouseX = positionX;
	oldMouseY = positionY;
}

void GLCore::runLoop(void)
{
	currentClock = std::chrono::high_resolution_clock::now();
	update(std::chrono::duration_cast<std::chrono::duration<float>>(currentClock - lastClock).count());
	lastClock = currentClock;

	draw();

	glutPostRedisplay();
}

void GLCore::uninit(int returnCode)
{
	Util::Log::uninit();
	Util::Config::uninit();

	exit(returnCode);
}

void GLCore::update(float deltaTime)
{
}