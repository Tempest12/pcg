#ifndef _UTIL_CONFIG_H
#define _UTIL_CONFIG_H

#include <string>

namespace Util
{
	class Config
	{
		//Variables:
	public:
	protected:
	private:

		//Functions:
	public:
		static void init(void);
		static void uninit(void);

		static std::string& convertSettingToString(const std::string& sectionName, const std::string& settingName);
		static std::string* convertSettingToStringPointer(const std::string& sectionName, const std::string& settingName);	

		static char convertSettingToChar(const std::string& sectionName, const std::string& settingName);
		static bool convertSettingToBool(const std::string& sectionName, const std::string& settingName);
		static short convertSettingToShort(const std::string& sectionName, const std::string& settingName); 
		static int convertSettingToInt(const std::string& sectionName, const std::string& settingName);
		static long convertSettingToLong(const std::string& sectionName, const std::string& settingName);
		static float convertSettingToFloat(const std::string& sectionName, const std::string& settingName);
		static double convertSettingToDouble(const std::string& sectionName, const std::string& settingName);

		static void printSections(void);

	protected:
	private:
		static void getNewLine(std::ifstream& file, std::string& line, int& lineNumber);
		static void parseSection(std::ifstream& file, std::string& line, int& lineNumber);

		
	};
}

#endif
