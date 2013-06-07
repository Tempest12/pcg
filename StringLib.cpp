//STL Imports
#include <string>
#include <iostream>

//C LIB Imports
#include <ctype.h>
#include <math.h>

//Project Imports
#include "StringLib.hpp"

//Author: Shane del Solar
//version: 2.0

/**
 * Includes some useful string operations that aren't in the STL library.
 *		Includes:
 *          contains
 *			equalsIgnoreCase
 *          isWhiteSpace
 *			split
 *          trim
 *
 *		Has classes for:
 * 			hash
 *			keyEquals
 */

/************************************************************************/
/* Variables:                                                           */
/************************************************************************/
std::string StringLib::emptyString = "";


/************************************************************************/
/* Functions:                                                           */
/************************************************************************/

/**
 *Checks to see if the base string contains the target string.
 *Params
 *	string base -> Base String to look in.
 *	string target -> Target String to check for.
 *Return
 *  bool -> is Target string in base string
 */
bool StringLib::contains(std::string& base, std::string& target)
{
	return base.find(target) != std::string::npos;
}

bool StringLib::contains(std::string& base, const std::string& target)
{
	return base.find(target) != std::string::npos;
}

bool StringLib::contains(std::string* base, std::string* target)
{
	return base->find(*target) != std::string::npos;
}

bool StringLib::contains(std::string* base, const std::string& target)
{
	return base->find(target) != std::string::npos;
}

/**
 *Splits a string by a given delimiter and returns a new array of strings.
 *Params
 *	string* whole -> The String to Split.
 *  char delimiter -> The delimiter to split the string with
 *  int* numOf -> Pointer where the count will be stored.
 *Return
 *  string -> An array of split string without the delimiter character.
 *  *int -> the int pointer will be set to the length of the array 
 */

std::string* StringLib::split(std::string* whole, char delimiter, int* numOf)
{
	std::string* array = NULL;
	int length = 0;
	int count = 0;
	bool countIndex = true;

	*numOf = 0;

	if(whole == NULL)
	{
		return /*NULL*/array;
	}

	//Get the number characters we care about. Should be all of the except if last character is a delimiter.
	length = whole->length();

	if(length == 0)
	{
		array = new std::string[1];	
		*numOf = 1;
		return array;	
	}

	for(int index = 0; index  < length; index++)
	{
		//Count the number of string pieces that we will have to make. Ignoring ones that would be empty.(Delimiter followed a delimiter)
		if(whole[0][index] == delimiter)
		{
			countIndex = true;
		}
		else 
		{
			if(countIndex)
			{
				countIndex = false;
				count++;
			}
		}
	}

	//The delimiter was not found at all or found only at the ends of string. Return the given input.
	if(count == 0)
	{
		*numOf = 1;
		array = new std::string[1];
		array->assign(*whole);
		return array;
	}

	countIndex = false;
	*numOf = count;
	array = new std::string[count];
	count = 0;

	//Parse through the string char by char
	for(int index = 0; index < length; index++)
	{
		if(whole[0][index] == delimiter)
		{
			countIndex = true;
		}
		else
		{
			if(countIndex)
			{
				if(array[count].length() > 0)
				{
					count++;
				}
				countIndex = false;
			}
			array[count] += whole[0][index];
		}
	} 

	return array;	
}


std::string* StringLib::split(std::string& whole, char delimiter, int& numOf)
{
	std::string* array = NULL;
	int length = 0;
	int count = 0;
	bool countIndex = true;

	numOf = 0;

	//Get the number characters we care about. Should be all of the except if last character is a delimiter.
	length = whole.length();

	if(length == 0)
	{
		array = new std::string[1];	
		numOf = 1;
		return array;	
	}

	for(int index = 0; index  < length; index++)
	{
		//Count the number of string pieces that we will have to make. Ignoring ones that would be empty.(Delimiter followed a delimiter)
		if(whole[index] == delimiter)
		{
			countIndex = true;
		}
		else 
		{
			if(countIndex)
			{
				countIndex = false;
				count++;
			}
		}
	}

	//The delimiter was not found at all or found only at the ends of string. Return the given input.
	if(count == 0)
	{
		numOf = 1;
		array = new std::string[1];
		array->assign(whole);
		return array;
	}

	countIndex = false;
	numOf = count;
	array = new std::string[count];
	count = 0;

	//Parse through the string char by char
	for(int index = 0; index < length; index++)
	{
		if(whole[index] == delimiter)
		{
			countIndex = true;
		}
		else
		{
			if(countIndex)
			{
				if(array[count].length() > 0)
				{
					count++;
				}
				countIndex = false;
			}
			array[count] += whole[index];
		}
	} 

	return array;	
}

/**
 *Checks to see if the given string is only white space characters.
 *Params:
 *	string* line -> String to look at.
 *Return:
 *	bool -> True - all whiteSpace, False - has non-white space character.
 */
