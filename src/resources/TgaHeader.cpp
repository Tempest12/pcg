#include <fstream>

#include "resources/TgaHeader.hpp"

using namespace Resources;

TgaHeader::TgaHeader(void)
{
	this->identSize     = 0;
	this->colorMapType  = 0;
	this->imageType     = 0;

	this->colorMapStart = 0;
	this->colorMapSize  = 0;
	this->colorMapBits  = 0;

	this->xStart        = 0;
	this->yStart        = 0;
	this->width         = 0;
	this->height        = 0;

	this->pixelSize     = 0;
	this->descriptor    = 0;
}

TgaHeader::~TgaHeader(void)
{
	//Nothing to do here.
}

void TgaHeader::load(std::ifstream* file)
{
	if(file->good())
	{
		*file >> this->identSize;
		*file >> this->colorMapType;
		*file >> this->imageType;

		*file >> this->colorMapStart;
		*file >> this->colorMapSize;
		*file >> this->colorMapBits;

		*file >> this->xStart;
		*file >> this->yStart;
		*file >> this->width;
		*file >> this->height;

		*file >> this->pixelSize;
		*file >> this->descriptor;
	}
}