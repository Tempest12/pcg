#ifndef _RESOURCES_TGALOADER
#define _RESOURCES_TGALOADER

namespace Resources
{
	class Texture;

	class TgaLoader
	{
	//Static Functions:
	public:

		static void loadFromFile(const std::string& fileName, Texture* texture);

	protected:
	private:
	};
}

#endif