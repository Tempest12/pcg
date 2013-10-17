#ifndef _PORT_FILE_SYSTEM_HPP
#define _PORT_FILE_SYSTEM_HPP
namespace Port
{
	namespace FileSystem
	{
		char getSeparator()
		{
		#ifdef WINDOWS
			
			return '\\';

		#else

			return '/';
			
		#endif
		}
	}
}

#endif
