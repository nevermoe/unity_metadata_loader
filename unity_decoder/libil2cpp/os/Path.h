#pragma once
#include <string>
#include <stdint.h>

namespace il2cpp
{
namespace os
{

class Path 
{
public:
	static std::string GetExecutablePath();
	static std::string GetTempPath();
};

}
}
