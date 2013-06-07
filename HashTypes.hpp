#ifndef _HASHTYPES_HPP
#define _HASHTYPES_HPP

#include <string>
#include <unordered_map>

#include "StringLib.hpp"

typedef std::unordered_map<std::string, std::string, StringLib::Hasher, StringLib::KeyEquals> StringToStringHash;

#endif