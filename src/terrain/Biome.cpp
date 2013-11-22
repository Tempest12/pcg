#include <iostream>

#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"
#include "util/StringLib.hpp"

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

	this->type = "Nothing...";
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

bool Biome::isSameType(Biome* that)
{
	return Util::StringLib::equalsIgnoreCase(&this->type, &that->type);
}

void Biome::printName()
{
	std::cout << this->type << " " << *this->position << std::endl;
}