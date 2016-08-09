#pragma once

#include "il2cpp-config.h"

#if IL2CPP_TARGET_POSIX

#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "os/File.h"

namespace il2cpp
{
namespace os
{

struct FileHandle
{
	int fd;
	File::FileType type;
	std::string path;
	int options;
	int shareMode;
	int accessMode;

	// device and inode are used as key for finding file handles
	dev_t device;
	ino_t inode;

	// Linked list of file handles
	FileHandle *prev;
	FileHandle *next;

	FileHandle() : prev(NULL), next(NULL)
	{
	}
};

}
}

#endif
