#include <string>

#include "JSONObject.hpp"

JSONObject::JSONObject(void)
{
	this->arrays = std::vector<JSONArray>();
	this->children = std::vector<JSONObject>();
	this->members = StringToStringHash();	
}	

JSONObject::JSONObject(std::string* json) : JSONObject()
{
	this->parseString(json);
}

JSONObject::JSONObject(std::string& json) : JSONObject()
{
	this->parseString(json);
}

JSONObject::~JSONObject()
{
	this->members.clear();
}





JSONArray* JSONObject::getArray(std::string* name)
{
	if(name == NULL)
	{
		return NULL;//You asked for this...
	}

	for(int index = 0; index < this->arrays.size(); index++)
	{

	}

	return NULL;
}




JSONObject* JSONObject::getChildObject(std::string* name)
{
}





StringToStringHash* JSONObject::getMembers(void)
{
}





std::string& JSONObject::getValue(std::string* name)
{
}

std::string& JSONObject::getValue(std::string& name)
{
}

bool JSONObject::getValueAsBool(std::string* name)
{
}

bool JSONObject::getValueAsBool(std::string& name)
{
}

char JSONObject::getValueAsChar(std::string* name)
{
}

char JSONObject::getValueAsChar(std::string& name)
{
}

double JSONObject::getValueAsDouble(std::string* name)
{
}

double JSONObject::getValueAsDouble(std::string& name)
{
}

float JSONObject::getValueAsFloat(std::string* name)
{
}

float JSONObject::getValueAsFloat(std::string& name)
{
}

int JSONObject::getValueAsInt(std::string* name)
{
}

int JSONObject::getValueAsInt(std::string& name)
{
}




void JSONObject::parseString(std::string* json)
{
}
void JSONObject::parseString(std::string& json)
{
}



std::string JSONObject::ToString(void)
{

}
