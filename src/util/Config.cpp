#include <fstream>
#include <iostream>
#include <string>

#include "util/Config.hpp"
#include "util/StringLib.hpp"
#include "main/Main.hpp"
#include "util/Trikey.hpp"

using namespace Util;

static Trikey data;
static bool initialized = false;

void Config::init(void)
{
	data = Trikey();

	int lineNumber = 0;
	std::string line;
	std::ifstream file;

	file.open("./data/config.txt", std::ifstream::in);

	if(!(file.is_open() && file.good()))
	{
		Main::die("Couldn't open config file.");
	}

	int count = 0;

	while(file.good())
	{
		Config::getNewLine(file, line, lineNumber);

		if(StringLib::isWhiteSpace(line) || line[0] == '#' || (line[0] == '/' && line[1] == '/'))
		{
			continue;
		}	
		else
		{
			count = 0;
			std::string* pieces = StringLib::split(line, ' ', count);
			
			if(count == 2 && StringLib::equalsIgnoreCase(pieces[0], "section"))
			{
				data.newSection(pieces[1]);
				Config::parseSection(file, pieces[1], lineNumber);
			}
			else
			{
				if(count == 1 && StringLib::equalsIgnoreCase(pieces[0], "section"))
				{
					file.close();
					Main::die("Sections must be named.");
				}
				else
				{
					file.close();
					Main::die("Something wrong with a line in the config.");
				}
			}

			delete[] pieces;
		}
	}

	file.close();

	initialized = true;
}

bool Config::convertSettingToBool(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);
	
	if(StringLib::equalsIgnoreCase(value, "true") || StringLib::equalsIgnoreCase(value, "yes"))
	{
		return true;
	}
	else if(StringLib::equalsIgnoreCase(value, "false") || StringLib::equalsIgnoreCase(value, "no"))
	{
		return false;
	}
	else
	{
		//TODO
		//Report Error
		return false;
	}
}

char Config::convertSettingToChar(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);

	if(value.length() > 0)
	{
		return value[0];
	}
	else
	{	
		//TODO:
		//Report Errors
		return ' ';
	}
}

double Config::convertSettingToDouble(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);

	if(value.length() > 0)
	{
		return std::stod(value);
	}
	else
	{
		//TODO:
		//Report Error
		return 0.0;;
	}
}

float Config::convertSettingToFloat(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);
	float temp = 0.0f;

	if(value.length() > 0)
	{
		return std::stof(value);
	}
	else
	{
		//TODO:
		//Report Error
		return temp;
	}
}

int Config::convertSettingToInt(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);

	if(value.length() > 0)
	{
		return std::stoi(value);
	}
	else
	{
		//TODO:
		//Report Error
		return 0;
	}
}


long Config::convertSettingToLong(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);

	if(value.length() > 0)
	{
		return std::stol(value);
	}
	else
	{
		//TODO:
		//Report Error
		return 0;
	}
}

short Config::convertSettingToShort(const std::string& sectionName, const std::string& settingName)
{
	std::string& value = data.get(sectionName, settingName);

	if(value.length() > 0)
	{	
		return (short)std::stoi(value);
	}
	else
	{
		//TODO:
		//Report Error
		return 0;
	}
}

std::string& Config::convertSettingToString(const std::string& sectionName, const std::string& settingName)
{
	return data.get(sectionName, settingName);
}

std::string* Config::convertSettingToStringPointer(const std::string& sectionName, const std::string& settingName)
{	
	return &data.get(sectionName, settingName);
}

void Config::getNewLine(std::ifstream& file, std::string& line, int& lineNumber)
{
	std::getline(file, line);
	StringLib::trim(line);
	lineNumber++;
}

void Config::parseSection(std::ifstream& file, std::string& sectionName, int& lineNumber)
{
	std::string line;
	int count;	

	Config::getNewLine(file, line, lineNumber);

	if(line[0] == '{')
	{
		Config::getNewLine(file, line, lineNumber);
		while(file.good() && line[0] != '}')
		{
			if(StringLib::isWhiteSpace(line) || line[0] == '#' || (line[0] == '/' && line[1] == '/'))
			{
				Config::getNewLine(file, line, lineNumber);
				continue;
			}
			
			count = 0;
			std::string* pieces = StringLib::split(line, ' ', count);
		
			if(count == 2)
			{
				data.put(sectionName, pieces[0], pieces[1]);
				Config::getNewLine(file, line, lineNumber);
			}
			else
			{
				file.close();
				Main::die("ERROR Config.parseSection: Too many things on a option line.");
			}
		}
	}
	else
	{
		file.close();
		std::cout << lineNumber << ": " << line << std::endl;
		Main::die("Section headers must be followed by opening curly bracket");
	}
}

void Config::printSections(void)
{
	data.printData();
}

void Config::uninit(void)
{
}