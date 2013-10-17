#include <iostream>
#include <stdexcept>

#include "core/World.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

using namespace Core;

World::World(void)
{
	this->boundarySize = Util::Config::convertSettingToInt("render", "boundary_size");
	this->tileSize = Util::Config::convertSettingToFloat("generator", "tile_size");

	this->generator = Terrain::Generator();

	this->tileHash = Util::CoordsToTileHash();
}

World::~World(void)
{
}

void World::draw(Math::Vector3f* cameraPosition, bool wired)
{
	int currentTile[2];

	int startRow = (int)(cameraPosition->z / this->tileSize) - this->boundarySize;
	int startCol = (int)(cameraPosition->x / this->tileSize) - this->boundarySize;
	int endRow = (int)(cameraPosition->z / this->tileSize) + this->boundarySize;
	int endCol = (int)(cameraPosition->x / this->tileSize) + this->boundarySize;

	//Look at the Hash map and see if any tiles need to be generated either war draw it to.
	for(currentTile[1] = startRow; currentTile[1] <= endRow; currentTile[1]++)
	{
		for(currentTile[0] = startCol; currentTile[0] <= endCol; currentTile[0]++)
		{
			Terrain::Tile* tile;

			try
			{
				tile = this->tileHash.at(currentTile);
			}
			catch(std::out_of_range& oore)
			{
				tile = this->generator.newTile(currentTile[0], currentTile[1]);

				this->tileHash.emplace(currentTile, tile);
			}

			tile->draw(wired);
		}
	}
}