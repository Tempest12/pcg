#ifndef _GENERATOR_HPP
#define _GENERATOR_HPP

#include <terrain/Tile.hpp>

namespace Terrain
{
	class Generator
	{
	//Variables:
	public:

		float tileSize;

	protected:
	private:

	//Functions:
	public:

		Generator(void);

		Tile* newTile(float x, float z, int steps);

	protected:
	private:

	};
}

#endif