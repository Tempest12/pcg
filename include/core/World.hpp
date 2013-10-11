#ifndef _WORLD_CPP
#define _WORLD_CPP

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

		Terrain::Generator generator;

		Util::CoordsToTileHash tiles;

	protected:
	private:		

	//Methods:	
	public:

		World(void);
		~World(void);

		void draw(Math::Vector3f* cameraPosition);

	protected:
	private:

	};
}

#endif