#ifndef _TERRAIN_BIOME_HPP
#define _TERRAIN_BIOME_HPP

#include <string>

//Forward declarations:
namespace Math
{
	class Vector3f;
}

namespace Terrain
{
	class Biome
	{

	//Variables:
	public:

		Math::Vector3f* position;

		float height;
		float colour[4];

		std::string type;

	protected:
	private:

	//Methods:
	public:

		Biome(float xCoord, float zCoord);
		~Biome();

		virtual float* getColour(void);
		virtual float  getHeight(void);

		virtual bool   isSameType(Biome* that);

		virtual void   printName(void);


	protected:
	private:

	};
}

#endif