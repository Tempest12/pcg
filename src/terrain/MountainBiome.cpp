#include "terrain/Biome.hpp"
#include "terrain/MountainBiome.hpp"
#include "util/Config.hpp"

//Namespace this codes belongs to:
using namespace Terrain;

MountainBiome::MountainBiome(float xCoord, float zCoord) : Biome(xCoord, zCoord)
{
	this->height = Util::Config::convertSettingToFloat("generator", "mountain_height");

	this->colour[0] = Util::Config::convertSettingToFloat("colours", "mountain_red");
	this->colour[1] = Util::Config::convertSettingToFloat("colours", "mountain_green");
	this->colour[2] = Util::Config::convertSettingToFloat("colours", "mountain_blue");
	this->colour[3] = Util::Config::convertSettingToFloat("colours", "mountain_alpha");

	std::cout << "Mountain Biome X: " << xCoord << " Z: " << zCoord << std::endl;
}

MountainBiome::~MountainBiome()
{

}