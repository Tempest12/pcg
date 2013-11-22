#include "terrain/SnowBiome.hpp"
#include "util/Config.hpp"

using namespace Terrain;

SnowBiome::SnowBiome(float xCoord, float zCoord) : Biome(xCoord, zCoord)
{
	this->colour[0] = Util::Config::convertSettingToFloat("colours", "snow_red");
	this->colour[1] = Util::Config::convertSettingToFloat("colours", "snow_green");
	this->colour[2] = Util::Config::convertSettingToFloat("colours", "snow_blue");
	this->colour[3] = Util::Config::convertSettingToFloat("colours", "snow_alpha");

	this->type = "Snow";
}

SnowBiome::~SnowBiome()
{

}