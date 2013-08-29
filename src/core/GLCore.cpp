#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

#include "core/Camera.hpp"
#include "core/GLCore.hpp"

#include "math/Vector3f.hpp"

#include "util/Config.hpp"

//The namespace this code belongs to:
using namespace Core;

static int width;
static int height;

static int windowID;

static std::chrono::high_resolution_clock::time_point lastClock;
static std::chrono::high_resolution_clock::time_point currentClock;

static Camera camera;

static int mouseX = 0;
static int mouseY = 0;

static int oldMouseX = 0;
static int oldMouseY = 0;

static bool leftClickDown = false;
static bool cRandom = true;

static Math::Vector3f angle;

static std::vector<Math::Vector3f*> cPoints;
static std::vector<Math::Vector3f*> cppPoints;

static std::minstd_rand irand;
static std::uniform_int_distribution<unsigned int> dist;

void GLCore::init(int argc, char** argv)
{	
	Util::Config::init();

	//Stuff Initialized for the update loop.
	lastClock = std::chrono::high_resolution_clock::now();

	camera = Camera(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, -1.0f);
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
	glutKeyboardFunc(GLCore::keyboard);
	glutMouseFunc(GLCore::mouseClick);
	glutMotionFunc(GLCore::mouseMove);
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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	angle = Math::Vector3f();
	cPoints = std::vector<Math::Vector3f*>();
	cppPoints = std::vector<Math::Vector3f*>();

	srand(17);

	irand.seed(37);
}

void GLCore::draw(void)
{
	Math::Vector3f* vector;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//Apply Camera Transformation
	camera.ApplyTransformation();

	glRotatef(angle.x, 1.0f, 0.0f, 0.0f);
	glRotatef(angle.y, 0.0f, 1.0f, 0.0f);


	//C Points
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);

		for(unsigned int index = 0; index < cPoints.size(); index++)
		{
			vector = cPoints.at(index);
			glVertex3f(vector->x, vector->y, vector->z);
		}

	glEnd();

	//C++ Points
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);

		for(unsigned int index = 0; index < cppPoints.size(); index++)
		{
			vector = cppPoints.at(index);
			glVertex3f(vector->x, vector->y, vector->z);
		}

	glEnd();


	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutWireCube(2.0f);

	glutSwapBuffers();
}

void GLCore::functionKeys(int keyCode, int positionX, int positionY)
{

}

void GLCore::keyboard(unsigned char keyCode, int positionX, int positionY)
{
	if(keyCode == ' ')
	{
		Math::Vector3f* newPoint;

		if(cRandom == true)
		{
			newPoint = new Math::Vector3f((float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX);

			if(rand() % 2 == 1)
			{
				newPoint->x *= -1;
			}
			if(rand() % 2 == 1)
			{
				newPoint->y *= -1;
			}
			if(rand() % 2 == 1)
			{
				newPoint->z *= -1;
			}

			cPoints.push_back(new Math::Vector3f(newPoint));
		}
		else
		{
			//newPoint = new Math::Vector3f((float)irand()/(float)irand.max(), (float)irand()/(float)irand.max(), (float)irand()/(float)irand.max()); 
			newPoint = new Math::Vector3f((float)dist(irand)/(float)dist.max(), (float)dist(irand)/(float)dist.max(), (float)dist(irand)/(float)dist.max());

			if(rand() % 2 == 1)
			{
				newPoint->x *= -1;
			}
			if(rand() % 2 == 1)
			{
				newPoint->y *= -1;
			}
			if(rand() % 2 == 1)
			{
				newPoint->z *= -1;
			}

			cppPoints.push_back(new Math::Vector3f(newPoint));
		}
	}
	else if(keyCode == 'c')
	{
		Math::Vector3f* helper;

		for(unsigned int index = 0; index < cPoints.size(); index++)
		{
			helper = cPoints[index];
			delete helper;
		}
		for(unsigned int index = 0; index < cppPoints.size(); index++)
		{
			helper = cppPoints[index];
			delete helper;
		}

		cPoints.clear();
		cppPoints.clear();
	}
	else if(keyCode == 'r')
	{
		cRandom = !cRandom;
	}
	else if(keyCode == 'a')
	{
		angle.x = 0;
		angle.y = 0;
	}
}

void GLCore::mouseClick(int buttonCode, int buttonState, int positionX, int positionY)
{
	if(buttonCode == GLUT_LEFT_BUTTON)
	{
		if(buttonState == GLUT_DOWN)
		{
			leftClickDown = true;

			oldMouseX = positionX;
			oldMouseY = positionY;
		}
		else
		{
			leftClickDown = false;
		}
	}
}

void GLCore::mouseMove(int positionX, int positionY)
{
	if(leftClickDown)
	{
		angle.x += 0.65f * (positionY - oldMouseY); 
		angle.y += 0.65f * (positionX - oldMouseX);

		angle.x = fmod(angle.x, 360.0f);
		angle.y = fmod(angle.y, 360.0f);
	}
	else
	{
		return;
	}

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

void GLCore::uninit(void)
{

}

void GLCore::update(float deltaTime)
{
}