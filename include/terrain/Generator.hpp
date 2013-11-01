#ifndef _TERRAIN_GENERATOR_HPP
#define _TERRAIN_GENERATOR_HPP

#include "terrain/Region.hpp"
#include "terrain/Tile.hpp"
#include "util/HashTypes.hpp"

namespace Terrain
{
	class Generator
	{
	//Variables:
	public:

		int steps;
		float tileSize;
		float regionSize;

		Util::CoordsToRegionHash* regionMap;

	protected:
	private:

	//Functions:
	public:

		Generator(Util::CoordsToRegionHash* regionMap);

		Biome* getClosestBiome(int xCoord, int zCoord);

		Region* newRegion(float x, float z);
		Tile*   newTile(float x, float z);

	protected:
	private:

	};
}

#endif
