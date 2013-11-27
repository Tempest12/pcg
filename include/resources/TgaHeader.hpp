#ifndef _RESOURCES_TGAHEADER_HPP
#define _RESOURCES_TGAHEADER_HPP

#include <fstream>

namespace Resources
{
	class TgaHeader
	{
	//Variables:
	public:

		char identSize;
		char colorMapType;
		char imageType;

		short colorMapStart;
		short colorMapSize;
		char colorMapBits;

		short xStart;
		short yStart;
		short width;
		short height;

		char pixelSize;
		char descriptor;
		
	protected:
	private:

	//Methods:
	public:

		TgaHeader(void);
		~TgaHeader(void);

		void load(std::ifstream* file);

	protected:
	private:
	};
}

#endif