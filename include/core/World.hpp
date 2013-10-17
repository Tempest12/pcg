#ifndef _CORE_WORLD_CPP
#define _CORE_WORLD_CPP

#include <vector>

#include "math/Vector3f.hpp"
#include "terrain/Generator.hpp"
#include "util/HashTypes.hpp"

namespace Core
{
	class World
	{
	//Variable:
	public:

		int boundarySize;
		float tileSize;

		Terrain::Generator generator;

		Util::CoordsToTileHash tileHash;

	protected:
	private:		

	//Methods:	
	public:

		World(void);
		~World(void);

		void draw(Math::Vector3f* cameraPosition, bool wired);

	protected:
	private:

	};
}

#endif
