#ifndef _TILE_HPP
#define _TILE_HPP

#include <random>

#include <GL/glut.h>

namespace Terrain
{
	class Tile
	{
	//Variables:
	public:

		//Location Data
		float   size;                                                                                                                                                                                                                                                                                                                                                                                                            
		int   xCoord;
		int   zCoord;

		//Rendering Data
		unsigned int* bufferIDs;
			// 0 -> Vertex Buffer
			// 1 -> Index Buffer
			// 2 -> Normal Buffer

		//Subdividing Data
		int round;

		float** points;
		unsigned int pointsCount;
        unsigned int indexCount;
        unsigned int vertexCount;  
		
	protected:
	private:

		bool hasBuffers;

	//Methods:
	public:

		Tile(int xCoord, int zCoord, float size);
		~Tile();

		void draw(bool wired);
		void prepareDraw(void);
		void subDivide(void);

	protected:
	private:
		float** buildArray(int count);
		void deleteArray(void);
		void deleteBuffers(void);

		unsigned int indexBufferIndex(unsigned int row, unsigned int col);
		unsigned int numberOfPoints(int level);
		unsigned int vertexBufferIndex(unsigned int row, unsigned int col, unsigned int coordinate);
		
	//Static 
	//Variables
	public:
		static float kValue;
		static float hValue;
		static float randomBump;

	private:
		static int bufferCount;

		static float primeOne;
		static float primeTwo;


	//Methods:
	public:

		static int getSeed(float xCoord, float zCoord, float seedMax);
		static void init(void);
		static float middleHeight(float xCoord, float zCoord);


	private:
	};

	//Used strictly to allow hashing of a tile object:
	class TileHasher
	{
	public:
		size_t operator()(const float* key)
		{
			if(key != NULL)
			{
				return (48859 * key[0]) + (80309 * key[1]);
			}
			else
			{
				return 0;
			}
		}
	};

	//Used to determine if two keys are the same:
	class TileKeyEquals
	{
	public:
		bool operator()(const float* left, const float* right)
		{
			if(left != NULL && right != NULL)
			{
				return (left[0] == right[0]) && (left[1] == right[1]);
			}
			else
			{
				return false;
			}
		}
	};
}

#endif