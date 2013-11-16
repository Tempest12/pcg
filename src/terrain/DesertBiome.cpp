#include "math/Vector3f.hpp"
#include "terrain/Biome.hpp"
#include "terrain/DesertBiome.hpp"
#include "util/Config.hpp"

//The namespace this code belongs to:
using namespace Terrain;

DesertBiome::DesertBiome(float xCoord, float zCoord) : Biome(xCoord, zCoord)
{
	this->height = Util::Config::convertSettingToFloat("generator", "desert_height");

	this->colour[0] = Util::Config::convertSettingToFloat("colours", "desert_red");
	this->colour[1] = Util::Config::convertSettingToFloat("colours", "desert_green");
	this->colour[2] = Util::Config::convertSettingToFloat("colours", "desert_blue");
	this->colour[3] = Util::Config::convertSettingToFloat("colours", "desert_alpha");

	this->type = "Desert";
}

DesertBiome::~DesertBiome()
{
}