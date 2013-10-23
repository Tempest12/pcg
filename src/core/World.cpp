#include <iostream>
#include <sstream>
#include <stdexcept>

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
				std::cout << "Failed to find tile at coords X: " << currentTile[0] << " Z: " << currentTile[1] << std::endl;
				std::cout << "Size: " << this->tileHash.size() << std::endl;
				Main::die("Tile not ready to be drawn.");
			}

			tile->draw(wired);
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

	//Delete tiles from CPU and GPU:
	for(auto index = this->tileHash.begin(); index != this->tileHash.end(); index++)
	{
		//CPU:
		if(index->first[1] >= cpuStartRow && index->first[1] <= cpuEndRow &&
		   index->first[0] >= cpuStartCol && index->first[0] <= cpuEndCol)
		{
			//Tile is inside boundary.
		}
		else
		{
			/*std::stringstream stream;
			stream.clear();
			stream << "Erasing Tile X: " << index->first[0] << " Z: " << index->first[1];
			Util::Log::writeDebug(stream.str());*/

			delete[] index->first;
			delete index->second;
			this->tileHash.erase(index, index);
			continue;			
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
				/*std::stringstream stream;
				stream.clear();
				stream << "Adding Tile X: " << coords[0] + " Z: " + coords[1];
				Util::Log::writeDebug(stream.str());*/

				newKey = new int[2];
				newKey[0] = coords[0];
				newKey[1] = coords[1];
				tile = this->generator.newTile(coords[0], coords[1]);
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