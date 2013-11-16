#ifndef _TERRAIN_REGION_HPP
#define _TERRAIN_REGION_HPP

#include <random>

#include "terrain/Biome.hpp"

namespace Terrain
{
	class Region
	{

	//Variables:
	public:

		int xCoord;
		int zCoord;

		float colour[4];

		float regionSize;

		Terrain::Biome** biomes;
		int biomeCount;

	protected:
	private:

	//Methods:
	public:

		Region(int xCoord, int zCoord, float regionSize);
		~Region();

		void draw(void);

		float getClosestBiome(float xCoord, float zCoord, Biome** returnPointer);
		std::minstd_rand::result_type getSeed(std::minstd_rand::result_type max);
		Biome* makeNewBiome(std::minstd_rand* random, std::poisson_distribution<int>* biomeDistribution);

	protected:
	private:

	//Static Varibles:
	public:
	protected:
	private:

	//Static Functions:
	public:
	protected:
	private:

	};

	class RegionHasher
	{
	public:
		size_t operator()(const int* key) const
		{
			return (100417 * key[0]) + (103399 * key[1]);
		}
	};

	class RegionKeyEquals
	{
	public:
		bool operator()(const int* left, const int* right) const
		{
			return (left[0] == right[0]) && (left[1] == right[1]);
		}
	};
}

#endif