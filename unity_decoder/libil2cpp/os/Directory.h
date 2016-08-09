#pragma once

#include <stdint.h>
#include <string>
#include <set>

namespace il2cpp
{
namespace os
{

class Directory
{
public:
	static std::string GetCurrent (int* error);
	static bool SetCurrent (const std::string& path, int* error);
	static bool Create (const std::string& path, int *error);
	static bool Remove (const std::string& path, int *error);
	static std::set<std::string> GetFileSystemEntries (const std::string& path, const std::string& pathWithPattern, int32_t attrs, int32_t mask, int* error);
};

}
}
