#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h> 

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

	this->hasData = true;

	this->uploadToGpu();
}

Texture::~Texture()
{

}

void Texture::uploadToGpu()
{
	//Load the Texture into GPU mem:
	glGenTextures(1, &this->id);
	
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, this->width, this->height, 0, GL_RGBA, GL_FLOAT, this->pixels);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, this->width, this->height, GL_RGBA, GL_FLOAT, this->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	this->loaded = true;
}