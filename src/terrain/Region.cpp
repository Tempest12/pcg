#include <random>

#include <GL/glew.h>
#include <GL/glut.h>

#include "main/Main.hpp"
#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"
#include "terrain/DesertBiome.hpp"
#include "terrain/JungleBiome.hpp"
#include "terrain/MountainBiome.hpp"
#include "terrain/SnowBiome.hpp"
#include "terrain/Region.hpp"
#include "util/Config.hpp"
#include "util/Log.hpp"

//Namespace this code belongs to:
using namespace Terrain;

Region::Region(int xCoord, int zCoord, float regionSize)
{
	this->xCoord = xCoord;
	this->zCoord = zCoord;

	this->colour[0] = Util::Config::convertSettingToFloat("colours", "region_red");
	this->colour[1] = Util::Config::convertSettingToFloat("colours", "region_green");
	this->colour[2] = Util::Config::convertSettingToFloat("colours", "region_blue");
	this->colour[3] = Util::Config::convertSettingToFloat("colours", "region_alpha");

	this->regionSize = regionSize;

	std::minstd_rand random = std::minstd_rand();
	std::poisson_distribution<int> distribution = std::poisson_distribution<int>(Util::Config::convertSettingToDouble("generator", "biome_density"));
	random.seed(this->getSeed(random.max()));

	std::poisson_distribution<int> biomeDistribution = std::poisson_distribution<int>(distribution(random));

	//Get number of biomes in region:
	this->biomeCount = distribution(random);

	if(biomeCount <= 0)
	{
		this->biomeCount = 1;
	}

	this->biomes = new Biome*[biomeCount];

	for(int index = 0; index < biomeCount; index++)
	{
		this->biomes[index] = this->makeNewBiome(&random, &biomeDistribution);
	}
}

Region::~Region()
{
	delete[] this->biomes;
}

void Region::draw(void)
{
	glColor4fv(this->colour);

	float botRow   = ((float)this->zCoord - 0.5f) * this->regionSize;
	float topRow   = ((float)this->zCoord + 0.5f) * this->regionSize;
	float leftCol  = ((float)this->xCoord - 0.5f) * this->regionSize;
	float rightCol = ((float)this->xCoord + 0.5f) * this->regionSize;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);

		glNormal3f(0.0f, 1.0f, 0.0f);

		glVertex3f(leftCol , 0.1f, botRow);
		glVertex3f(leftCol , 0.1f, topRow);
		glVertex3f(rightCol, 0.1f, topRow);
		glVertex3f(rightCol, 0.1f, botRow);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Draw Biome Markers:
	for(int index = 0; index < this->biomeCount; index++)
	{
		glPushMatrix();

			glColor4fv(this->biomes[index]->colour);
			glTranslatef(this->biomes[index]->position->x, 0.1f, this->biomes[index]->position->z);
			glutSolidSphere(1.0f, 8, 8);			

		glPopMatrix();
	}
}

float Region::getClosestBiome(float xCoord, float zCoord, Biome** returnPointer)
{
	Math::Vector3f tilePosition = Math::Vector3f(xCoord, 0.0f, zCoord);
	float minDistance =  tilePosition.distanceSquared(this->biomes[0]->position);
	float distance = 0.0f;
	*returnPointer = this->biomes[0];

	for(int index = 1; index < this->biomeCount; index++)
	{
		distance = tilePosition.distanceSquared(this->biomes[index]->position);

		if(distance < minDistance)
		{
			minDistance = distance;
			*returnPointer = this->biomes[index];
		}
	}

	return minDistance;
}

std::minstd_rand::result_type Region::getSeed(std::minstd_rand::result_type maxSeed)
{
	float primeOne = 100417;
	float primeTwo = 103399;
	float randomBump = Util::Config::convertSettingToFloat("generator", "region_random_bump");


	float seed = (this->xCoord * primeOne) + (this->zCoord * primeTwo) + randomBump;
	seed = fmod(seed, maxSeed);
	return (std::minstd_rand::result_type)seed;
}

Biome* Region::makeNewBiome(std::minstd_rand* random, std::poisson_distribution<int>* biomeDistribution)
{
	std::uniform_real_distribution<float> uniformDistribution = std::uniform_real_distribution<float>(0.0, 1.0);

	float biomeX = uniformDistribution(*random);
	float biomeZ = uniformDistribution(*random);
	float type = uniformDistribution(*random);

	biomeX = ((this->xCoord - 0.5f) * this->regionSize) + biomeX * this->regionSize;
	biomeZ = ((this->zCoord - 0.5f) * this->regionSize) + biomeZ * this->regionSize;

	Biome* biome;

	float desertChance    = Util::Config::convertSettingToFloat("generator", "desert_chance");
	float jungleChance    = Util::Config::convertSettingToFloat("generator", "jungle_chance");
	float mountainChance = Util::Config::convertSettingToFloat("generator", "mountain_chance");
	float snowChance     = Util::Config::convertSettingToFloat("generator", "snow_chance");
	
	float sum = desertChance + jungleChance + mountainChance + snowChance;
    
    type *= sum;

    if(type < desertChance)
    {
    	biome = new DesertBiome(biomeX, biomeZ);
    }
    else if(type < desertChance + jungleChance)
    {
    	biome = new JungleBiome(biomeX, biomeZ);
    }
    else if(type < desertChance + jungleChance + mountainChance)
    {
    	biome = new MountainBiome(biomeX, biomeZ);
    }
    else
    {
    	biome = new SnowBiome(biomeX, biomeZ);
    }


	return biome;
}