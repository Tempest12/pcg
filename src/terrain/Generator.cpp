#include <terrain/Generator.hpp>
#include <terrain/Tile.hpp>

using namespace Terrain;

Generator::Generator(float tileSize)
{
	this->tileSize = tileSize;
}

Tile* Generator::newTile(float x, float z, int steps)
{
	Tile* tile = new Tile(x, z, this->tileSize);

	for(int count = 0; count < steps; count++)
	{
		//tile->subDivide(0.0f);
	}

	return tile;
}