bool StringLib::isWhiteSpace(std::string* line)
{
	//Iterate through the string and check to see if each character is a white space Character. Return False if we find something that isn't. 
	for(unsigned int index = 0; index < line->length(); index++)
	{
		if(isspace(line[0][index]))
		{	
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool StringLib::isWhiteSpace(std::string& line)
{
	//Iterate through the string and check to see if each character is a white space Character. Return False if we find something that isn't. 
	for(unsigned int index = 0; index < line.length(); index++)
	{
		if(isspace(line[index]))
		{	
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}
/**
 *Check to see if two strings are equal to each other ignoring differences in case
 *Params:
 *	string* one -> The first string 
 *	string* two -> The second string
 *Return
 *	bool -> Equality of the two Strings
 */
bool StringLib::equalsIgnoreCase(std::string& one, std::string& two)
{
	unsigned int lengthOne = one.length();
	unsigned int lengthTwo = two.length();

	//If the length's aren't the same then obviously the strings are different.
	if(lengthOne != lengthTwo)
	{
		return false;
	}

	for(unsigned int index = 0; index < lengthOne; index++)
	{
		//If Both letters then compare the lower case version of the letters
		if(isalpha(one[index]) & isalpha(two[index]))
		{
			  if(tolower(one[index]) != tolower(two[index]))
			  {
			  	return false;
			  }
		}
		else
		{
		  //If not letters then chars must be equal
		  if(one[index] != two[index])
		  {
			   return false;
		  }
		}    
	}

	//All chars the same
	return true; 
}

bool StringLib::equalsIgnoreCase(std::string& one, const std::string& two)
{
	unsigned int lengthOne = one.length();
	unsigned int lengthTwo = two.length();

	//If the length's aren't the same then obviously the strings are different.
	if(lengthOne != lengthTwo)
	{
		return false;
	}

	for(unsigned int index = 0; index < lengthOne; index++)
	{
		//If Both letters then compare the lower case version of the letters
		if(isalpha(one[index]) & isalpha(two[index]))
		{
			  if(tolower(one[index]) != tolower(two[index]))
			  {
			  	return false;
			  }
		}
		else
		{
		  //If not letters then chars must be equal
		  if(one[index] != two[index])
		  {
			   return false;
		  }
		}    
	}

	//All chars the same
	return true; 
}

bool StringLib::equalsIgnoreCase(std::string* one, std::string* two)
{
	unsigned int lengthOne = one->length();
	unsigned int lengthTwo = two->length();

	//If the length's aren't the same then obviously the strings are different.
	if(lengthOne != lengthTwo)
	{
		return false;
	}

	for(unsigned int index = 0; index < lengthOne; index++)
	{
		//If Both letters then compare the lower case version of the letters
		if(isalpha(one->at(index) && isalpha(two->at(index))))
		{
			  if(tolower(one->at(index)) != tolower(two->at(index)))
			  {
			  	return false;
			  }
		}
		else
		{
		  //If not letters then chars must be equal
		  if(one->at(index) != two->at(index))
		  {
			   return false;
		  }
		}    
	}

	//All chars the same
	return true; 
}

bool StringLib::equalsIgnoreCase(std::string* one, const std::string& two)
{
	unsigned int lengthOne = one->length();
	unsigned int lengthTwo = two.length();

	//If the length's aren't the same then obviously the strings are different.
	if(lengthOne != lengthTwo)
	{
		return false;
	}

	for(unsigned int index = 0; index < lengthOne; index++)
	{
		//If Both letters then compare the lower case version of the letters
		if(isalpha(one->at(index)) & isalpha(two[index]))
		{
			  if(tolower(one->at(index)) != tolower(two[index]))
			  {
			  	return false;
			  }
		}
		else
		{
		  //If not letters then chars must be equal
		  if(one->at(index) != two[index])
		  {
			   return false;
		  }
		}    
	}

	//All chars the same
	return true; 
}

/**
 * Trim removes white space characters from the beginning and end of a string
 *Params:
 *	string& str -> The string to be trimmed
 *Return
 * void
 */
void StringLib::trim(std::string& str)
{
	size_t start = 0;
	size_t length = 0;

	if(str.length() == 0)
	{
		return;
	}

	for(int index = 0; index < (int)str.length(); index++)
	{
		if(isspace(str[index]))
		{
			start++;
		}
		else
		{
			break;
		}
	}

	for(int index = start; index < (int)str.length(); index++)
	{
		if(!isspace(str[index]))
		{
			length = index - start + 1;
		}
	}

	str.assign(str.substr(start, length));
}

/**
 *Hashes the given string reference using the Java string hash implementation.
 *Params:
 *	string& key -> The string to be hashed
 *Return
 *	size_t -> Hashcode of the given string.
 */
size_t StringLib::Hasher::operator()(const std::string& key) const
{
	size_t hashCode = 0;
	int length = key.length();

	for(int index = 0; index < length; index++)
	{
		//We are going to use the Java String Hash function.
		hashCode += key[index] * pow(31, length - index);
	}

	return hashCode;
}

/**
 * Checks the equality of the given strings. Wrapper to be used in a Hashmap
 *Params:
 *	string& one -> The first string. 
 *  string& two -> The second strng.
 *Return
 * bool -> Whether the strings are equal.
 */
bool StringLib::KeyEquals::operator()(const std::string& one, const std::string& two) const
{
	return (one == two);
}
