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

		//Subdividing Data
		int     round;

		float** points;
		float   pointsCount;
		
	protected:
	private:

		//Generation Data:
		std::minstd_rand randomNum;
		std::normal_distribution<float> distribution;


	//Methods:
	public:

		Tile(float xCoord, float zCoord, float size);
		~Tile();

		void draw(bool wired);
		void subDivide(float range);

	protected:
	private:
		float** buildArray(int count);
		void deleteArray(void);

		void prepareDraw(void);
		
		int numberOfPoints();

		unsigned int vertexIndex(int row, int col, int dimension);
		unsigned int vertexIndex(int row, int col);
		
	//Static 
	//Variables
	public:
		static float kValue;
		static float hValue;
		static float randomBump;

	private:
		static float primeOne;
		static float primeTwo;


	//Methods:
	public:

		static float spawnMiddle(float xCoord, float zCoord);

	private:
	};
}

#endif