#include "il2cpp-config.h"
#include <cassert>
#include "class-internals.h"
#include "icalls/mscorlib/System.IO/MonoIO.h"
#include "os/Directory.h"
#include "os/File.h"
#include "os/Path.h"
#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/String.h"
#include "vm/Exception.h"
#include "utils/StringUtils.h"

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

Il2CppChar MonoIO::get_AltDirectorySeparatorChar (void)
{
#if IL2CPP_COMPILER_MSVC
	return '/';	/* forward slash */
#else
	return '/';	/* slash, same as DirectorySeparatorChar */
#endif
}

bool MonoIO::Close (Il2CppIntPtr handle, int *error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;

	return il2cpp::os::File::Close (fileHandle, error);
}

bool MonoIO::CopyFile (Il2CppString *src, Il2CppString *dest, bool overwrite, int *error)
{
	const std::string srcString(il2cpp::utils::StringUtils::Utf16ToUtf8 (src->chars));
	const std::string destString(il2cpp::utils::StringUtils::Utf16ToUtf8 (dest->chars));

	return il2cpp::os::File::CopyFile (srcString, destString, overwrite, error);
}

Il2CppIntPtr MonoIO::get_ConsoleError (void)
{
	Il2CppIntPtr ret;
	ret.m_value = il2cpp::os::File::GetStdError ();
	return ret;
}

Il2CppIntPtr  MonoIO::get_ConsoleInput (void)
{
	Il2CppIntPtr ret;
	ret.m_value = il2cpp::os::File::GetStdInput ();
	return ret;
}

Il2CppIntPtr  MonoIO::get_ConsoleOutput (void)
{
	Il2CppIntPtr ret;
	ret.m_value = il2cpp::os::File::GetStdOutput ();
	return ret;
}

bool MonoIO::CreateDirectory (Il2CppString* path, int32_t* error)
{
	return il2cpp::os::Directory::Create (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), error);
}

bool MonoIO::DeleteFile (Il2CppString *path, int *error)
{
	return il2cpp::os::File::DeleteFile (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), error);
}

Il2CppChar MonoIO::get_DirectorySeparatorChar (void)
{
	return IL2CPP_DIR_SEPARATOR;
}

Il2CppString * MonoIO::GetCurrentDirectory (int *error)
{
	return vm::String::New (il2cpp::os::Directory::GetCurrent (error).c_str ());
}

int MonoIO::GetFileAttributes (Il2CppString* path, int* error)
{
	return il2cpp::os::File::GetFileAttributes (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), error);
}

bool MonoIO::GetFileStat (Il2CppString* path, FileStat * stat, int32_t* error)
{
	os::FileStat fileStat;

	const bool ret = il2cpp::os::File::GetFileStat (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), &fileStat, error);

	if(ret)
	{
		stat->name = vm::String::New(fileStat.name.c_str());
		stat->attributes = fileStat.attributes;
		stat->length = fileStat.length;
		stat->creation_time = fileStat.creation_time;
		stat->last_access_time = fileStat.last_access_time;
		stat->last_write_time = fileStat.last_write_time;
	}

	return ret;
}

int MonoIO::GetFileType (Il2CppIntPtr handle,int *error)
{
	il2cpp::os::FileHandle* h = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::GetFileType (h);
}

int64_t MonoIO::GetLength (Il2CppIntPtr handle, int *error)
{
	il2cpp::os::FileHandle* h = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::GetLength (h, error);
}

Il2CppIntPtr MonoIO::Open (Il2CppString *filename, int mode, int access_mode, int share, int options, int *error)
{
	Il2CppIntPtr ret;
	ret.m_value = il2cpp::os::File::Open (il2cpp::utils::StringUtils::Utf16ToUtf8 (filename->chars), mode, access_mode, share, options, error);
	return ret;
}

Il2CppChar MonoIO::get_PathSeparator (void)
{
#if IL2CPP_COMPILER_MSVC
	return ';';	/* semicolon */
#else
	return ':';	/* colon */
#endif
}

int MonoIO::Read (Il2CppIntPtr handle, Il2CppArray *dest, int dest_offset, int count, int *error)
{
	assert(dest != NULL);

	*error = 0; // ERROR_SUCCESS

	if(((uint32_t)dest_offset + count) > il2cpp::vm::Array::GetLength (dest))
		return 0;

	il2cpp::os::FileHandle* h = (il2cpp::os::FileHandle*)handle.m_value;

	char *buffer = il2cpp_array_addr(dest, char, dest_offset);

	return il2cpp::os::File::Read (h, buffer, count, error);
}

bool MonoIO::SetCurrentDirectory (Il2CppString* path, int* error)
{
	return il2cpp::os::Directory::SetCurrent (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), error);
}

bool MonoIO::SetLength (Il2CppIntPtr handle, int64_t length, int *error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::SetLength (fileHandle, length, error);
}

int64_t MonoIO::Seek (Il2CppIntPtr handle, int64_t offset, int origin, int *error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::Seek (fileHandle, offset, origin, error);
}

