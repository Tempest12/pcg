#include <iostream>

#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "terrain/Tile.hpp"

//Namespace this codes belongs to:
using namespace Terrain;

Tile::Tile(float xCoord, float zCoord, float size)
{
	//Location Data:
	this->xCoord = xCoord;
	this->zCoord = zCoord;
	this->size   = size;

	//Rendering Data:
	this->bufferIDs = new unsigned int[2];

	//Subdividing Data:
	this->round = 0;
	this->pointsCount = this->numberOfPoints();
	this->points      = this->buildArray(pointsCount);

	//Setup the random generator
	this->randomNum = std::minstd_rand();
	float seed = (primeOne * this->xCoord) + (primeTwo * this->zCoord) + randomBump;
	seed = fmod(seed, this->randomNum.max());
	this->randomNum.seed(seed);
	this->distribution = std::normal_distribution<float>(0, kValue);

	//Setup initl		
	this->points[0][0] = spawnMiddle(this->xCoord - 1, this->zCoord - 1);
	this->points[0][1] = spawnMiddle(this->xCoord - 1, this->zCoord - 0);
	this->points[0][2] = spawnMiddle(this->xCoord - 1, this->zCoord + 1);

	this->points[1][0] = spawnMiddle(this->xCoord - 0, this->zCoord - 1);
	this->points[1][1] = this->distribution(this->randomNum);
	this->points[1][2] = spawnMiddle(this->xCoord - 0, this->zCoord + 1);

	this->points[2][0] = spawnMiddle(this->xCoord + 1, this->zCoord - 1);
	this->points[2][1] = spawnMiddle(this->xCoord + 1, this->zCoord - 0);
	this->points[2][2] = spawnMiddle(this->xCoord + 1, this->zCoord + 1);
}

Tile::~Tile()
{

}

void Tile::draw(bool wired)
{
	/*glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	float delta = this->size / (this->pointsCount - 1.0f);

	for(int row = 0; row < this->pointsCount - 1; row++)
	{
		for(int col = 0; col < this->pointsCount - 1; col++)
		{
			glVertex3f(this->xCoord + (col + 0) * delta, this->points[row + 0][col + 0], this->zCoord + (row + 0) * delta);
			glVertex3f(this->xCoord + (col + 1) * delta, this->points[row + 0][col + 1], this->zCoord + (row + 0) * delta);
			glVertex3f(this->xCoord + (col + 1) * delta, this->points[row + 1][col + 1], this->zCoord + (row + 1) * delta);
			glVertex3f(this->xCoord + (col + 0) * delta, this->points[row + 1][col + 0], this->zCoord + (row + 1) * delta);

			//std::cout << "X: " << this->xCoord << " Z: " << this->zCoord << " Heights: " << this->points[row][col] << std::endl;

		}
	}

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Tile::subDivide(float range)
{
	this->round++;
	int newCount = this->numberOfPoints();
	float** newPoints = buildArray(newCount);

	this->distribution = std::normal_distribution<float>(0, kValue * pow(2, -round * hValue));

	for(int row = 0; row < this->pointsCount - 1; row++)
	{
		for(int col = 0; col < this->pointsCount - 1; col++)
		{
			newPoints[row * 2 + 0][col * 2 + 0] = (9.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 1] / 16.0f) + this->distribution(this->randomNum);
			newPoints[row * 2 + 1][col * 2 + 0] = (9.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 1] / 16.0f) + this->distribution(this->randomNum);
			newPoints[row * 2 + 1][col * 2 + 1] = (9.0f * this->points[row + 1][col + 1] / 16.0f) + (3.0f * this->points[row + 1][col + 0] / 16.0f) + (3.0f * this->points[row + 0][col + 1] / 16.0f) + (1.0f * this->points[row + 0][col + 0] / 16.0f) + this->distribution(this->randomNum);
			newPoints[row * 2 + 0][col * 2 + 1] = (9.0f * this->points[row + 0][col + 1] / 16.0f) + (3.0f * this->points[row + 0][col + 0] / 16.0f) + (3.0f * this->points[row + 1][col + 1] / 16.0f) + (1.0f * this->points[row + 1][col + 0] / 16.0f) + this->distribution(this->randomNum);
		}
	}

	this->deleteArray();

	this->pointsCount = newCount;
	this->points = newPoints;
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
	for(int index = 0; index < this->pointsCount; index++)
	{
		delete[] this->points[index];
	}

	delete[] this->points;
}

void Tile::prepareDraw(void)
{
	int vertexCount = (this->pointsCount * this->pointsCount) * 3;
	int indexCount = (this->pointsCount - 1) * (this->pointsCount) * 6;

	float delta = this->size / (this->pointsCount - 1);

	float* vertexData = new float[vertexCount];
	unsigned int* indexData = new unsigned int[indexCount];

	//Vertex Buffer:
	for(int row = 0; row < this->pointsCount; row++)
	{
		for(int col = 0; col < this->pointsCount; col++)
		{
			//Set X:
			vertexData[vertexIndex(row, col, 0)] = this->xCoord + delta * col;

			//Set Y:
			vertexData[vertexIndex(row, col, 1)] = this->points[row][col];

			//Set Z:
			vertexData[vertexIndex(row, col, 1)] = this->zCoord + delta * row;
		}
	}

	bool flip = true;
	int index = 0;
	//Index Buffer
	for(int row = 0; row < this->pointsCount - 1; row++)
	{
		for(int col = 0; col < this->pointsCount - 1; col++)
		{
			if(flip)
			{
				//Build Top Left Triangle:
				indexData[index++] = vertexIndex(row + 0, col + 0);
				indexData[index++] = vertexIndex(row + 1, col + 0);
				indexData[index++] = vertexIndex(row + 0, col + 1);

				//Build Bottom Right Triangle:
				indexData[index++] = vertexIndex(row + 1, col + 0);
				indexData[index++] = vertexIndex(row + 1, col + 1);
				indexData[index++] = vertexIndex(row + 0, col + 1);
			}
			else
			{
				//Build Top Right Triangle:
				indexData[index++] = vertexIndex(row + 0, col + 0);
				indexData[index++] = vertexIndex(row + 1, col + 1);
				indexData[index++] = vertexIndex(row + 0, col + 1);

				//Build Bottom Left Triangle:
				indexData[index++] = vertexIndex(row + 0, col + 0);
				indexData[index++] = vertexIndex(row + 1, col + 0);
				indexData[index++] = vertexIndex(row + 1, col + 1);
			}

			flip = !flip;
		}
	}

	//Send Buffers to GPU Mem:
	glGenBuffers(2, bufferIDs);

	//Vertex Buffer:
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertexData, GL_STATIC_DRAW);
	delete[] vertexData;

	//Index Buffer:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	delete[] indexData;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int Tile::numberOfPoints()
{
	return (int)pow(2, round) + 2;
}

unsigned int Tile::vertexIndex(int row, int col, int dimension)
{
	return row * this->pointsCount + col * 3 + dimension;
}

unsigned int Tile::vertexIndex(int row, int col)
{
	return row * this->pointsCount + col;
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