#ifndef _HASHTYPES_HPP
#define _HASHTYPES_HPP

#include <string>
#include <unordered_map>

#include "util/StringLib.hpp"
#include "terrain/Tile.hpp"

namespace Util
{
	//Basic Types:
	typedef std::unordered_map<std::string, std::string, StringLib::Hasher, StringLib::KeyEquals> StringToStringHash;

	//Custom Types:
	typedef std::unordered_map<float*, Terrain::Tile*, Terrain::TileHasher, Terrain::TileKeyEquals> CoordsToTileHash;
}

#endif
