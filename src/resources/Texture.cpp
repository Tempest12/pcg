#include <iostream>

#include "main/Main.hpp"
#include "resources/Texture.hpp"
#include "resources/TgaLoader.hpp"
#include "util/StringLib.hpp"

using namespace Resources;

Texture::Texture(const std::string& fileName)
{
	//Load the pxiels from given file:
	if(Util::StringLib::endsWith(fileName, ".tga"))
	{
		TgaLoader::loadFromFile(fileName, this);
	}
	else
	{
		Main::die("The following type of file cannot current be loaded as a texture: " + fileName);
	}
}

Texture::~Texture()
{

}