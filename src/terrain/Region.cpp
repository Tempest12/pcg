#include <random>

#include "main/Main.hpp"
#include "terrain/Biome.hpp"
#include "terrain/DesertBiome.hpp"
#include "terrain/MountainBiome.hpp"
#include "terrain/Region.hpp"
#include "util/Config.hpp"

//Namespace this code belongs to:
using namespace Terrain;

Region::Region(int xCoord, int zCoord, float regionSize)
{
	this->xCoord = xCoord;
	this->zCoord = zCoord;

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

float Region::getClosestBiome(float xCoord, float zCoord, Biome** returnPointer)
{
	Math::Vector3f tilePosition = Math::Vector3f(xCoord, 0.0f, zCoord);
	float minDistance =  tilePosition.distanceSquared(this->biomes[0]->position);
	float distance = 0.0f;
	*returnPointer = this->biomes[0];

	for(int index = 1; index < this->biomeCount; index++)
	{
		distance = tilePosition.distanceSquared(this->biomes[index]->position);

		if(minDistance > distance)
		{
			minDistance = distance;
			*returnPointer = this->biomes[index];
		}
	}

	return distance;
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
	float biomeX = (float)biomeDistribution[0](*random) / (float)biomeDistribution->max();
	float biomeZ = (float)biomeDistribution[0](*random) / (float)biomeDistribution->max();
	float type = (float)biomeDistribution[0](*random) / (float)biomeDistribution->max();

	biomeX = ((this->xCoord - 0.5f) * this->regionSize) + biomeX * this->regionSize;
	biomeZ = ((this->zCoord - 0.5f) * this->regionSize) + biomeZ * this->regionSize;

	Biome* biome;

	if(type > 1.0f || type < 0.0f)
	{
		Main::die("The poisson distribution doesn't work like you think it does cause type is not in the space from 0 to 1.");
	}

	float desertChance   = Util::Config::convertSettingToFloat("generator", "desert_chance");
	//float mountainChance = Util::Config::convertSettingToFloat("generator", "mountain_chance");

	if(type < desertChance)
	{
		biome = new DesertBiome(biomeX, biomeZ);
	}
	else
	{
		biome = new MountainBiome(biomeX, biomeZ);
	}

	return biome;
}