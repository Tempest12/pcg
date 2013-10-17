#ifndef _RESOURCE_JSONOBJECT_HPP
#define _RESOURCE_JSONOBJECT_HPP

#include <string>
#include <unordered_map>

#include "JSONArray.hpp"
#include "HashTypes.hpp"

namespace Resource
{
	class JSONObject
	{
	//Properies:
	public:
	protected:
	private:
		std::vector<JSONArray> arrays;
		std::vector<JSONObject> children;
		StringToStringHash members;

	//Functions:
	public:
		JSONObject();
		JSONObject(std::string* json);
		JSONObject(std::string& json);
		~JSONObject();

		JSONArray* getArray(std::string* name);

		JSONObject* getChildObject(std::string* name);

		StringToStringHash* getMembers(void);

		std::string& getValue(std::string* name);
		std::string& getValue(std::string& name);
		bool getValueAsBool(std::string* name);
		bool getValueAsBool(std::string& name);
		char getValueAsChar(std::string* name);
		char getValueAsChar(std::string& name);
		double getValueAsDouble(std::string* name);
		double getValueAsDouble(std::string& name);
		float getValueAsFloat(std::string* name);
		float getValueAsFloat(std::string& name);
		int getValueAsInt(std::string* name);
		int getValueAsInt(std::string& name);

		void parseString(std::string* json);
		void parseString(std::string& json);

	protected:
	private:
	};
}


#endif
