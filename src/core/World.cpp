#include "core/World.hpp"
#include "util/Config.hpp"

using namespace Core;

World::World(void)
{
	this->boundarySize = Util::Config::convertSettingToInt("render", "boundary_size");

	this->generator = Terrain::Generator();

	this->tiles = Util::CoordsToTileHash();
}

World::~World(void)
{
}

void World::draw(Math::Vector3f* cameraPosition)
{
	//Current tile:
	int tileNum[2];
		//0 = X
		//1 = Z

	//tileNum[0]

	//Look at the Hash map and see if any tiles need to be generated:

}