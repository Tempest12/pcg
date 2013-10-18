#include <iostream>
#include <stdexcept>

#include "core/Camera.hpp"
#include "core/World.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

using namespace Core;

World::World(void)
{
	this->cpuBoundarySize = Util::Config::convertSettingToInt("render", "cpu_boundary_size");
	this->gpuBoundarySize = Util::Config::convertSettingToInt("render", "gpu_boundary_size");
	this->renderBoundarySize = Util::Config::convertSettingToInt("render", "render_boundary_size");

	this->tileSize = Util::Config::convertSettingToFloat("generator", "tile_size");

	this->generator = Terrain::Generator();

	this->tileHash = Util::CoordsToTileHash();
}

World::~World(void)
{
}

void World::draw(Camera* camera, bool wired)
{
	int currentTile[2];

	int startRow = (int)(camera->position.z / this->tileSize) - this->renderBoundarySize;
	int startCol = (int)(camera->position.x / this->tileSize) - this->renderBoundarySize;
	int endRow = (int)(camera->position.z / this->tileSize) + this->renderBoundarySize;
	int endCol = (int)(camera->position.x / this->tileSize) + this->renderBoundarySize;

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

void World::maintainTiles(Math::Vector3f* position)
{
	/*int cpuEndRow = (int)(position->z / this->tileSize) - this->cpuBoundarySize;
	int cpuEndCol = (int)(position->x / this->tileSize) - this->cpuBoundarySize;
	int cpuStartRow = (int)(position->z / this->tileSize) + this->cpuBoundarySize;
	int cpuStartCol = (int)(position->x / this->tileSize) + this->cpuBoundarySize;

	int gpuEndRow = (int)(position.z / this->tileSize) - this->gpuBoundarySize;
	int gpuEndCol = (int)(position.x / this->tileSize) - this->gpuBoundarySize;
	int gpuStartRow = (int)(position.z / this->tileSize) + this->gpuBoundarySize;
	int gpuStartCol = (int)(position.x / this->tileSize) + this->gpuBoundarySize;*/

	//for()
	{
		//for()
	}	
}