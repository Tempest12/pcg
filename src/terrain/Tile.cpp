#include <iostream>
#include <random>
#include <sstream>

#include <float.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"
#include "terrain/Generator.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

//Namespace this codes belongs to:
using namespace Terrain;

//Defines:
#define BUFFERCOUNT 4

Tile::Tile(int xCoord, int zCoord, float size, Generator* generator)
{
	float halfSize = size / 2.0f;

	//Location Data:
	this->xCoord = xCoord;
	this->zCoord = zCoord;
	this->size   = size;

	this->botLeftCorner  = new Math::Vector3f(xCoord * size - halfSize, 2.0f, zCoord * size - halfSize);
	this->topLeftCorner  = new Math::Vector3f(xCoord * size - halfSize, 2.0f, zCoord * size + halfSize);
	this->botRightCorner = new Math::Vector3f(xCoord * size + halfSize, 2.0f, zCoord * size - halfSize);
	this->topRightCorner = new Math::Vector3f(xCoord * size + halfSize, 2.0f, zCoord * size + halfSize);

	//Biome Stuff:
	this->biome = NULL;
	this->transitionTile = false;
	
	this->botLeftDistance[0] = FLT_MAX;
	this->botLeftDistance[1] = FLT_MAX;

	this->topLeftDistance[0] = FLT_MAX;
	this->topLeftDistance[1] = FLT_MAX;

	this->botRightDistance[0] = FLT_MAX;
	this->botRightDistance[1] = FLT_MAX;

	this->topRightDistance[0] = FLT_MAX;
	this->topRightDistance[1] = FLT_MAX;

	//Rendering Data:
	this->bufferIDs = new unsigned int[BUFFERCOUNT];

	//Subdividing Data:
	this->round = 0;
	this->pointsCount = this->numberOfPoints(this->round);
	this->points      = this->buildArray(pointsCount);
	this->indexCount  = 6;
	this->vertexCount = 4;

	this->boundaryColor[0] = Util::Config::convertSettingToFloat("colours", "tile_boundary_red");
	this->boundaryColor[1] = Util::Config::convertSettingToFloat("colours", "tile_boundary_green");
	this->boundaryColor[2] = Util::Config::convertSettingToFloat("colours", "tile_boundary_blue");
	this->boundaryColor[3] = Util::Config::convertSettingToFloat("colours", "tile_boundary_alpha");

	//Determine Biome:
	//generator->getClosestBiome(this);

	//Setup initl		
	this->points[0][0] = middleHeight(this->xCoord - 1, this->zCoord - 1);
	this->points[0][1] = middleHeight(this->xCoord + 0, this->zCoord - 1);
	this->points[0][2] = middleHeight(this->xCoord + 1, this->zCoord - 1);

	this->points[1][0] = middleHeight(this->xCoord - 1, this->zCoord + 0);
	this->points[1][1] = middleHeight(this->xCoord + 0, this->zCoord + 0);
	this->points[1][2] = middleHeight(this->xCoord + 1, this->zCoord + 0);

	this->points[2][0] = middleHeight(this->xCoord - 1, this->zCoord + 1);
	this->points[2][1] = middleHeight(this->xCoord + 0, this->zCoord + 1);
	this->points[2][2] = middleHeight(this->xCoord + 1, this->zCoord + 1);

	this->hasBuffers = false;
}

Tile::~Tile()
{
	this->deleteArray();
	this->deleteBuffers();
}

