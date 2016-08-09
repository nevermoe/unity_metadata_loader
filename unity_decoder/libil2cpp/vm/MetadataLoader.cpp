#include "il2cpp-config.h"
#include "MetadataLoader.h"
#include "os/File.h"
#include "os/MemoryMappedFile.h"
#include "os/Mutex.h"
#include "utils/PathUtils.h"
#include "vm/Runtime.h"

using namespace il2cpp::os;
using namespace il2cpp::vm;

void* MetadataLoader::LoadMetadataFile (const char* fileName)
{
	std::string resourcesDirectory = utils::PathUtils::Combine (Runtime::GetDataDir (), "Metadata");

	std::string resourceFilePath = utils::PathUtils::Combine (resourcesDirectory, fileName);

	int error = 0;
	FileHandle* handle = File::Open (resourceFilePath, File::kFileModeOpen, File::kFileAccessRead, File::kFileShareRead, File::kFileOptionsNone, &error);
	if (error != 0)
		return NULL;

	void* fileBuffer = MemoryMappedFile::Map (handle);

	File::Close (handle, &error);
	if (error != 0)
	{
		MemoryMappedFile::Unmap (fileBuffer);
		fileBuffer = NULL;
		return NULL;
	}

	return fileBuffer;
}
