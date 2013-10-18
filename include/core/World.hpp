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

		float tileSize;

		Terrain::Generator generator;

		Util::CoordsToTileHash tileHash;

	protected:
	private:		

	//Methods:	
	public:

		World(void);
		~World(void);

		void draw(Camera* camera, bool wired);
		void maintainTiles(Math::Vector3f* position);

	protected:
	private:

	};
}

#endif
