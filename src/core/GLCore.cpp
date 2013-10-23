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

static float nearClip;
static float farClip;

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

//Lighting Stuff:
static float* ambientLighting;

static float* lightDiffuse;
static float* lightSpecular;

static float* materialDiffuse;
static float* materialSpecular;
static float* materialShininess;


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

	nearClip = Util::Config::convertSettingToFloat("camera", "near_clip");
	farClip = Util::Config::convertSettingToFloat("camera", "far_clip");

	gluPerspective(60, 1.0f, nearClip, farClip);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);

	//Speaking of lights let's load the light and material colours here:
	ambientLighting = new float[4];
	ambientLighting[0] = Util::Config::convertSettingToFloat("lighting", "ambient_red");
	ambientLighting[1] = Util::Config::convertSettingToFloat("lighting", "ambient_green");
	ambientLighting[2] = Util::Config::convertSettingToFloat("lighting", "ambient_blue");
	ambientLighting[3] = Util::Config::convertSettingToFloat("lighting", "ambient_alpha");

	lightDiffuse = new float[4];
	lightDiffuse[0] = Util::Config::convertSettingToFloat("lighting", "light_diffuse_red");
	lightDiffuse[1] = Util::Config::convertSettingToFloat("lighting", "light_diffuse_green");
	lightDiffuse[2] = Util::Config::convertSettingToFloat("lighting", "light_diffuse_blue");
	lightDiffuse[3] = Util::Config::convertSettingToFloat("lighting", "light_diffuse_alpha");

	lightSpecular = new float[4];
	lightSpecular[0] = Util::Config::convertSettingToFloat("lighting", "light_specular_red");
	lightSpecular[1] = Util::Config::convertSettingToFloat("lighting", "light_specular_green");
	lightSpecular[2] = Util::Config::convertSettingToFloat("lighting", "light_specular_blue");
	lightSpecular[3] = Util::Config::convertSettingToFloat("lighting", "light_specular_alpha");

	materialDiffuse = new float[4];
	materialDiffuse[0] = Util::Config::convertSettingToFloat("lighting", "mat_diffuse_red");
	materialDiffuse[1] = Util::Config::convertSettingToFloat("lighting", "mat_diffuse_green");
	materialDiffuse[2] = Util::Config::convertSettingToFloat("lighting", "mat_diffuse_blue");
	materialDiffuse[3] = Util::Config::convertSettingToFloat("lighting", "mat_diffuse_alpha");

	materialSpecular = new float[4];
	materialSpecular[0] = Util::Config::convertSettingToFloat("lighting", "mat_specular_red");
	materialSpecular[1] = Util::Config::convertSettingToFloat("lighting", "mat_specular_green");
	materialSpecular[2] = Util::Config::convertSettingToFloat("lighting", "mat_specular_blue");
	materialSpecular[3] = Util::Config::convertSettingToFloat("lighting", "mat_specular_alpha");

	materialShininess = new float;
	*materialShininess = Util::Config::convertSettingToFloat("lighting", "mat_shininess");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	wired = false;

	world = new World();
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};

	//Apply Camera Transformation
	camera->applyTransformation();

	//Lighting Stuff:
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, materialDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, materialSpecular);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLighting);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	world->maintainTiles(&camera->position);
	world->draw(camera, wired);

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