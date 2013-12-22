#include <fstream>
#include <iostream>

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
		file->read((char*)&this->identSize, sizeof(this->identSize));
		file->read((char*)&this->colorMapType, sizeof(this->colorMapType));
		file->read((char*)&this->imageType, sizeof(this->imageType));

		file->read((char*)&this->colorMapStart, sizeof(this->colorMapStart));
		file->read((char*)&this->colorMapSize, sizeof(this->colorMapSize));
		file->read((char*)&this->colorMapBits, sizeof(this->colorMapBits));

		file->read((char*)&this->xStart, sizeof(this->xStart));
		file->read((char*)&this->yStart, sizeof(this->yStart));
		file->read((char*)&this->width, sizeof(this->width));
		file->read((char*)&this->height, sizeof(this->height));

		file->read((char*)&this->pixelSize, sizeof(this->pixelSize));
		file->read((char*)&this->descriptor, sizeof(this->descriptor));
	}
}