#include "il2cpp-config.h"
#include "utils/PathUtils.h"
#include <string>

namespace il2cpp
{
namespace utils
{

namespace PathUtils
{

std::string Basename (const std::string& path)
{
	if(path.empty())
		return ".";

	const size_t pos = path.rfind(IL2CPP_DIR_SEPARATOR);

	// No seperators. Path is filename
	if(pos == std::string::npos)
		return path;

	return path.substr(pos+1);
}

std::string DirectoryName (const std::string& path)
{
	if(path.empty())
		return std::string();

	const size_t pos = path.rfind(IL2CPP_DIR_SEPARATOR);

	if(pos == std::string::npos)
		return ".";

	if(pos == 0)
		return "/";

	return path.substr(0, pos);
}


std::string Combine (const std::string& path1, const std::string& path2)
{
	return path1 + IL2CPP_DIR_SEPARATOR + path2;
}

}

} /* utils */
} /* il2cpp */
