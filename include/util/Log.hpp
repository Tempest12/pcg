#ifndef _UTIL_LOG_H
#define _UTIL_LOG_H

#include <string>

enum LogLevel
{
	DEBUG,
	INFO,
	ERROR,
	FATAL,
	SPECIAL
};

namespace Util
{
	class Log
	{
	//Variables:
	public:
	protected:
	private:

	//Functions
	public:
		static bool init(bool append, int log_level);
		static void uninit(void);

		static void setLevel(LogLevel level);

		static void writeDebug(const std::string& message);
		static void writeInfo(const std::string& message);
		static void writeError(const std::string& message);
		static void writeFatal(const std::string& message);
		static void writeSpecial(const std::string& message);

	protected:
	private:
		static void writeMessage(const std::string& prefix, const std::string& message);
	};
}

#endif
