#ifndef _RESOURCES_TEXTURE_HPP
#define _RESOURCES_TEXTURE_HPP

#include <string>

namespace Resources
{
	class Texture
	{
	//Variables:
	public:

		unsigned int id;

		int width;
		int height;

		float* pixels;

	protected:
	private:

		bool hasData;
		bool loaded;

	//Functions:
	public:

		Texture(const std::string& fileName);
		~Texture(void);

		void uploadToGpu(void);

	protected:
	private:
	};
}

#endif
