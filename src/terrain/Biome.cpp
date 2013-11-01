#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"

//Namespace this code belongs to:
using namespace Terrain;

Biome::Biome(float xCoord, float zCoord)
{
	this->position = new Math::Vector3f(xCoord, 0.0f, zCoord);

	this->height = 0.0f;

	this->colour[0] = 0.5f;
	this->colour[1] = 1.0f;
	this->colour[2] = 1.0f;
	this->colour[3] = 1.0f;
}

Biome::~Biome()
{
}

float* Biome::getColour()
{
	return this->colour;
}

float Biome::getHeight()
{
	return this->height;
}