#ifndef _TERRAIN_GENERATOR_HPP
#define _TERRAIN_GENERATOR_HPP

#include "terrain/Region.hpp"
#include "terrain/Tile.hpp"
#include "util/HashTypes.hpp"

namespace Math
{
	class Vector3f;
}

namespace Terrain
{
	class Biome;

	class Generator
	{
	//Variables:
	public:

		int steps;
		float tileSize;
		float regionSize;
		float transitionSize;

		Util::CoordsToRegionHash* regionMap;

	protected:
	private:

	//Functions:
	public:

		Generator(Util::CoordsToRegionHash* regionMap);

		void getClosestBiome(Tile* tile);
		void getClosestCornerBiome(int* regionCoords, Math::Vector3f* position, Biome** biome, float* minDistance);

		Region* newRegion(int x, int z);
		Tile*   newTile(int x, int z);

		void setSecondClosestBiome(Tile* tile);

	protected:
	private:

	};
}

#endif
