#include <iostream>

#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "math/Vector3f.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

//Namespace this codes belongs to:
using namespace Terrain;

//Defines:
#define BUFFERCOUNT 3

Tile::Tile(float xCoord, float zCoord, float size)
{
	//Location Data:
	this->xCoord = xCoord;
	this->zCoord = zCoord;
	this->size   = size;

	//Rendering Data:
	this->bufferIDs = new unsigned int[BUFFERCOUNT];

	//Subdividing Data:
	this->round = 0;
	this->pointsCount = this->numberOfPoints(this->round);
	this->points      = this->buildArray(pointsCount);
	this->indexCount  = 6;
	this->vertexCount = 4;

	//Setup the random generator
	this->randomNum = std::minstd_rand();
	float seed = (primeOne * this->xCoord) + (primeTwo * this->zCoord) + randomBump;
	seed = fmod(seed, this->randomNum.max());
	this->randomNum.seed(seed);
	this->distribution = std::normal_distribution<float>(0, kValue);

	//Setup initl		
	this->points[0][0] = spawnMiddle(this->xCoord - 1, this->zCoord - 1);
	this->points[0][1] = spawnMiddle(this->xCoord + 0, this->zCoord - 1);
	this->points[0][2] = spawnMiddle(this->xCoord + 1, this->zCoord - 1);

	this->points[1][0] = spawnMiddle(this->xCoord - 1, this->zCoord + 0);
	this->points[1][1] = this->distribution(this->randomNum);
	this->points[1][2] = spawnMiddle(this->xCoord + 1, this->zCoord + 0);

	this->points[2][0] = spawnMiddle(this->xCoord - 1, this->zCoord + 1);
	this->points[2][1] = spawnMiddle(this->xCoord + 0, this->zCoord + 1);
	this->points[2][2] = spawnMiddle(this->xCoord + 1, this->zCoord + 1);

	this->hasBuffers = false;
}

Tile::~Tile()
{
	this->deleteArray();
	this->deleteBuffers();
}

void Tile::draw(bool wired)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	if(wired)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(Util::Config::convertSettingToBool("render", "use_buffers") && this->hasBuffers)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
		glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_QUADS);
		{
			float delta = this->size / (this->pointsCount - 2.0f);

			float startX = (this->xCoord * this->size) - (size / 2.0f);
			float startZ = (this->zCoord * this->size) - (size / 2.0f);

			glNormal3f(0.0f, 1.0f, 0.0f);

			for(unsigned int row = 0; row < this->pointsCount - 2; row++)
			{
				for(unsigned int col = 0; col < this->pointsCount - 2; col++)
				{
					glVertex3f(startX + (col + 0) * delta, this->points[row + 0][col + 0], startZ + (row + 0) * delta);
					glVertex3f(startX + (col + 1) * delta, this->points[row + 0][col + 1], startZ + (row + 0) * delta);
					glVertex3f(startX + (col + 1) * delta, this->points[row + 1][col + 1], startZ + (row + 1) * delta);
					glVertex3f(startX + (col + 0) * delta, this->points[row + 1][col + 0], startZ + (row + 1) * delta);
				}
			}
		}
		glEnd();
	}
}

