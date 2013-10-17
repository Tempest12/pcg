#ifndef _TERRAIN_GENERATOR_HPP
#define _TERRAIN_GENERATOR_HPP

#include <terrain/Tile.hpp>

namespace Terrain
{
	class Generator
	{
	//Variables:
	public:

		int steps;
		float tileSize;

	protected:
	private:

	//Functions:
	public:

		Generator(void);

		Tile* newTile(float x, float z);

	protected:
	private:

	};
}

#endif
