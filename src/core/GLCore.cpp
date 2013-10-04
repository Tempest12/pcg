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

static Terrain::Tile** tiles;

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

	Terrain::Tile::kValue = Util::Config::convertSettingToFloat("generator", "height_scale");
	Terrain::Tile::hValue = Util::Config::convertSettingToFloat("generator", "h_value");
	Terrain::Tile::randomBump = Util::Config::convertSettingToFloat("generator", "random_bump");


	Terrain::Generator* tileGen = new Terrain::Generator(Util::Config::convertSettingToFloat("generator", "tile_size"));
	
	tiles = new Terrain::Tile*[9];

	int index = 0;
	for(int row = -1; row < 2; row++)
	{
		for(int col = -1; col < 2; col++)
		{
			tiles[index] = tileGen->newTile(col, row, Util::Config::convertSettingToInt("generator", "subdivisions"));
			tiles[index][0].prepareDraw();
			index++;
		}
	}
	
	delete tileGen;
}

void GLCore::draw(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float lightPosition[] = {0.0f, 10.0f, 0.0f, 0.0f};
	float specularColour[] = {0.2f, 0.2f, 0.2f, 1.0f};
	float shininess = 5.0f;

	//Apply Camera Transformation
	camera.applyTransformation();

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColour);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	for(int index = 0; index < 9; index++)
	{
		tiles[index][0].draw(wired);
	}

	/*glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-10.0f, 0.0f, -10.0f);
		glVertex3f(-10.0f, 0.0f, 30.0f);
		glVertex3f(10.0f, 0.0f, 30.0f);
		glVertex3f(10.0f, 0.0f, -10.0f);
	glEnd();*/


	glutSwapBuffers();
}

void GLCore::functionKeys(int keyCode, int positionX, int positionY)
{

}

void GLCore::keyboard(unsigned char keyCode, int positionX, int positionY)
{
	if(keyCode == 27)//KeyCode for escape.
	{
		uninit(0);
	}
	else if(keyCode == backwardKey)
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
	camera.panHorizontally(oldMouseX - positionX);
	camera.panVertically(oldMouseY - positionY);

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