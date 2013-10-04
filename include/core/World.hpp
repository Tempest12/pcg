#ifndef _WORLD_CPP
#define _WORLD_CPP

#include <vector>

#include "terrain/Generator.hpp"

namespace Core
{
	class World
	{
	//Variable:
	public:

		Terrain::Generator* generator;

	protected:
	private:		

	//Methods:	
	public:

		World(void);
		~World(void);

		void draw(void);

	protected:
	private:

	};
}

#endif