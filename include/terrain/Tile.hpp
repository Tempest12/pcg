#ifndef _TERRAIN_TILE_HPP
#define _TERRAIN_TILE_HPP

#include <iostream>
#include <random>

//Some Forward declarations:
namespace Math
{
	class Vector3f;
}


namespace Terrain
{
	class Biome;
	class Generator;

	class Tile
	{
	//Variables:
	public:

		//Location Data:
		float   size;                                                                                                                                                                                                                                                                                                                                                                                                            
		int   xCoord;
		int   zCoord;

		Math::Vector3f* botLeftCorner;
		Math::Vector3f* topLeftCorner;
		Math::Vector3f* botRightCorner;
		Math::Vector3f* topRightCorner;

		//Biome Data:
		Biome* biome;
		Biome* subBiome;

		Biome* botLeftBiome[2];
		Biome* topLeftBiome[2];
		Biome* botRightBiome[2];
		Biome* topRightBiome[2];

		float botLeftDistance[2];
		float topLeftDistance[2];
		float botRightDistance[2];
		float topRightDistance[2];

		bool transitionTile;

		//Rendering Data
		unsigned int* bufferIDs;
			// 0 -> Vertex Buffer
			// 1 -> Index Buffer
			// 2 -> Normal Buffer
			// 3 -> Colour Buffer
		bool hasBuffers;

		float boundaryColor[4];

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
		void draw(bool wired, bool drawBoundaries);
		void prepareDraw(Generator* generator);
		void subDivide(void);
		std::string toString(void);

	protected:
	private:
		float** buildArray(int count);
		void deleteArray(void);

		unsigned int indexBufferIndex(unsigned int row, unsigned int col);
		unsigned int numberOfPoints(int level);
		unsigned int vertexBufferIndex(unsigned int row, unsigned int col, unsigned int coordinate);
		
	//Static Variables
	public:
		static float kValue;
		static float hValue;
		static int randomBump;
    
    protected:
	private:
		static int bufferCount;

		static int primeOne;
		static int primeTwo;


	//Static Functions:
	public:

		//static Biome* getClosestBiome(int xCoord, int zCoord, Util::CoordsToRegionHash* regionHash);
		static int getSeed(int xCoord, int zCoord, float seedMax);
		static void init(void);
		static float middleHeight(int xCoord, int zCoord);

    protected:
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