void Tile::prepareDraw(void)
{
	float delta = this->size / (this->pointsCount - 2);
	float startX = (this->xCoord * this->size) - (this->size / 2.0f);
	float startZ = (this->zCoord * this->size) - (this->size / 2.0f);

	float* vertexData = new float[vertexCount * 3];
	unsigned int* indexData = new unsigned int[indexCount];
	float* normalData = new float[vertexCount * 3];

	//Vertex Buffer:
	for(unsigned int row = 0; row < this->pointsCount - 1; row++)
	{
		for(unsigned int col = 0; col < this->pointsCount - 1; col++)
		{
			//Set X:
			vertexData[vertexBufferIndex(row, col, 0)] = startX + delta * col;

			//Set Y:
			vertexData[vertexBufferIndex(row, col, 1)] = this->points[row][col];

			//Set Z:
			vertexData[vertexBufferIndex(row, col, 2)] = startZ + delta * row;
		}
	}

	bool flip = true;
	int index = 0;
	//Index Buffer
	for(unsigned int row = 0; row < this->pointsCount - 2; row++)
	{
		for(unsigned int col = 0; col < this->pointsCount - 2; col++)
		{
			if(flip)
			{
				//Build Top Left Triangle:
				indexData[index++] = indexBufferIndex(row + 0, col + 0);
				indexData[index++] = indexBufferIndex(row + 1, col + 0);
				indexData[index++] = indexBufferIndex(row + 0, col + 1);

				//Build Bottom Right Triangle:
				indexData[index++] = indexBufferIndex(row + 1, col + 0);
				indexData[index++] = indexBufferIndex(row + 1, col + 1);
				indexData[index++] = indexBufferIndex(row + 0, col + 1);
			}
			else
			{
				//Build Top Right Triangle:
				indexData[index++] = indexBufferIndex(row + 0, col + 0);
				indexData[index++] = indexBufferIndex(row + 1, col + 1);
				indexData[index++] = indexBufferIndex(row + 0, col + 1);

				//Build Bottom Left Triangle:
				indexData[index++] = indexBufferIndex(row + 0, col + 0);
				indexData[index++] = indexBufferIndex(row + 1, col + 0);
				indexData[index++] = indexBufferIndex(row + 1, col + 1);
			}

			flip = !flip;
		}

		flip = !flip;
	}

	//Normal Buffer:
	if(Util::Config::convertSettingToBool("render", "smooth_shading"))
	{
		Math::Vector3f currentPoint = Math::Vector3f();
		Math::Vector3f edgeOne = Math::Vector3f();
		Math::Vector3f edgeTwo = Math::Vector3f();

		for(unsigned int row = 0; row < this->pointsCount - 1; row++)
		{
			for(unsigned int col = 0; col < this->pointsCount - 1; col++)
			{
				//Set Current Point
				currentPoint.x = startX + (delta * col);
				currentPoint.y = this->points[row][col];
				currentPoint.z = startZ + (delta * row);

				//Set Up Point:
				edgeOne.x = currentPoint.x;
				edgeOne.y = this->points[row + 1][col];
				edgeOne.z = currentPoint.z + delta;

				//Set Right Point:
				edgeTwo.x = currentPoint.x + delta;
				edgeTwo.y = this->points[row][col + 1];
				edgeTwo.z = currentPoint.z;

				//Create Edge Vectors:
				edgeOne.subtract(&currentPoint);
				edgeTwo.subtract(&currentPoint);

				edgeOne.crossProduct(&edgeTwo);
				edgeOne.normalize();

				//Set X:
				normalData[vertexBufferIndex(row, col, 0)] = edgeOne.x;

				//Set Y:
				normalData[vertexBufferIndex(row, col, 1)] = edgeOne.y;

				//Set Z:
				normalData[vertexBufferIndex(row, col, 2)] = edgeOne.z;
			}
		}
	}
	else
	{
		Math::Vector3f currentPoint = Math::Vector3f();
		Math::Vector3f edgeOne = Math::Vector3f();
		Math::Vector3f edgeTwo = Math::Vector3f();
		
		for(unsigned int row = 0; row < this->pointsCount - 1; row+=2)
		{
			for(unsigned int col = 0; col < this->pointsCount - 1; col+=2)
			{
				//Set Current Point
				currentPoint.x = startX + (delta * col);
				currentPoint.y = this->points[row][col];
				currentPoint.z = startZ + (delta * row);

				//Set Up Point:
				edgeOne.x = currentPoint.x;
				edgeOne.y = this->points[row + 1][col];
				edgeOne.z = currentPoint.z + delta;

				//Set Right Point:
				edgeTwo.x = currentPoint.x + delta;
				edgeTwo.y = this->points[row][col + 1];
				edgeTwo.z = currentPoint.z;

				//Create Edge Vectors:
				edgeOne.subtract(&currentPoint);
				edgeTwo.subtract(&currentPoint);

				edgeOne.crossProduct(&edgeTwo);
				edgeOne.normalize();

				//Starting Point
				normalData[vertexBufferIndex(row, col, 0)] = edgeOne.x;
				normalData[vertexBufferIndex(row, col, 1)] = edgeOne.y;
				normalData[vertexBufferIndex(row, col, 2)] = edgeOne.z;

				//Right Point:
				if(col + 1 < this->pointsCount - 1)
				{
					normalData[vertexBufferIndex(row, col + 1, 0)] = edgeOne.x;
					normalData[vertexBufferIndex(row, col + 1, 1)] = edgeOne.y;
					normalData[vertexBufferIndex(row, col + 1, 2)] = edgeOne.z;
				}

				//Top Point:
				if(row + 1 < this->pointsCount - 1)
				{
					normalData[vertexBufferIndex(row + 1, col, 0)] = edgeOne.x;
					normalData[vertexBufferIndex(row + 1, col, 1)] = edgeOne.y;
					normalData[vertexBufferIndex(row + 1, col, 2)] = edgeOne.z;
				}
			}
		}

		for(unsigned int row = 1; row < this->pointsCount - 1; row+=2)
		{
			for(unsigned int col = 1; col < this->pointsCount - 1; col+=2)
			{
				//Set Current Point
				currentPoint.x = startX + (delta * col);
				currentPoint.y = this->points[row][col];
				currentPoint.z = startZ + (delta * row);

				//Set Up Point:
				edgeOne.x = currentPoint.x;
				edgeOne.y = this->points[row + 1][col];
				edgeOne.z = currentPoint.z + delta;

				//Set Right Point:
				edgeTwo.x = currentPoint.x + delta;
				edgeTwo.y = this->points[row][col + 1];
				edgeTwo.z = currentPoint.z;

				//Create Edge Vectors:
				edgeOne.subtract(&currentPoint);
				edgeTwo.subtract(&currentPoint);

				edgeOne.crossProduct(&edgeTwo);
				edgeOne.normalize();

				//Starting Point
				normalData[vertexBufferIndex(row, col, 0)] = edgeOne.x;
				normalData[vertexBufferIndex(row, col, 1)] = edgeOne.y;
				normalData[vertexBufferIndex(row, col, 2)] = edgeOne.z;

				//Right Point:
				if(col + 1 < this->pointsCount - 1)
				{
					normalData[vertexBufferIndex(row, col + 1, 0)] = edgeOne.x;
					normalData[vertexBufferIndex(row, col + 1, 1)] = edgeOne.y;
					normalData[vertexBufferIndex(row, col + 1, 2)] = edgeOne.z;
				}

				//Top Point:
				if(row + 1 < this->pointsCount - 1)
				{
					normalData[vertexBufferIndex(row + 1, col, 0)] = edgeOne.x;
					normalData[vertexBufferIndex(row + 1, col, 1)] = edgeOne.y;
					normalData[vertexBufferIndex(row + 1, col, 2)] = edgeOne.z;
				}
			}
		}
	}

	//Delete the Old Buffers(if there are any)
	this->deleteBuffers();

	//Create Buffers
	glGenBuffers(BUFFERCOUNT, bufferIDs);

	//Vertex Buffer:
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] vertexData;

	//Index Buffer:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indexData;

	//Normal Buffer:
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, normalData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] normalData;

	this->hasBuffers = true;

	//std::cout << "Vertex Count: " << (this->vertexCount * 3)<< std::endl;
	//std::cout << "Index Count: " << this->indexCount << std::endl;
}

