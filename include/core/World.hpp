#ifndef _CORE_WORLD_CPP
#define _CORE_WORLD_CPP

#include <vector>

#include "core/Camera.hpp"
#include "math/Vector3f.hpp"
#include "terrain/Generator.hpp"
#include "util/HashTypes.hpp"

namespace Core
{
	class World
	{
	//Variable:
	public:

		int cpuBoundarySize;
		int gpuBoundarySize;
		int renderBoundarySize;

		int regionBoundarySize;

		float tileSize;
		float regionSize;

		Terrain::Generator* generator;

		Util::CoordsToTileHash tileHash;
		Util::CoordsToRegionHash regionHash;

	protected:
	private:		

	//Methods:	
	public:

		World(void);
		~World(void);

		void draw(Camera* camera, bool wired, bool drawRegions);
		void maintainRegions(Math::Vector3f* position);
		void maintainTiles(Math::Vector3f* position);

	protected:
	private:

	};
}

#endif
