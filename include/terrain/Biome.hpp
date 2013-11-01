#ifndef _TERRAIN_BIOME_HPP
#define _TERRAIN_BIOME_HPP

#include "math/Vector3f.hpp"

namespace Terrain
{
	class Biome
	{

	//Variables:
	public:

		Math::Vector3f* position;

		float height;
		float colour[4];

	protected:
	private:

	//Methods:
	public:

		Biome(float xCoord, float zCoord);
		~Biome();

		virtual float* getColour(void);
		virtual float  getHeight(void);

	protected:
	private:

	};
}

#endif