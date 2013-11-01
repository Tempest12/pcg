#ifndef _TERRAIN_TILE_HPP
#define _TERRAIN_TILE_HPP

#include <iostream>
#include <random>

#include <GL/glut.h>

namespace Terrain
{
	class Biome;
	class Generator;

	class Tile
	{
	//Variables:
	public:

		//Location Data
		float   size;                                                                                                                                                                                                                                                                                                                                                                                                            
		int   xCoord;
		int   zCoord;

		Biome* biome;

		//Rendering Data
		unsigned int* bufferIDs;
			// 0 -> Vertex Buffer
			// 1 -> Index Buffer
			// 2 -> Normal Buffer
		bool hasBuffers;

		//Subdividing Data
		int round;

		float** points;
		unsigned int pointsCount;
        unsigned int indexCount;
        unsigned int vertexCount;  
		
	protected:
	private:

	//Methods:
	public:

		Tile(int xCoord, int zCoord, float size, Generator* generator);
		~Tile();

		void deleteBuffers(void);
		void draw(bool wired);
		void prepareDraw(void);
		void subDivide(void);
		std::string toString(void);

	protected:
	private:
		float** buildArray(int count);
		void deleteArray(void);

		unsigned int indexBufferIndex(unsigned int row, unsigned int col);
		unsigned int numberOfPoints(int level);
		unsigned int vertexBufferIndex(unsigned int row, unsigned int col, unsigned int coordinate);
		
	//Static 
	//Variables
	public:
		static float kValue;
		static float hValue;
		static int randomBump;

	private:
		static int bufferCount;

		static int primeOne;
		static int primeTwo;


	//Methods:
	public:

		//static Biome* getClosestBiome(int xCoord, int zCoord, Util::CoordsToRegionHash* regionHash);
		static int getSeed(int xCoord, int zCoord, float seedMax);
		static void init(void);
		static float middleHeight(int xCoord, int zCoord);


	private:
	};

	//Used strictly to allow hashing of a tile object:
	class TileHasher
	{
	public:
		size_t operator()(const int* key) const
		{
			return (48859 * key[0]) + (80309 * key[1]);
		}
	};

	//Used to determine if two keys are the same:
	class TileKeyEquals
	{
	public:
		bool operator()(const int* left, const int* right) const
		{
			return (left[0] == right[0]) && (left[1] == right[1]);
		}
	};
}

#endif
