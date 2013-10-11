#include "terrain/Generator.hpp"
#include "terrain/Tile.hpp"
#include "util/Config.hpp"

using namespace Terrain;

Generator::Generator(void)
{
	this->tileSize = Util::Config::convertSettingToFloat("generator", "tile_size");
}

Tile* Generator::newTile(float x, float z, int steps)
{
	Tile* tile = new Tile(x, z, this->tileSize);

	for(int count = 0; count < steps; count++)
	{
		tile->subDivide();
	}

	tile->prepareDraw();

	return tile;
}