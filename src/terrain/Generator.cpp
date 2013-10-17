#include "terrain/Generator.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

using namespace Terrain;

Generator::Generator(void)
{
	this->tileSize = Util::Config::convertSettingToFloat("generator", "tile_size");

	this->steps = Util::Config::convertSettingToInt("generator", "subdivisions");

	Terrain::Tile::init();
}

Tile* Generator::newTile(float x, float z)
{
	Tile* tile = new Tile(x, z, this->tileSize);

	for(int count = 0; count < this->steps; count++)
	{
		tile->subDivide();
	}

	tile->prepareDraw();

	return tile;
}