void Tile::draw(bool wired, bool drawBoundaries)
{
	//Draw boundaries: (For debuging purposes
	if(drawBoundaries)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_QUADS);

			glColor4fv(this->boundaryColor);
			glVertex3f((this->xCoord - 0.5) * this->size, 1.0, (this->zCoord - 0.5) * this->size);
			glVertex3f((this->xCoord + 0.5) * this->size, 1.0, (this->zCoord - 0.5) * this->size);
			glVertex3f((this->xCoord + 0.5) * this->size, 1.0, (this->zCoord + 0.5) * this->size);
			glVertex3f((this->xCoord - 0.5) * this->size, 1.0, (this->zCoord + 0.5) * this->size);

		glEnd();
	}

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
		glEnableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[3]);
		glColorPointer(4, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
		glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else
	{
		//At this point I would say this is legcacy code. Not entirely sure this works any more with all of the other changes

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

void Tile::prepareDraw(Generator* generator)
{
	float delta = this->size / (this->pointsCount - 2);
	float startX = (this->xCoord * this->size) - (this->size / 2.0f);
	float startZ = (this->zCoord * this->size) - (this->size / 2.0f);

	float* vertexData = new float[vertexCount * 3];
	unsigned int* indexData = new unsigned int[indexCount];
	float* normalData = new float[vertexCount * 3];
	float* colourData = new float[vertexCount * 4];

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
		//Smooth Shading:
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
	else //Flat Shading
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

	//Colour Buffer:
	index =  0;
	Math::Vector3f vertexPosition = Math::Vector3f();
	Biome* biomes[2];
	int regionCoords[2];
	float distances[2];

	float halfTransitionSize = generator->transitionSize / 2.0f;

	Math::Vector3f one = Math::Vector3f();
	Math::Vector3f two = Math::Vector3f();
	Math::Vector3f pPrime = Math::Vector3f();

	float L2 = 0.0f;
	float t = 0.0f;
	float diff = 0.0f;
	float dOne = 0.0f;
	float dTwo = 0.0f;

	for(unsigned int row = 0; row < this->pointsCount - 1; row++)
	{
		for(unsigned int col = 0; col < this->pointsCount - 1; col++)
		{
			regionCoords[0] =  (int)((startX + delta * col)/ generator->regionSize);
			regionCoords[1] =  (int)((startZ + delta * row)/ generator->regionSize);

			vertexPosition.x = vertexData[vertexBufferIndex(row, col, 0)];
			vertexPosition.y = 0.0f;
			vertexPosition.z = vertexData[vertexBufferIndex(row, col, 2)];

			generator->getClosestCornerBiome(regionCoords, &vertexPosition, biomes, distances);

			L2 = biomes[0]->position->distanceSquared(biomes[1]->position);

			one = Math::Vector3f(&vertexPosition);
			two = Math::Vector3f(biomes[1]->position);

			one.subtract(biomes[0]->position);
			two.subtract(biomes[0]->position);

			t = one.dotProduct(&two) / L2;

			pPrime = Math::Vector3f(biomes[1]->position);
			pPrime.subtract(biomes[0]->position);
			pPrime.scale(t);
			pPrime.add(biomes[0]->position);

			dOne = pPrime.distance(biomes[0]->position);
			dTwo = pPrime.distance(biomes[1]->position);

			diff = abs(dOne - dTwo);

			/*if(diff <= halfTransitionSize)
			{
				float factor = (diff + halfTransitionSize) / generator->transitionSize;

				if(dOne <= dTwo)
				{
					colourData[index++] = factor * biomes[0]->colour[0] + (1.0f - factor) * biomes[1]->colour[0];
					colourData[index++] = factor * biomes[0]->colour[1] + (1.0f - factor) * biomes[1]->colour[1];
					colourData[index++] = factor * biomes[0]->colour[2] + (1.0f - factor) * biomes[1]->colour[2];
					colourData[index++] = factor * biomes[0]->colour[3] + (1.0f - factor) * biomes[1]->colour[3];
				}
				else
				{
					colourData[index++] = factor * biomes[1]->colour[0] + (1.0f - factor) * biomes[0]->colour[0];
					colourData[index++] = factor * biomes[1]->colour[1] + (1.0f - factor) * biomes[0]->colour[1];
					colourData[index++] = factor * biomes[1]->colour[2] + (1.0f - factor) * biomes[0]->colour[2];
					colourData[index++] = factor * biomes[1]->colour[3] + (1.0f - factor) * biomes[0]->colour[3];
				}

				colourData[index++] = 1.0f;
				colourData[index++] = 1.0f;
				colourData[index++] = 1.0f;
				colourData[index++] = 1.0f;
			}
			else*/
			{
				colourData[index++] = biomes[0]->colour[0];
				colourData[index++] = biomes[0]->colour[1];
				colourData[index++] = biomes[0]->colour[2];
				colourData[index++] = biomes[0]->colour[3];
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

	//Colour Buffer:
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 4, colourData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] colourData;

	this->hasBuffers = true;

	//std::cout << "Vertex Count: " << (this->vertexCount * 3)<< std::endl;
	//std::cout << "Index Count: " << this->indexCount << std::endl;
}

void Tile::subDivide(void)
{
	this->round++;
	int newCount = this->numberOfPoints(this->round);
	float** newPoints = buildArray(newCount);

	for(unsigned int row = 0; row < this->pointsCount - 1; row++)
	{
		for(unsigned int col = 0; col < this->pointsCount - 1; col++)
		{
			newPoints[row * 2 + 0][col * 2 + 0] = (9.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 1] / 16.0f);
			newPoints[row * 2 + 1][col * 2 + 0] = (9.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 1] / 16.0f);
			newPoints[row * 2 + 1][col * 2 + 1] = (9.0f * this->points[row + 1][col + 1] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 0] / 16.0f); 
			newPoints[row * 2 + 0][col * 2 + 1] = (9.0f * this->points[row + 0][col + 1] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 0] / 16.0f);
		}
	}

	if(Util::Config::convertSettingToBool("generator", "random_subdivide"))
	{
		float stdDev = kValue * pow(2, -round * hValue);

		float delta = this->size / (newCount - 2);
		float startX = (this->size * this->xCoord) - (this->size / 2.0f);
		float startZ = (this->size * this->zCoord) - (this->size / 2.0f);

		for(int row = 0; row < newCount; row++)
		{
			for(int col = 0; col < newCount; col++)
			{
				std::minstd_rand random = std::minstd_rand();
				std::normal_distribution<float> distribution = std::normal_distribution<float>(0.0, stdDev);
				random.seed(getSeed(startX + col * delta, startZ + row * delta, random.max()));

				newPoints[row][col] += distribution(random);
			}
		}
	}

	this->deleteArray();

	this->pointsCount = newCount;
	this->points = newPoints;

	this->vertexCount = (this->pointsCount - 1) * (this->pointsCount - 1);
	this->indexCount *= 4;
}

std::string Tile::toString(void)
{
	std::stringstream stream;

	stream << this;
	stream << " Tile X: " << this->xCoord << " Z: " << this->zCoord;

	return stream.str();
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
int Tile::randomBump;

int Tile::primeOne = 48859;
int Tile::primeTwo = 80309;

//Functions:
int Tile::getSeed(int xCoord, int zCoord, float maxSeed)
{
	float seed = (primeOne * xCoord) + (primeTwo * zCoord) + randomBump;
	seed = fmod(seed, maxSeed);
	return (int)seed;
}

void Tile::init(void)
{
	kValue = Util::Config::convertSettingToInt("generator", "height_scale");
	hValue = Util::Config::convertSettingToFloat("generator", "h_value");
	randomBump = Util::Config::convertSettingToInt("generator", "tile_random_bump");
}

float Tile::middleHeight(int xCoord, int zCoord)
{ 
	std::minstd_rand random = std::minstd_rand();
	std::normal_distribution<float> distribution = std::normal_distribution<float>(0.0f, kValue);
	random.seed(getSeed(xCoord, zCoord, random.max()));

	return distribution(random);
}