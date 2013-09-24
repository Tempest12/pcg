#include <chrono>
#include <iostream>
#include <random>
#include <vector>


#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>


#include "core/Camera.hpp"
#include "core/GLCore.hpp"
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

static Camera camera;

static int oldMouseX = 0;
static int oldMouseY = 0;

static Terrain::Tile* tile;
static Terrain::Tile* OGTile;
static Terrain::Tile* second;

static char backwardKey;
static char downKey;
static char forwardKey;
static char leftKey;
static char rightKey;
static char upKey;


void GLCore::init(int argc, char** argv)
{	
	Util::Config::init();
	Util::Log::init(false, 0);

	//Stuff Initialized for the update loop.
	lastClock = std::chrono::high_resolution_clock::now();

	camera = Camera(0.0f, 0.0f, 6.0f);
	camera.setSpeeds(Util::Config::convertSettingToFloat("camera", "walk_speed"),
					 Util::Config::convertSettingToFloat("camera", "run_speed"),
					 Util::Config::convertSettingToFloat("camera", "pan_speed"));

	backwardKey = Util::Config::convertSettingToChar("controls", "backward");
	downKey = Util::Config::convertSettingToChar("controls", "down");
	forwardKey = Util::Config::convertSettingToChar("controls", "forward");
	leftKey = Util::Config::convertSettingToChar("controls", "left");
	rightKey = Util::Config::convertSettingToChar("controls", "right");
	upKey = Util::Config::convertSettingToChar("controls", "up");
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//GL Initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	//Set the window size and position:
	glutInitWindowSize(Util::Config::convertSettingToInt("window", "width"), Util::Config::convertSettingToInt("window", "height"));
	glutInitWindowPosition(Util::Config::convertSettingToInt("window", "init_x"), Util::Config::convertSettingToInt("window", "init_y"));

	windowID = glutCreateWindow("Solar Shard");
	glewInit();


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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	Terrain::Tile::kValue = Util::Config::convertSettingToFloat("generator", "height_scale");
	Terrain::Tile::hValue = Util::Config::convertSettingToFloat("generator", "h_value");
	Terrain::Tile::randomBump = Util::Config::convertSettingToFloat("generator", "random_bump");


	Terrain::Generator* tileGen = new Terrain::Generator(Util::Config::convertSettingToFloat("generator", "tile_size"));
	tile = tileGen->newTile(0, 0, 0);
	OGTile = tileGen->newTile(0, -5, 0);
	second = tileGen->newTile(0, 1, 0);
	
	for(int index = 0; index < 7; index++)
	{
		tile->subDivide(0.0);
		second->subDivide(0.0);
	}
	delete tileGen;
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//Apply Camera Transformation
	camera.applyTransformation();

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	/*glBegin(GL_QUADS);
	{
		glVertex3f(0, -1, 0);
		glVertex3f(5, -1, 0);
		glVertex3f(5, -1, -5);
		glVertex3f(0, -1, -5);

		glVertex3f(5, -1, 0);
		glVertex3f(10, -1, 0);
		glVertex3f(10, -1, -5);
		glVertex3f(5, -1, -5);

		glVertex3f(0, -1, -5);
		glVertex3f(5, -1, -5);
		glVertex3f(5, -1, -10);
		glVertex3f(0, -1, -10);

		glVertex3f(5, -1, -5);
		glVertex3f(10, -1, -5);
		glVertex3f(10, -1, -10);
		glVertex3f(5, -1, -10);
	}
	glEnd();*/

	if(tile != NULL && OGTile != NULL)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		tile->draw(false);
		second->draw(false);
		OGTile->draw(false);
	}

	glutSwapBuffers();
}

void GLCore::functionKeys(int keyCode, int positionX, int positionY)
{

}

void GLCore::keyboard(unsigned char keyCode, int positionX, int positionY)
{
	if(keyCode == backwardKey)
	{
		camera.moveBackward(false);
	}
	else if(keyCode == downKey)
	{
		camera.moveDown(false);
	}
	else if(keyCode == forwardKey)
	{
		camera.moveForward(false);
	}
	else if(keyCode == leftKey)
	{
		camera.moveLeft(false);
	}
	else if(keyCode == rightKey)
	{
		camera.moveRight(false);
	}
	else if(keyCode == upKey)
	{
		camera.moveUp(false);
	}
}

void GLCore::mouseClick(int buttonCode, int buttonState, int positionX, int positionY)
{
}

void GLCore::mouseActiveMotion(int positionX, int positionY)
{
	camera.panHorizontally(oldMouseX - positionX);
	camera.panVertically(oldMouseY - positionY);

	oldMouseX = positionX;
	oldMouseY = positionY;
}

void GLCore::mousePassiveMotion(int positionX, int positionY)
{
	/*camera.panHorizontally(oldMouseX - positionX);
	camera.panVertically(oldMouseY - positionY);*/

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