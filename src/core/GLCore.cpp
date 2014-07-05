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
#include "resources/Texture.hpp"
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

static bool wiredTiles;
static bool drawBoundaries;

static char backwardKey;
static char downKey;
static char forwardKey;
static char leftKey;
static char rightKey;
static char upKey;
static char wiredTileKey;
static char drawBoundaryKey;

//Lighting Stuff:
static float* ambientLighting;

static float* lightDiffuse;
static float* lightSpecular;

static float* materialDiffuse;
static float* materialSpecular;
static float* materialShininess;

static Resources::Texture* skyBack;
static Resources::Texture* skyFront;
static Resources::Texture* skyRight;
static Resources::Texture* skyLeft;
static Resources::Texture* skyTop;

static float skyBump;

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
	wiredTileKey = Util::Config::convertSettingToChar("controls", "wired");
	drawBoundaryKey = Util::Config::convertSettingToChar("controls", "regions");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//GL Initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	//Set the window size and position:
	width = Util::Config::convertSettingToInt("window", "width");
    height = Util::Config::convertSettingToInt("window", "height");
    glutInitWindowSize(width, height);
	glutInitWindowPosition(Util::Config::convertSettingToInt("window", "init_x"), Util::Config::convertSettingToInt("window", "init_y"));

	windowID = glutCreateWindow("pcg");
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
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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

	wiredTiles = false;
	drawBoundaries = false;

	world = new World();

    skyBump = Util::Config::convertSettingToFloat("sky_box", "bump");

	skyBack = new Resources::Texture(Util::Config::convertSettingToString("sky_box", "back"));
	skyFront = new Resources::Texture(Util::Config::convertSettingToString("sky_box", "front"));
	skyLeft = new Resources::Texture(Util::Config::convertSettingToString("sky_box", "left"));
	skyRight = new Resources::Texture(Util::Config::convertSettingToString("sky_box", "right"));
	skyTop = new Resources::Texture(Util::Config::convertSettingToString("sky_box", "top"));
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f};

	//Apply Camera Transformation
	camera->applyTransformation();

	//Lighting Stuff:
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  materialDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, materialSpecular);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLighting);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	drawSkyBox();

	glDisable(GL_TEXTURE_2D);
	world->maintainRegions(&camera->position);
	world->maintainTiles(&camera->position);
	world->draw(camera, wiredTiles, drawBoundaries);
	glEnable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

