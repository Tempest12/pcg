#ifndef _UTIL_HASHTYPES_HPP
#define _UTIL_HASHTYPES_HPP

#include <string>
#include <unordered_map>

//Forward Declarations:
namespace Terrain
{
	class Region;
	class RegionHasher;
	class RegoinKeyEquals;
	
	class Tile;
    class TileHasher;
    class TileKeyEquals;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Util
{
    //Forward Declarations:
    namespace StringLib
    {
        class Hasher;
        class KeyEquals;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Basic Types:
	typedef std::unordered_map<std::string, std::string, StringLib::Hasher, StringLib::KeyEquals> StringToStringHash;

	//Custom Types:
	typedef std::unordered_map<int*, Terrain::Tile*, Terrain::TileHasher, Terrain::TileKeyEquals> CoordsToTileHash;

	typedef std::unordered_map<int*, Terrain::Region*, Terrain::RegionHasher, Terrain::RegionKeyEquals> CoordsToRegionHash;
}

#endif
