#include "terrain/JungleBiome.hpp"
#include "util/Config.hpp"

using namespace Terrain;

JungleBiome::JungleBiome(float xCoord, float zCoord) : Biome(xCoord, zCoord)
{
	this->colour[0] = Util::Config::convertSettingToFloat("colours", "jungle_red");
	this->colour[1] = Util::Config::convertSettingToFloat("colours", "jungle_green");
	this->colour[2] = Util::Config::convertSettingToFloat("colours", "jungle_blue");
	this->colour[3] = Util::Config::convertSettingToFloat("colours", "jungle_alpha");

	this->type = "Jungle";
}

JungleBiome::~JungleBiome()
{

}