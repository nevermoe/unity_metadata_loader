#pragma once

#include <string>

namespace il2cpp
{
namespace utils
{

namespace PathUtils
{
	std::string Basename (const std::string& path);
	std::string DirectoryName (const std::string& path);
	std::string Combine (const std::string& path1, const std::string& path2);
}

} /* utils */
} /* il2cpp */