void GLCore::drawSkyBox()
{
	float fronBotLeft[3];
	float fronBotRigh[3];
	float fronTopLeft[3];
	float fronTopRigh[3];

	float backBotLeft[3];
	float backBotRigh[3];
	float backTopLeft[3];
	float backTopRigh[3];
		
	//Front 4 points:
	fronBotLeft[0] = camera->position.x - 2.0f;
	fronBotLeft[1] = camera->position.y - 2.0f;
	fronBotLeft[2] = camera->position.z - 2.0f;

	fronBotRigh[0] = camera->position.x + 2.0f;
	fronBotRigh[1] = camera->position.y - 2.0f;
	fronBotRigh[2] = camera->position.z - 2.0f;

	fronTopLeft[0] = camera->position.x - 2.0f;
	fronTopLeft[1] = camera->position.y + 2.0f;
	fronTopLeft[2] = camera->position.z - 2.0f;

	fronTopRigh[0] = camera->position.x + 2.0f;
	fronTopRigh[1] = camera->position.y + 2.0f;
	fronTopRigh[2] = camera->position.z - 2.0f;

	//Back 4 Points:
	backBotLeft[0] = camera->position.x - 2.0f;
	backBotLeft[1] = camera->position.y - 2.0f;
	backBotLeft[2] = camera->position.z + 2.0f;

	backBotRigh[0] = camera->position.x + 2.0f;
	backBotRigh[1] = camera->position.y - 2.0f;
	backBotRigh[2] = camera->position.z + 2.0f;

	backTopLeft[0] = camera->position.x - 2.0f;
	backTopLeft[1] = camera->position.y + 2.0f;
	backTopLeft[2] = camera->position.z + 2.0f;

	backTopRigh[0] = camera->position.x + 2.0f;
	backTopRigh[1] = camera->position.y + 2.0f;
	backTopRigh[2] = camera->position.z + 2.0f;

	glBindTexture(GL_TEXTURE_2D, skyFront->id);
	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		//Front:
		glTexCoord2d(0.0f, 0.0f); glVertex3f(fronBotLeft[0] - skyBump, fronBotLeft[1] - skyBump, fronBotLeft[2]);
		glTexCoord2d(1.0f, 0.0f); glVertex3f(fronBotRigh[0] + skyBump, fronBotRigh[1] - skyBump, fronBotRigh[2]);
		glTexCoord2d(1.0f, 1.0f); glVertex3f(fronTopRigh[0] + skyBump, fronTopRigh[1] + skyBump, fronTopRigh[2]);
		glTexCoord2d(0.0f, 1.0f); glVertex3f(fronTopLeft[0] - skyBump, fronTopLeft[1] + skyBump, fronTopLeft[2]);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, skyRight->id);
	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		//Right
		glTexCoord2d(0.0f, 0.0f); glVertex3f(fronTopRigh[0], fronTopRigh[1] + skyBump, fronTopRigh[2] - skyBump);
		glTexCoord2d(1.0f, 0.0f); glVertex3f(backTopRigh[0], backTopRigh[1] + skyBump, backTopRigh[2] + skyBump);
		glTexCoord2d(1.0f, 1.0f); glVertex3f(backBotRigh[0], backBotRigh[1] - skyBump, backBotRigh[2] + skyBump);
		glTexCoord2d(0.0f, 1.0f); glVertex3f(fronBotRigh[0], fronBotRigh[1] - skyBump, fronBotRigh[2] - skyBump);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, skyLeft->id);
	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		//Left
		glTexCoord2d(0.0f, 0.0f); glVertex3f(backTopLeft[0], backTopLeft[1] + skyBump, backTopLeft[2] + skyBump);
		glTexCoord2d(1.0f, 0.0f); glVertex3f(fronTopLeft[0], fronTopLeft[1] + skyBump, fronTopLeft[2] - skyBump);
		glTexCoord2d(1.0f, 1.0f); glVertex3f(fronBotLeft[0], fronBotLeft[1] - skyBump, fronBotLeft[2] - skyBump);
		glTexCoord2d(0.0f, 1.0f); glVertex3f(backBotLeft[0], backBotLeft[1] - skyBump, backBotLeft[2] + skyBump);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, skyBack->id);
	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		//Back
		glTexCoord2d(0.0f, 0.0f); glVertex3f(backTopRigh[0] + skyBump, backTopRigh[1] + skyBump, backTopRigh[2]);
		glTexCoord2d(1.0f, 0.0f); glVertex3f(backTopLeft[0] - skyBump, backTopLeft[1] + skyBump, backTopLeft[2]);
		glTexCoord2d(1.0f, 1.0f); glVertex3f(backBotLeft[0] - skyBump, backBotLeft[1] - skyBump, backBotLeft[2]);
		glTexCoord2d(0.0f, 1.0f); glVertex3f(backBotRigh[0] + skyBump, backBotRigh[1] - skyBump, backBotRigh[2]);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, skyTop->id);
	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		//Top:
		glTexCoord2d(0.0f, 0.0f); glVertex3f(backTopLeft[0] - skyBump, backTopLeft[1], backTopLeft[2] + skyBump);
		glTexCoord2d(1.0f, 0.0f); glVertex3f(backTopRigh[0] + skyBump, backTopRigh[1], backTopRigh[2] + skyBump);
		glTexCoord2d(1.0f, 1.0f); glVertex3f(fronTopRigh[0] + skyBump, fronTopRigh[1], fronTopRigh[2] - skyBump);
		glTexCoord2d(0.0f, 1.0f); glVertex3f(fronTopLeft[0] - skyBump, fronTopLeft[1], fronTopLeft[2] - skyBump);

	glEnd();

	glClear(GL_DEPTH_BUFFER_BIT);
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
	else if(keyCode == wiredTileKey)
	{
		wiredTiles = !wiredTiles;
	}
	else if(keyCode == drawBoundaryKey)
	{
		drawBoundaries = !drawBoundaries;
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
