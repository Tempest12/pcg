#include <stdexcept>
#include <string>

#include <float.h>

#include "main/Main.hpp"
#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"
#include "terrain/Generator.hpp"
#include "terrain/Region.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"
#include "util/HashTypes.hpp"

using namespace Terrain;

Generator::Generator(Util::CoordsToRegionHash* regionMap)
{
	this->tileSize       = Util::Config::convertSettingToFloat("generator", "tile_size");
	this->regionSize     = Util::Config::convertSettingToFloat("generator", "region_size"); 
	this->transitionSize = Util::Config::convertSettingToFloat("generator", "transition_size");

	this->transitionSize /= 2.0f;

	this->steps = Util::Config::convertSettingToInt("generator", "subdivisions");

	this->regionMap = regionMap;

	Terrain::Tile::init();
}

void Generator::getClosestBiome(Tile* tile)
{
	int regionCoords[2];
	regionCoords[0] = (int)(tile->xCoord * this->tileSize / this->regionSize);
	regionCoords[1] = (int)(tile->zCoord * this->tileSize / this->regionSize);
	
	this->getClosestCornerBiome(regionCoords, tile->botLeftCorner, tile->botLeftBiome, tile->botLeftDistance);
	this->getClosestCornerBiome(regionCoords, tile->topLeftCorner, tile->topLeftBiome, tile->topLeftDistance);
	this->getClosestCornerBiome(regionCoords, tile->botRightCorner, tile->botRightBiome, tile->botRightDistance);
	this->getClosestCornerBiome(regionCoords, tile->topRightCorner, tile->topRightBiome, tile->topRightDistance);

	if(tile->botLeftBiome[0]->isSameType(tile->topLeftBiome[0])  &&
	   tile->botLeftBiome[0]->isSameType(tile->botRightBiome[0]) &&
	   tile->botLeftBiome[0]->isSameType(tile->topRightBiome[0]))
	{
		if(tile->botLeftBiome[0] ->isSameType(tile->botLeftBiome[1])  &&
		   tile->topLeftBiome[0] ->isSameType(tile->topLeftBiome[1])  &&
		   tile->botRightBiome[0]->isSameType(tile->botRightBiome[1]) &&
		   tile->topRightBiome[0]->isSameType(tile->topRightBiome[1]))
		{
			tile->biome = tile->botLeftBiome[0];
		}
		else
		{
			if(tile->botLeftDistance[1]  - tile->botLeftDistance[0]  < transitionSize ||
			   tile->topLeftDistance[1]  - tile->topLeftDistance[0]  < transitionSize ||
			   tile->botRightDistance[1] - tile->botRightDistance[0] < transitionSize ||
			   tile->topRightDistance[1] - tile->topRightDistance[0] < transitionSize)
			{//Transition Tile:

				tile->transitionTile = true;
				//tile->biome = tile->botLeftBiome[0];
			}
			else
			{
				tile->biome = tile->botLeftBiome[0];
			}
		}
	}
	else //Definitely a transition tile:
	{
		tile->transitionTile = true;
	}

	if(tile->transitionTile)
	{
		Math::Vector3f tileCenter = Math::Vector3f(this->tileSize * tile->xCoord, 0.0f, this->tileSize * tile->zCoord);
		float centerDistance[2];
		Biome* controllingBiomes[2];

		centerDistance[0] = FLT_MAX;
		centerDistance[1] = FLT_MAX;

		this->getClosestCornerBiome(regionCoords, &tileCenter, controllingBiomes, centerDistance);

		tile->biome    = controllingBiomes[0];
		tile->subBiome = controllingBiomes[1];
	}

	
}

void Generator::getClosestCornerBiome(int* regionCoords, Math::Vector3f* position, Biome** biomes, float* minDistance)
{
	Region* region;

	int coords[2];

	float distance = 0.0f;

	minDistance[0] = FLT_MAX;
	minDistance[1] = FLT_MAX;

	for(coords[1] = regionCoords[1] - 1;  coords[1] <= regionCoords[1] + 1; coords[1]++)
	{
		for(coords[0] = regionCoords[0] - 1; coords[0] <= regionCoords[0] + 1; coords[0]++)
		{
			try
			{
				region = this->regionMap->at(coords);
			}
			catch(std::out_of_range& oore)
			{
				std::cout << "Region Count: " << this->regionMap->size() << std::endl;
				std::cout << "Was given position X: " << position->x << " Z: " << position->z << std::endl;
				std::cout << "Looked for region x: " << coords[0] << " z: " << coords[1] << std::endl;
				Main::die("Can't find a region while trying to find the closest Biome.");
			}

			for(int index = 0; index < region->biomeCount; index++)
			{
				distance = position->distance(region->biomes[index]->position);

				if(distance < minDistance[0])
				{
					minDistance[1] = minDistance[0];
					minDistance[0] = distance;

					biomes[1] = biomes[0];
					biomes[0] = region->biomes[index];
				}
				else if(distance < minDistance[1])
				{
					minDistance[1] = distance;
					biomes[1] = region->biomes[index];
				}
			}
		}
	}
}

Region* Generator::newRegion(int x, int z)
{
	return new Region(x, z, this->regionSize);
}

Tile* Generator::newTile(int x, int z)
{
	Tile* tile = new Tile(x, z, this->tileSize, (Generator*)this);

	for(int count = 0; count < this->steps; count++)
	{
		tile->subDivide();
	}

	//tile->prepareDraw(this);

	return tile;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                           //
//                                                         Static:                                                           //
//                                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Generator* Generator::generator = NULL;

void Generator::initGenerator(Util::CoordsToRegionHash* regionMap)
{

}

Generator* Generator::getGenerator(void)
{
    return Generator::generator;
}