int MonoIO::Write (Il2CppIntPtr handle, Il2CppArray * src, int src_offset, int count, int * error)
{
	assert(src != NULL);

	*error = 0; // ERROR_SUCCESS

	if((uint32_t)(src_offset + count) > il2cpp::vm::Array::GetLength (src))
		return 0;

	il2cpp::os::FileHandle* h = (il2cpp::os::FileHandle*)handle.m_value;
	char *buffer = il2cpp_array_addr(src, char, src_offset);

	return il2cpp::os::File::Write (h, buffer, count, error);
}

Il2CppChar MonoIO::get_VolumeSeparatorChar (void)
{
#if IL2CPP_COMPILER_MSVC
	return ':';	/* colon */
#else
	return '/';	/* forward slash */
#endif
}

bool MonoIO::RemoveDirectory (Il2CppString* path, MonoIOError* error)
{
	return il2cpp::os::Directory::Remove (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), error);
}

Il2CppArray* MonoIO::GetFileSystemEntries (Il2CppString* path, Il2CppString* path_with_pattern, int32_t attrs, int32_t mask, MonoIOError* error)
{
	const std::string pathString(il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars));
	const std::string pathPatternString(il2cpp::utils::StringUtils::Utf16ToUtf8 (path_with_pattern->chars));

	const std::set<std::string> entries(il2cpp::os::Directory::GetFileSystemEntries (pathString, pathPatternString, attrs, mask, error));

	Il2CppClass *klass = il2cpp::vm::Class::GetArrayClass (il2cpp_defaults.string_class, 1);
	Il2CppArray* array = (Il2CppArray*)il2cpp::vm::Array::NewSpecific (klass, (il2cpp_array_size_t)entries.size());

	size_t index = 0;

	for(std::set<std::string>::const_iterator entry = entries.begin(), end = entries.end(); entry  != end; ++entry)
		il2cpp_array_setref (array, index++, il2cpp::vm::String::New(entry->c_str()));

	return array;
}

bool MonoIO::MoveFile (Il2CppString* src, Il2CppString* dest, MonoIOError* error)
{
	const std::string srcString(il2cpp::utils::StringUtils::Utf16ToUtf8 (src->chars));
	const std::string destString(il2cpp::utils::StringUtils::Utf16ToUtf8 (dest->chars));

	return il2cpp::os::File::MoveFile (srcString, destString, error);
}

bool MonoIO::ReplaceFile (Il2CppString* sourceFileName, Il2CppString* destinationFileName, Il2CppString* destinationBackupFileName, bool ignoreMetadataErrors, MonoIOError* error)
{
	const std::string srcString(il2cpp::utils::StringUtils::Utf16ToUtf8 (sourceFileName->chars));
	const std::string destString(il2cpp::utils::StringUtils::Utf16ToUtf8 (destinationFileName->chars));
	const std::string destBackupString(destinationBackupFileName ? il2cpp::utils::StringUtils::Utf16ToUtf8 (destinationBackupFileName->chars) : "");

	return il2cpp::os::File::ReplaceFile (srcString, destString, destBackupString, ignoreMetadataErrors, error);
}

bool MonoIO::SetFileAttributes (Il2CppString* path, FileAttributes attrs, MonoIOError* error)
{
	return il2cpp::os::File::SetFileAttributes (il2cpp::utils::StringUtils::Utf16ToUtf8 (path->chars), (il2cpp::os::File::FileAttributes)attrs, error);
}

bool MonoIO::Flush (Il2CppIntPtr handle, MonoIOError* error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::Flush (fileHandle, error);
}

bool MonoIO::SetFileTime (Il2CppIntPtr handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, MonoIOError* error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::SetFileTime (fileHandle, creation_time, last_access_time, last_write_time, error);
}

void MonoIO::Lock (Il2CppIntPtr handle, int64_t position, int64_t length, MonoIOError* error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::Lock (fileHandle, position, length, error);
}

void MonoIO::Unlock (Il2CppIntPtr handle, int64_t position, int64_t length, MonoIOError* error)
{
	il2cpp::os::FileHandle* fileHandle = (il2cpp::os::FileHandle*)handle.m_value;
	return il2cpp::os::File::Unlock (fileHandle, position, length, error);
}

bool MonoIO::CreatePipe (Il2CppIntPtr* read_handle, Il2CppIntPtr* write_handle)
{
	il2cpp::os::FileHandle* input = (il2cpp::os::FileHandle*)read_handle->m_value;
	il2cpp::os::FileHandle* output = (il2cpp::os::FileHandle*)write_handle->m_value;

	return il2cpp::os::File::CreatePipe(&input, &output);
}

bool MonoIO::DuplicateHandle (Il2CppIntPtr source_process_handle, Il2CppIntPtr source_handle, Il2CppIntPtr target_process_handle, Il2CppIntPtr* target_handle, int32_t access, int32_t inherit, int32_t options)
{
	// Only used on Windows in Process.Start_noshell.
	NOT_IMPLEMENTED_ICALL (MonoIO::DuplicateHandle);
	return false;
}

// This is never called from Mono.
int32_t MonoIO::GetTempPath (Il2CppString** path)
{
	const std::string tempPath(il2cpp::os::Path::GetTempPath());
	*path = vm::String::New (tempPath.c_str ());
	return vm::String::GetLength(*path);
}

} /* namespace IO */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
