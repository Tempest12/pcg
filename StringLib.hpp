#ifndef _STRINGLIB_H
#define _STRINGLIB_H

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <string>
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

namespace StringLib
{
	//Variables
	extern std::string emptyString;

	//Functions
	bool contains(std::string& base, std::string& target);
	bool contains(std::string& base, const std::string& target);
	bool contains(std::string* base, std::string* target);	
	bool contains(std::string* base, const std::string& target);

	bool equalsIgnoreCase(std::string& one, std::string& two);
	bool equalsIgnoreCase(std::string& one, const std::string& two);	
	bool equalsIgnoreCase(std::string* one, std::string* two);
	bool equalsIgnoreCase(std::string* one, const std::string& two);	

	std::string* split(std::string* whole, char delimiter, int* numOf);
	std::string* split(std::string& whole, char delimiter, int& numOf);

	bool isWhiteSpace(std::string& line);
	bool isWhiteSpace(std::string* line);

	void trim(std::string& str);
	void trim(std::string* str);

	//Classes
	class Hasher
	{
		//Properties:
		public:
		protected:
		private:

		//Functions:
		public:
			size_t operator()(const std::string& key) const;

		protected:
		private:
	};

	class KeyEquals
	{
		//Properties:
		public:
		protected:
		private:

		//Functions:
		public:
			bool operator()(const std::string& one, const std::string& two) const;

		protected:
		private:

	};
}

#endif
