#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "core/Camera.hpp"
#include "core/World.hpp"
#include "main/Main.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"
#include "util/Log.hpp"

using namespace Core;

World::World(void)
{
	this->cpuBoundarySize = Util::Config::convertSettingToInt("render", "cpu_boundary_size");
	this->gpuBoundarySize = Util::Config::convertSettingToInt("render", "gpu_boundary_size");
	this->renderBoundarySize = Util::Config::convertSettingToInt("render", "render_boundary_size");

	if(renderBoundarySize > gpuBoundarySize)
	{
		Main::die("The render boundary size must be equal to or less than the gpu boundary size.");
	}
	else if(gpuBoundarySize > cpuBoundarySize)
	{
		Main::die("The gpu boundary size must be equal to or less than the cpu boundary size.");
	}

	this->tileSize = Util::Config::convertSettingToFloat("generator", "tile_size");
	this->regionSize = Util::Config::convertSettingToFloat("generator", "region_size");

	std::cout << this->regionSize << std::endl;

	//Figure out what the Region Boundary should be:
	float tileSpace = cpuBoundarySize + cpuBoundarySize + 1;
	tileSpace *= tileSize;

	this->regionBoundarySize = ceil(tileSpace / this->regionSize);
	this->regionBoundarySize++;

	this->tileHash = Util::CoordsToTileHash();
	this->regionHash = Util::CoordsToRegionHash();

	this->generator = new Terrain::Generator(&this->regionHash);
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
				std::cout << "Failed to find tile at coords X: " << currentTile[0] << " Z: " << currentTile[1] << std::endl;
				std::cout << "Size: " << this->tileHash.size() << std::endl;
				Main::die("Tile not ready to be drawn.");
			}

			tile->draw(wired);
		}
	}
}

void World::maintainRegions(Math::Vector3f* position)
{
	Terrain::Region* region;

	int startRow   = (int)(position->z / this->regionSize) - this->regionBoundarySize;
	int startCol   = (int)(position->x / this->regionSize) - this->regionBoundarySize;
	int endRow = (int)(position->z / this->regionSize) + this->regionBoundarySize;
	int endCol = (int)(position->x / this->regionSize) + this->regionBoundarySize;

	int coords[2];

	int* newKey;

	std::vector<int> keyX = std::vector<int>();
	std::vector<int> keyZ = std::vector<int>();

	std::unordered_map<int*, Terrain::Region*>::const_iterator target;

	//Mark Regions for deletion:
	for(std::unordered_map<int*, Terrain::Region*>::const_iterator index = this->regionHash.begin(); index != regionHash.end(); index++)
	{
		if(index->first[1] >= startRow && index->first[1] <= endRow && 
		   index->first[0] >= startCol && index->first[1] <= endCol)
		{
			//Region is inside boundary.
		}
		else
		{
			keyX.push_back(index->first[0]);
			keyZ.push_back(index->first[1]);
		}
	}

	//Delete Regions:
	for(unsigned int index = 0; index < keyX.size(); index++)
	{
		coords[0] = keyX[index];
		coords[1] = keyZ[index];

		target = this->regionHash.find(coords);

		delete[] target->first;
		delete   target->second;
		this->regionHash.erase(target);
	}

	//Add Regions to the world.
	for(coords[1] = startRow; coords[1] <= endRow; coords[1]++)
	{
		for(coords[0] = startCol; coords[0] <= endCol; coords[0]++)
		{
			try
			{
				region = this->regionHash.at(coords);
			}
			catch(std::out_of_range& oore)
			{
				newKey = new int[2];
				newKey[0] = coords[0];
				newKey[1] = coords[1];

				region = this->generator->newRegion(coords[0], coords[1]);
				this->regionHash.emplace(newKey, region);
			}
		}
	}
}

void World::maintainTiles(Math::Vector3f* position)
{
	Terrain::Tile* tile;

	int cpuStartRow = (int)(position->z / this->tileSize) - this->cpuBoundarySize;
	int cpuStartCol = (int)(position->x / this->tileSize) - this->cpuBoundarySize;
	int cpuEndRow = (int)(position->z / this->tileSize) + this->cpuBoundarySize;
	int cpuEndCol = (int)(position->x / this->tileSize) + this->cpuBoundarySize;

	int gpuStartRow = (int)(position->z / this->tileSize) - this->gpuBoundarySize;
	int gpuStartCol = (int)(position->x / this->tileSize) - this->gpuBoundarySize;
	int gpuEndRow = (int)(position->z / this->tileSize) + this->gpuBoundarySize;
	int gpuEndCol = (int)(position->x / this->tileSize) + this->gpuBoundarySize;

	int coords[2];
	int* newKey;

	std::vector<int> keyX = std::vector<int>();
	std::vector<int> keyZ = std::vector<int>();

	std::unordered_map<int*, Terrain::Tile*>::const_iterator target;

	//Mark tiles from deletion from the CPU and GPU:
	for(std::unordered_map<int*, Terrain::Tile*>::const_iterator index = this->tileHash.begin(); index != tileHash.end(); index++)
	{
		//CPU:
		if(index->first[1] >= cpuStartRow && index->first[1] <= cpuEndRow &&
		   index->first[0] >= cpuStartCol && index->first[0] <= cpuEndCol)
		{
			//Tile is inside boundary.
		}
		else
		{
			//Add to list to be deleted later.
			keyX.push_back(index->first[0]);
			keyZ.push_back(index->first[1]);
		}

		//GPU:
		if(index->first[1] >= cpuStartRow && index->first[1] <= cpuEndRow &&
		   index->first[0] >= cpuStartCol && index->first[0] <= cpuEndCol)
		{
			//Tile is inside boundary
		}
		else
		{
			index->second->deleteBuffers();
		}
	}	

	//Now remove the tiles from the list if we need to:
	for(unsigned int index = 0; index < keyX.size(); index++)
	{
		coords[0] = keyX[index];
		coords[1] = keyZ[index];

		target = this->tileHash.find(coords);

		delete[] target->first;
		delete target->second;
		this->tileHash.erase(target);
	}

	//Add tiles to the CPU:
	for(coords[1] = cpuStartRow; coords[1] <= cpuEndRow; coords[1]++)
	{
		for(coords[0] = cpuStartCol; coords[0] <= cpuEndCol; coords[0]++)
		{
			try
			{
				tile = this->tileHash.at(coords);
			}
			catch(std::out_of_range& oore)
			{
				newKey = new int[2];
				newKey[0] = coords[0];
				newKey[1] = coords[1];
				
				tile = this->generator->newTile(coords[0], coords[1]);
				this->tileHash.emplace(newKey, tile);
			}
		}
	}

	//Add tiles to the GPU:
	for(coords[1] = gpuStartRow; coords[1] <= gpuEndRow; coords[1]++)
	{
		for(coords[0] = gpuStartCol; coords[0] <= gpuEndCol; coords[0]++)
		{
			try
			{
				tile = this->tileHash.at(coords);
			}
			catch(std::out_of_range& oore)
			{
				std::cout << "Tile not found. Coords X: " << coords[0] << " Z: " << coords[1] << std::endl;
			}

			if(!tile->hasBuffers)
			{
				tile->prepareDraw();
			}
		}
	}
}