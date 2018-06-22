#pragma once

#include <stdint.h>
#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppArray;
struct Il2CppString;

typedef int32_t MonoIOError;
typedef int32_t FileAttributes;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace IO
{

struct FileStat
{
	Il2CppString *name;
	int32_t attributes;
	int64_t length;
	int64_t creation_time;
	int64_t last_access_time;
	int64_t last_write_time;
};

class LIBIL2CPP_CODEGEN_API MonoIO
{
public:
	static bool Close (Il2CppIntPtr handle,int *error);
	static bool CopyFile (Il2CppString* src, Il2CppString* dest, bool overwrite, MonoIOError* error);
	static bool CreateDirectory (Il2CppString* path, MonoIOError* error);
	static bool CreatePipe (Il2CppIntPtr* read_handle, Il2CppIntPtr* write_handle);
	static bool DeleteFile (Il2CppString* path, MonoIOError* error);
	static bool DuplicateHandle (Il2CppIntPtr source_process_handle, Il2CppIntPtr source_handle, Il2CppIntPtr target_process_handle, Il2CppIntPtr* target_handle, int32_t access, int32_t inherit, int32_t options);
	static bool Flush (Il2CppIntPtr handle, MonoIOError* error);
	static Il2CppString* GetCurrentDirectory (MonoIOError* error);
	static FileAttributes GetFileAttributes (Il2CppString* path, MonoIOError* error);
	static bool GetFileStat (Il2CppString* path, FileStat * stat, int32_t* error);
	static Il2CppArray* GetFileSystemEntries (Il2CppString* path, Il2CppString* path_with_pattern, int32_t attrs, int32_t mask, MonoIOError* error);
	static int GetFileType (Il2CppIntPtr handle,int *error);
	static int64_t GetLength (Il2CppIntPtr handle, int *error);
	static int32_t GetTempPath (Il2CppString** path);
	static void Lock (Il2CppIntPtr handle, int64_t position, int64_t length, MonoIOError* error);
	static bool MoveFile (Il2CppString* src, Il2CppString* dest, MonoIOError* error);
	static Il2CppIntPtr Open (Il2CppString *filename, int mode, int access_mode, int share, int options, int *error);
	static int Read (Il2CppIntPtr handle, Il2CppArray *dest, int dest_offset, int count, int *error);
	static bool RemoveDirectory (Il2CppString* path, MonoIOError* error);
	static bool ReplaceFile (Il2CppString* sourceFileName, Il2CppString* destinationFileName, Il2CppString* destinationBackupFileName, bool ignoreMetadataErrors, MonoIOError* error);
	static int64_t Seek (Il2CppIntPtr handle, int64_t offset, int origin, int *error);
	static bool SetCurrentDirectory (Il2CppString* path, int* error);
	static bool SetFileAttributes (Il2CppString* path, FileAttributes attrs, MonoIOError* error);
	static bool SetFileTime (Il2CppIntPtr handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, MonoIOError* error);
	static bool SetLength (Il2CppIntPtr handle, int64_t length, int *error);
	static void Unlock (Il2CppIntPtr handle, int64_t position, int64_t length, MonoIOError* error);
	static int Write (Il2CppIntPtr handle, Il2CppArray * src, int src_offset, int count, int * error);
	static Il2CppChar get_AltDirectorySeparatorChar (void);
	static Il2CppIntPtr get_ConsoleError (void);
	static Il2CppIntPtr get_ConsoleInput (void);
	static Il2CppIntPtr get_ConsoleOutput (void);
	static Il2CppChar get_DirectorySeparatorChar (void);
	static Il2CppChar get_PathSeparator (void);
	static Il2CppChar get_VolumeSeparatorChar (void);
};

} /* namespace IO */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
