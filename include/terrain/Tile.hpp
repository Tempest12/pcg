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
		float   xCoord;
		float   zCoord;

		//Rendering Data
		unsigned int* bufferIDs;
			// 0 -> Vertex Buffer
			// 1 -> Index Buffer
			// 2 -> Normal Buffer

		//Subdividing Data
		int     round;

		float** points;
		unsigned int pointsCount;
        unsigned int indexCount;
        unsigned int vertexCount;  
		
	protected:
	private:

		bool hasBuffers;

		//Generation Data:
		std::minstd_rand randomNum;
		std::normal_distribution<float> distribution;

	//Methods:
	public:

		Tile(float xCoord, float zCoord, float size);
		~Tile();

		void draw(bool wired);
		void prepareDraw(void);
		void subDivide(float range);

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
		static float primeOne;
		static float primeTwo;

		static int bufferCount;
	//Methods:
	public:

		static float spawnMiddle(float xCoord, float zCoord);

	private:
	};
}

#endif