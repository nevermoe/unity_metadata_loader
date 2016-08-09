#pragma once

#include <string>
#include "File.h"

namespace il2cpp
{
namespace os
{

class MemoryMappedFile
{
public:
	static void* Map(FileHandle* file)
	{
		return Map(file, 0, 0);
	}

	static void Unmap(void* address)
	{
		Unmap(address, 0);
	}

	static void* Map(FileHandle* file, size_t length, size_t offset);
	static void Unmap(void* address, size_t length);
};

}
}