#pragma once

#include <stdint.h>
#include <string>
#include "os/ErrorCodes.h"

namespace il2cpp
{
namespace os
{

struct FileHandle;

struct FileStat
{
	std::string name;
	int32_t attributes;
	int64_t length;
	int64_t creation_time;
	int64_t last_access_time;
	int64_t last_write_time;
};

class LIBIL2CPP_CODEGEN_API File
{
public:

	enum FileType
	{
		kFileTypeUnknown = 0x0000,
		kFileTypeDisk = 0x0001,
		kFileTypeChar = 0x0002,
		kFileTypePipe = 0x0003,
		kFileTypeRemote = 0x8000
	};

	enum FileAttributes
	{
		kFileAttributeReadOnly = 0x00000001,
		kFileAttributeHidden = 0x00000002,
		kFileAttributeSystem = 0x00000004,
		kFileAttributeDirectory = 0x00000010,
		kFileAttributeArchive = 0x00000020,
		kFileAttributeDevice = 0x00000040,
		kFileAttributeNormal = 0x00000080,
		kFileAttributeTemporary = 0x00000100,
		kFileAttributeSparse_file = 0x00000200,
		kFileAttributeReparse_point = 0x00000400,
		kFileAttributeCompressed = 0x00000800,
		kFileAttributeOffline = 0x00001000,
		kFileAttributeNot_content_indexed = 0x00002000,
		kFileAttributeEncrypted = 0x00004000,
		kFileAttributeVirtual = 0x00010000,
		kFileAttributeInternalMonoExecutable = 0x80000000 // Only used internally by Mono
	};

	enum FileAccess
	{
		kFileAccessRead = 0x01,
		kFileAccessWrite = 0x02,
		kFileAccessReadWrite = kFileAccessRead|kFileAccessWrite
	};

	enum FileMode
	{
		kFileModeCreateNew = 1,
		kFileModeCreate = 2,
		kFileModeOpen = 3,
		kFileModeOpenOrCreate = 4,
		kFileModeTruncate = 5,
		kFileModeAppend = 6
	};

	enum FileShare
	{
		kFileShareNone = 0x0,
		kFileShareRead = 0x01,
		kFileShareWrite = 0x02,
		kFileShareReadWrite = kFileShareRead|kFileShareWrite,
		kFileShareDelete = 0x04
	};

	enum FileOptions
	{
		kFileOptionsNone = 0,
		kFileOptionsTemporary = 1,		// Internal.   See note in System.IO.FileOptions
		kFileOptionsEncrypted = 0x4000,
		kFileOptionsDeleteOnClose = 0x4000000,
		kFileOptionsSequentialScan = 0x8000000,
		kFileOptionsRandomAccess = 0x10000000,
		kFileOptionsAsynchronous = 0x40000000,
		kFileOptionsWriteThrough = 0x80000000
	};

	enum SeekOrigin
	{
		kFileSeekOriginBegin = 0,
		kFileSeekOriginCurrent = 1,
		kFileSeekOriginEnd = 2
	};

	static bool Isatty(FileHandle* fileHandle);
	static FileHandle* GetStdInput ();
	static FileHandle* GetStdOutput ();
	static FileHandle* GetStdError ();
	static bool CreatePipe (FileHandle** read_handle, FileHandle** write_handle);
	static FileType GetFileType (FileHandle* handle);
	static FileAttributes GetFileAttributes (const std::string& path, int* error);
	static bool SetFileAttributes (const std::string& path, FileAttributes attributes, int* error);
	static bool GetFileStat (const std::string& path, FileStat * stat, int* error);
	static bool CopyFile (const std::string& src, const std::string& dest, bool overwrite, int* error);
	static bool MoveFile (const std::string& src, const std::string& dest, int* error);
	static bool DeleteFile (const std::string& path, int *error);
	static bool ReplaceFile (const std::string& sourceFileName, const std::string& destinationFileName, const std::string& destinationBackupFileName, bool ignoreMetadataErrors, int* error);
	static FileHandle* Open (const std::string& path, int openMode, int accessMode, int shareMode, int options, int *error);
	static bool Close (FileHandle* handle, int *error);
	static bool SetFileTime (FileHandle* handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, int* error);
	static int64_t GetLength (FileHandle* handle, int *error);
	static bool SetLength (FileHandle* handle, int64_t length, int *error);
	static int64_t Seek (FileHandle* handle, int64_t offset, int origin, int *error);
	static int Read (FileHandle* handle, char *dest, int count, int *error);
	static int32_t Write (FileHandle* handle, const char* buffer, int count, int *error);
	static bool Flush (FileHandle* handle, int* error);
	static void Lock (FileHandle* handle,  int64_t position, int64_t length, int* error);
	static void Unlock (FileHandle* handle,  int64_t position, int64_t length, int* error);

};

}
}
