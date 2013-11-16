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
		float tileDistance;
		float regionSize;
		float transitionSize;

		Util::CoordsToRegionHash* regionMap;

	protected:
	private:

	//Functions:
	public:

		Generator(Util::CoordsToRegionHash* regionMap);

		void getClosestBiome(int xCoord, int zCoord, Tile* tile);

		Region* newRegion(int x, int z);
		Tile*   newTile(int x, int z);

	protected:
	private:

	};
}

#endif
