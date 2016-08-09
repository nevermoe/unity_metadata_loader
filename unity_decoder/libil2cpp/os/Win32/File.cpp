#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "WindowsHelpers.h"

#undef CopyFile
#undef DeleteFile
#undef MoveFile
#undef ReplaceFile
#undef GetFileAttributes
#undef SetFileAttributes
#undef CreatePipe

#include "os/File.h"
#include "utils/StringUtils.h"
#include "utils/PathUtils.h"

#include <cassert>
#include <stdint.h>

static inline int Win32ErrorToErrorCode (DWORD win32ErrorCode)
{
	return win32ErrorCode;
}

namespace il2cpp
{
namespace os
{

bool File::Isatty(FileHandle* fileHandle)
{
	NOT_IMPLEMENTED_ICALL(File::IsAtty);
	return false;
}

FileHandle* File::GetStdInput ()
{
	return (FileHandle*)GetStdHandle (STD_INPUT_HANDLE);
}

#if !IL2CPP_TARGET_XBOXONE
FileHandle* File::GetStdError ()
{
	return (FileHandle*)GetStdHandle (STD_ERROR_HANDLE);
}

FileHandle* File::GetStdOutput ()
{
	return (FileHandle*)GetStdHandle (STD_OUTPUT_HANDLE);
}
#endif

bool File::CreatePipe (FileHandle** read_handle, FileHandle** write_handle)
{
	return false;
}

File::FileAttributes File::GetFileAttributes (const std::string& path, int *error)
{
	const UTF16String utf16Path (utils::StringUtils::Utf8ToUtf16 (path.c_str()));
	WIN32_FILE_ATTRIBUTE_DATA fileAttributes;

	BOOL result = ::GetFileAttributesExW ((LPCWSTR)utf16Path.c_str(), GetFileExInfoStandard, &fileAttributes);
	if (result == FALSE)
	{
		*error = Win32ErrorToErrorCode(::GetLastError());
		return static_cast<File::FileAttributes>(INVALID_FILE_ATTRIBUTES);
	}

	*error = kErrorCodeSuccess;
	return static_cast<FileAttributes>(fileAttributes.dwFileAttributes);
}

bool File::SetFileAttributes (const std::string& path, FileAttributes attributes, int* error)
{
	const UTF16String utf16Path (utils::StringUtils::Utf8ToUtf16 (path.c_str ()));

	*error = kErrorCodeSuccess;

	if (::SetFileAttributesW ((LPCWSTR)utf16Path.c_str (), attributes))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

static inline int64_t HighAndLowToInt64 (uint32_t high, uint32_t low)
{
	return ((uint64_t)high << 32) + low;
}

static inline int64_t FileTimeToInt64 (const FILETIME& fileTime)
{
	return HighAndLowToInt64 (fileTime.dwHighDateTime, fileTime.dwLowDateTime);
}

bool File::GetFileStat (const std::string& path, il2cpp::os::FileStat * stat, int* error)
{
	*error = kErrorCodeSuccess;
	const UTF16String utf16Path (utils::StringUtils::Utf8ToUtf16 (path.c_str ()));

	WIN32_FILE_ATTRIBUTE_DATA data;
	if (!::GetFileAttributesExW ((LPCWSTR)utf16Path.c_str (), GetFileExInfoStandard, &data))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return false;
	}

	stat->name = il2cpp::utils::PathUtils::Basename (path);
	stat->attributes = data.dwFileAttributes;
	stat->creation_time = FileTimeToInt64 (data.ftCreationTime);
	stat->last_access_time = FileTimeToInt64 (data.ftLastAccessTime);
	stat->last_write_time = FileTimeToInt64 (data.ftLastWriteTime);
	stat->length = HighAndLowToInt64 (data.nFileSizeHigh, data.nFileSizeLow);
	return true;
}

File::FileType File::GetFileType (FileHandle* handle)
{
	int result = ::GetFileType ((HANDLE)handle);
	/*if (result == FILE_TYPE_UNKNOWN)
	{
		*error = GetLastError();
	}*/
	return (File::FileType)result;
}

bool File::CopyFile (const std::string& src, const std::string& dest, bool overwrite, int* error)
{
	const UTF16String utf16Src (utils::StringUtils::Utf8ToUtf16 (src.c_str ()));
	const UTF16String utf16Dest (utils::StringUtils::Utf8ToUtf16 (dest.c_str ()));

	*error = kErrorCodeSuccess;

	if (::CopyFileW ((LPWSTR)utf16Src.c_str (), (LPWSTR)utf16Dest.c_str (), overwrite ? FALSE : TRUE))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool File::MoveFile (const std::string& src, const std::string& dest, int* error)
{
	const UTF16String utf16Src (utils::StringUtils::Utf8ToUtf16 (src.c_str ()));
	const UTF16String utf16Dest (utils::StringUtils::Utf8ToUtf16 (dest.c_str ()));

	*error = kErrorCodeSuccess;

	if (::MoveFileExW ((LPWSTR)utf16Src.c_str (), (LPWSTR)utf16Dest.c_str (), MOVEFILE_COPY_ALLOWED))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool File::DeleteFile (const std::string& path, int *error)
{
	*error = kErrorCodeSuccess;
	const UTF16String utf16Path (utils::StringUtils::Utf8ToUtf16 (path.c_str ()));
	if (::DeleteFileW ((LPWSTR)utf16Path.c_str ()))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool File::ReplaceFile (const std::string& sourceFileName, const std::string& destinationFileName, const std::string& destinationBackupFileName, bool ignoreMetadataErrors, int* error)
{
	const UTF16String utf16Src (utils::StringUtils::Utf8ToUtf16 (sourceFileName.c_str ()));
	const UTF16String utf16Dest (utils::StringUtils::Utf8ToUtf16 (destinationFileName.c_str ()));
	const UTF16String utf16Backup (utils::StringUtils::Utf8ToUtf16 (destinationBackupFileName.c_str ()));

	*error = kErrorCodeSuccess;

	DWORD flags = REPLACEFILE_WRITE_THROUGH;
	if (ignoreMetadataErrors)
		flags |= REPLACEFILE_IGNORE_MERGE_ERRORS;

	if (::ReplaceFileW ((LPWSTR)utf16Dest.c_str (), (LPWSTR)utf16Src.c_str (), utf16Backup.empty() ? NULL : (LPWSTR)utf16Backup.c_str (), flags, NULL, NULL))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

static inline int MonoToWindowsOpenMode(int monoOpenMode)
{
	switch (monoOpenMode)
	{
	case File::kFileModeCreateNew:
		return CREATE_NEW;

	case File::kFileModeCreate:
		return CREATE_ALWAYS;

	case File::kFileModeOpen:
		return OPEN_EXISTING;

	case File::kFileModeOpenOrCreate:
	case File::kFileModeAppend:
		return OPEN_ALWAYS;

	case File::kFileModeTruncate:
		return TRUNCATE_EXISTING;

	default:
		Assert(false && "Unknown mono open mode");
		IL2CPP_UNREACHABLE;
	}
}

static inline int MonoToWindowsAccessMode(int monoAccessMode)
{
	switch (monoAccessMode)
	{
	case File::kFileAccessRead:
		return GENERIC_READ;

	case File::kFileAccessWrite:
		return GENERIC_WRITE;

	case File::kFileAccessReadWrite:
		return GENERIC_READ | GENERIC_WRITE;

	default:
		return 0;
	}
}

static inline DWORD MonoOptionsToWindowsFlagsAndAttributes(const std::string& path, int options)
{
	DWORD flagsAndAttributes;

	if (options & File::kFileOptionsEncrypted)
	{
		flagsAndAttributes = FILE_ATTRIBUTE_ENCRYPTED;
	}
	else
	{
		flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	}

	// Temporary flag does not mean temporary file. 
	flagsAndAttributes |= options & ~(File::kFileOptionsEncrypted | File::kFileOptionsTemporary);

	int error;
	File::FileAttributes currentAttributes = File::GetFileAttributes(path, &error);

	if (currentAttributes != INVALID_FILE_ATTRIBUTES && (currentAttributes & FILE_ATTRIBUTE_DIRECTORY))
		flagsAndAttributes |= FILE_FLAG_BACKUP_SEMANTICS;	// Required to open a directory

	return flagsAndAttributes;
}

FileHandle* File::Open (const std::string& path, int openMode, int accessMode, int shareMode, int options, int *error)
{
	const UTF16String utf16Path (utils::StringUtils::Utf8ToUtf16 (path.c_str ()));

	openMode = MonoToWindowsOpenMode(openMode);
	accessMode = MonoToWindowsAccessMode(accessMode);
	DWORD flagsAndAttributes = MonoOptionsToWindowsFlagsAndAttributes(path, options);

	HANDLE handle = ::CreateFileW ((LPCWSTR)utf16Path.c_str (), accessMode, shareMode, NULL, openMode, flagsAndAttributes, NULL);
	
	if (INVALID_HANDLE_VALUE == handle)
	{
		*error = Win32ErrorToErrorCode(::GetLastError());
		return (FileHandle*)INVALID_HANDLE_VALUE;
	}

	*error = kErrorCodeSuccess;
	return (FileHandle*)handle;
}

bool File::Close (FileHandle* handle, int *error)
{
	*error = kErrorCodeSuccess;
	if (CloseHandle ((HANDLE)handle))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());
	return false;
}

bool File::SetFileTime (FileHandle* handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, int* error)
{
	FILE_BASIC_INFO fileInfo;

	fileInfo.CreationTime.QuadPart = creation_time;
	fileInfo.LastAccessTime.QuadPart = last_access_time;
	fileInfo.LastWriteTime.QuadPart = last_write_time;
	fileInfo.ChangeTime.QuadPart = 0; // 0 means don't change anything
	fileInfo.FileAttributes = 0; // 0 means don't change anything

	if (SetFileInformationByHandle(handle, FileBasicInfo, &fileInfo, sizeof(FILE_BASIC_INFO)) == FALSE)
	{
		*error = GetLastError();
		return false;
	}

	*error = kErrorCodeSuccess;
	return true;
}

int64_t File::GetLength (FileHandle* handle, int *error)
{
	*error = kErrorCodeSuccess;
	LARGE_INTEGER size;
	if (!::GetFileSizeEx ((HANDLE)handle, &size))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return 0;
	}
	return size.QuadPart;
}

bool File::SetLength (FileHandle* handle, int64_t length, int *error)
{
	*error = kErrorCodeSuccess;
	LARGE_INTEGER zeroOffset = { 0 };
	LARGE_INTEGER requestedOffset = { 0 };
	requestedOffset.QuadPart = length;
	LARGE_INTEGER initialPosition = { 0 };

	// set position to 0 from current to retrieve current position
	if (!::SetFilePointerEx ((HANDLE)handle, zeroOffset, &initialPosition, FILE_CURRENT))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return false;
	}

	// seek to requested length
	if (!::SetFilePointerEx ((HANDLE)handle, requestedOffset, NULL, FILE_BEGIN))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return false;
	}

	// set requested length
	if (!::SetEndOfFile ((HANDLE)handle))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return false;
	}

