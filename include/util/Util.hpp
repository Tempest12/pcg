#ifndef _UTIL_HPP
#define _UTIL_HPP

namespace Util
{
	class Port
	{

	//Variables:
	public:
	#ifdef WINDOWS
		static char fileSeparator = '\\';
	#else
		static char fileSeparator = '/';
	#endif

	protected:
	private:


	//Methods:
	public:
	protected:
	private:

	};
}

#endif
