#include <iostream>
#include <stdexcept>

#include "util/Trikey.hpp"

using namespace Util;

Trikey::Trikey(void)
{
	this->data = HashTable();
}

Trikey::~Trikey(void)
{

}

void Trikey::newSection(const std::string& sectionName)
{
	SectionHashTable* newSection = new SectionHashTable();

	this->data.emplace(sectionName, newSection);
}

void Trikey::newSection(std::string& sectionName)
{
	SectionHashTable* newSection = new SectionHashTable();

	this->data.emplace(sectionName, newSection);
}

void Trikey::newSection(std::string* sectionName)
{
	SectionHashTable* newSection = new SectionHashTable();

	this->data.emplace(*sectionName, newSection);
}

void Trikey::put(const std::string& sectionName, const std::string& key, const std::string& value)
{
	HashTable::const_iterator hashPair = this->data.find(sectionName);
	
	if(hashPair == data.end())
	{
		//TODO Report Error
		std::cout << "Error in Trikey.put" << std::endl;
	}

	SectionHashTable* sectionTable = hashPair->second;

	sectionTable->emplace(key, value);
}

void Trikey::put(std::string& sectionName, std::string& key, std::string& value)
{
	HashTable::const_iterator hashPair = this->data.find(sectionName);
	
	if(hashPair == data.end())
	{
		//TODO Report Error
		std::cout << "Error in Trikey.put" << std::endl;
	}

	SectionHashTable* sectionTable = hashPair->second;

	sectionTable->emplace(key, value);
}

void Trikey::put(std::string* sectionName, std::string* key, std::string* value)
{
	HashTable::const_iterator hashPair = this->data.find(*sectionName);
	
	if(hashPair == data.end())
	{
		//TODO Report Error
		std::cout << "Error in Trikey.put" << std::endl;
	}

	SectionHashTable* sectionTable = hashPair->second;

	sectionTable->emplace(*key, *value);
}

std::string& Trikey::get(std::string* sectionName, std::string* key)
{
	SectionHashTable* section;
	
	try
	{
		section = this->data.at(*sectionName);
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Section does not exist: " << *sectionName << std::endl;
		return StringLib::emptyString;
	}

	try
	{
		std::string& temp = section->at(*key);
		return temp;
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Key does not exist: " << *key << " in Section: " << *sectionName << std::endl;
		return StringLib::emptyString;
	}

	return StringLib::emptyString;
}

std::string& Trikey::get(std::string& sectionName, std::string& key)
{
	SectionHashTable* section;
	
	try
	{
		section = this->data.at(sectionName);
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Section does not exist: " << sectionName << std::endl;
		return StringLib::emptyString;
	}

	try
	{
		std::string& temp = section->at(key);
		return temp;
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Key does not exist: " << key << " in Section: " << sectionName << std::endl;
		return StringLib::emptyString;
	}

	return StringLib::emptyString;
}

std::string& Trikey::get(const std::string& sectionName, const std::string& key)
{
	SectionHashTable* section;
	
	try
	{
		section = this->data.at(sectionName);
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Section does not exist: " << sectionName << std::endl;
		return StringLib::emptyString;
	}

	try
	{
		std::string& temp = section->at(key);
		return temp;
	}
	catch(std::out_of_range& oore)
	{
		std::cout << "Key does not exist: " << key << " in Section: " << sectionName << std::endl;
		return StringLib::emptyString;
	}

	return StringLib::emptyString;
}

void Trikey::printData(void)
{
	std::cout << "Trikey is holding: \n" << std::endl;	
	for(HashTable::const_iterator sections =  data.begin(); sections != data.end(); sections++)
	{
		std::cout << "Section: " << sections->first << std::endl;
		std::cout << '{' << std::endl;

		for(SectionHashTable::const_iterator options = sections->second->begin(); options != sections->second->end(); options++)
		{
			std::cout << options->first	<< ':' << options->second << std::endl;
		}

		std::cout << '}' << std::endl;
		std::cout << std::endl;
	}
}