	// restore original position
	if (!::SetFilePointerEx ((HANDLE)handle, initialPosition, NULL, FILE_BEGIN))
	{
		*error = Win32ErrorToErrorCode (::GetLastError ());
		return false;
	}

	return true;
}

int64_t File::Seek (FileHandle* handle, int64_t offset, int origin, int *error)
{
	*error = kErrorCodeSuccess;
	LARGE_INTEGER distance;
	distance.QuadPart = offset;
	LARGE_INTEGER position = { 0 };
	if (!::SetFilePointerEx ((HANDLE)handle, distance, &position, origin))
		*error = Win32ErrorToErrorCode (::GetLastError ());

	return position.QuadPart;
}

int File::Read (FileHandle* handle, char *dest, int count, int *error)
{
	*error = kErrorCodeSuccess;
	DWORD bytesRead = 0;
	if (!::ReadFile (handle, dest, count, &bytesRead, NULL))
		*error = Win32ErrorToErrorCode (::GetLastError ());

	return bytesRead;
}

int32_t File::Write (FileHandle* handle, const char* buffer, int count, int *error)
{
	int32_t result;
	int32_t written;

	result = WriteFile ((HANDLE)handle, buffer, count, (LPDWORD)&written, NULL);

	/*if (!result)
	{
		*error = GetLastError ();
		return -1;
	}*/

	return written;
}

bool File::Flush (FileHandle* handle, int* error)
{
	*error = kErrorCodeSuccess;
	if (FlushFileBuffers ((HANDLE)handle))
		return true;

	*error = Win32ErrorToErrorCode (::GetLastError ());

	return false;
}

void File::Lock (FileHandle* handle,  int64_t position, int64_t length, int* error)
{
	*error = kErrorCodeSuccess;

	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(overlapped));

	overlapped.Offset = position & 0xFFFFFFFF;
	overlapped.OffsetHigh = position >> 32;

	LARGE_INTEGER lengthUnion;
	lengthUnion.QuadPart = length;

	if (!::LockFileEx((HANDLE)handle, LOCKFILE_FAIL_IMMEDIATELY, 0, lengthUnion.LowPart, lengthUnion.HighPart, &overlapped))
		*error = Win32ErrorToErrorCode (::GetLastError ());
}

void File::Unlock (FileHandle* handle,  int64_t position, int64_t length, int* error)
{
	*error = kErrorCodeSuccess;

	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(overlapped));

	overlapped.Offset = position & 0xFFFFFFFF;
	overlapped.OffsetHigh = position >> 32;

	LARGE_INTEGER lengthUnion;
	lengthUnion.QuadPart = length;

	if (!::UnlockFileEx((HANDLE)handle, 0, lengthUnion.LowPart, lengthUnion.HighPart, &overlapped))
		*error = Win32ErrorToErrorCode (::GetLastError ());
}

}
}

#endif
