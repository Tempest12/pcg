#include <stdexcept>
#include <string>

#include <float.h>

#include "main/Main.hpp"
#include "terrain/Generator.hpp"
#include "terrain/Region.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"
#include "util/HashTypes.hpp"

using namespace Terrain;

Generator::Generator(Util::CoordsToRegionHash* regionMap)
{
	this->tileSize   = Util::Config::convertSettingToFloat("generator", "tile_size");
	this->regionSize = Util::Config::convertSettingToFloat("generator", "region_size"); 

	this->steps = Util::Config::convertSettingToInt("generator", "subdivisions");

	this->regionMap = regionMap;

	Terrain::Tile::init();
}

Biome* Generator::getClosestBiome(int xCoord, int zCoord)
{
	Biome* biome;
	Biome* closest;

	int regionCoords[2];
	regionCoords[0] = (int)(xCoord * this->tileSize / this->regionSize);
	regionCoords[1] = (int)(zCoord * this->tileSize / this->regionSize);

	float minDistance = FLT_MAX;
	float distance = 0.0f;

	int coords[2];

	for(coords[1] = regionCoords[1] - 1;  coords[1] < regionCoords[1] + 2; coords[1]++)
	{
		for(coords[0] = regionCoords[0] - 1; coords[0] < regionCoords[0] + 2; coords[0]++)
		{
			try
			{
				distance = this->regionMap->at(coords)->getClosestBiome(xCoord * this->tileSize, zCoord * this->tileSize, &biome);
			}
			catch(std::out_of_range& oore)
			{
				std::cout << "Region Count: " << this->regionMap->size() << std::endl;
				std::cout << "Was given X: " << xCoord << " Z: " << zCoord << std::endl;
				std::cout << "Looked for region x: " << coords[0] << " z: " << coords[1] << std::endl;
				Main::die("Can't find a region while trying to find the closest Biome.");
			}

			if(distance < minDistance)
			{
				minDistance = distance;
				closest = biome;
			}
		}
	}

	return closest;
}

Region* Generator::newRegion(float x, float z)
{
	return new Region(x, z, this->regionSize);
}

Tile* Generator::newTile(float x, float z)
{
	Tile* tile = new Tile(x, z, this->tileSize, (Generator*)this);

	for(int count = 0; count < this->steps; count++)
	{
		tile->subDivide();
	}

	tile->prepareDraw();

	return tile;
}