void Tile::subDivide(float range)
{
	this->round++;
	int newCount = this->numberOfPoints(this->round);
	float** newPoints = buildArray(newCount);

	this->distribution = std::normal_distribution<float>(0, kValue * pow(2, -round * hValue));

	for(unsigned int row = 0; row < this->pointsCount - 1; row++)
	{
		for(unsigned int col = 0; col < this->pointsCount - 1; col++)
		{
			newPoints[row * 2 + 0][col * 2 + 0] = (9.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 1] / 16.0f);
			newPoints[row * 2 + 1][col * 2 + 0] = (9.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 1] / 16.0f);
			newPoints[row * 2 + 1][col * 2 + 1] = (9.0f * this->points[row + 1][col + 1] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 0] / 16.0f); 
			newPoints[row * 2 + 0][col * 2 + 1] = (9.0f * this->points[row + 0][col + 1] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 0] / 16.0f);

			if(Util::Config::convertSettingToBool("generator", "random_subdivide"))
			{
				newPoints[row * 2 + 0][col * 2 + 0] += this->distribution(this->randomNum);
				newPoints[row * 2 + 1][col * 2 + 0] += this->distribution(this->randomNum);
				newPoints[row * 2 + 1][col * 2 + 1] += this->distribution(this->randomNum);
				newPoints[row * 2 + 0][col * 2 + 1] += this->distribution(this->randomNum);
			}
		}
	}

	this->deleteArray();

	this->pointsCount = newCount;
	this->points = newPoints;

	this->vertexCount = (this->pointsCount - 1) * (this->pointsCount - 1);
	this->indexCount *= 4;
}


//Private:
float** Tile::buildArray(int count)
{
	float** array = new float*[count];

	for(int index = 0; index < count; index++)
	{
		array[index] = new float[count];
	}

	return array;
}

void Tile::deleteArray(void)
{
	for(unsigned int index = 0; index < this->pointsCount; index++)
	{
		delete[] this->points[index];
	}

	delete[] this->points;
}

void Tile::deleteBuffers(void)
{
	if(this->hasBuffers)
	{
		this->hasBuffers = false;

		glDeleteBuffers(BUFFERCOUNT, this->bufferIDs);
	}
}

unsigned int Tile::indexBufferIndex(unsigned int row, unsigned int col)
{
	return row * (this->pointsCount - 1) + col;
}

unsigned int Tile::numberOfPoints(int level)
{
	return (unsigned int)pow(2, level) + 2;
}

unsigned int Tile::vertexBufferIndex(unsigned int row, unsigned int col, unsigned int coordinate)
{
	return (row * (this->pointsCount - 1) * 3.0f) + (col * 3.0f) + coordinate;
}

//Static Stuff:
//Variables:
float Tile::kValue;
float Tile::hValue;
float Tile::randomBump;

float Tile::primeOne = 48859.0f;
float Tile::primeTwo = 80309.0f;

//Functions:
float Tile::spawnMiddle(float xCoord, float zCoord)
{
	std::minstd_rand random = std::minstd_rand();
	std::normal_distribution<float> dist = std::normal_distribution<float>(0.0f, kValue);

	float seed = (primeOne * xCoord) + (primeTwo * zCoord) + randomBump;
	seed = fmod(seed, random.max());
	random.seed(seed);

	return dist(random);
}