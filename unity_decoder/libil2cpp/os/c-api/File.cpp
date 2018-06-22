#include "os/File.h"
#include "os/c-api/File-c-api.h"

extern "C"
{
bool UnityPalIsatty(UnityPalFileHandle* fileHandle)
{
    return il2cpp::os::File::Isatty(fileHandle);
}

UnityPalFileHandle* UnityPalGetStdInput()
{
    return il2cpp::os::File::GetStdInput();
}

UnityPalFileHandle* UnityPalGetStdOutput()
{
    return il2cpp::os::File::GetStdOutput();
}

UnityPalFileHandle* UnityPalGetStdError()
{
    return il2cpp::os::File::GetStdError();
}

bool UnityPalCreatePipe(UnityPalFileHandle** read_handle, UnityPalFileHandle** write_handle)
{
    return il2cpp::os::File::CreatePipe(read_handle, write_handle);
}

bool UnityPalCreatePipe_With_Error(UnityPalFileHandle** read_handle, UnityPalFileHandle** write_handle, int* error)
{
    return il2cpp::os::File::CreatePipe(read_handle, write_handle, error);
}

FileType UnityPalGetFileType(UnityPalFileHandle* handle)
{
    return il2cpp::os::File::GetFileType(handle);
}

UnityPalFileAttributes UnityPalGetFileAttributes(const char* path, int* error)
{
    return il2cpp::os::File::GetFileAttributes(path, error);
}

bool UnityPalSetFileAttributes(const char* path, UnityPalFileAttributes attributes, int* error)
{
    return il2cpp::os::File::SetFileAttributes(path, attributes, error);
}

bool UnityPalGetFileStat(const char* path, UnityPalFileStat * stat, int* error)
{
    return il2cpp::os::File::GetFileStat(path, stat, error);
}

bool UnityPalCopyFile(const char* src, const char* dest, bool overwrite, int* error)
{
    return il2cpp::os::File::CopyFile(src, dest, overwrite, error);
}

bool UnityPalMoveFile(const char* src, const char* dest, int* error)
{
    return il2cpp::os::File::MoveFile(src, dest, error);
}

bool UnityPalDeleteFile(const char* path, int *error)
{
    return il2cpp::os::File::DeleteFile(path, error);
}

bool UnityPalReplaceFile(const char* sourceFileName, const char* destinationFileName, const char* destinationBackupFileName, bool ignoreMetadataErrors, int* error)
{
    return il2cpp::os::File::ReplaceFile(sourceFileName, destinationFileName, destinationBackupFileName, ignoreMetadataErrors, error);
}

UnityPalFileHandle* UnityPalOpen(const char* path, int openMode, int accessMode, int shareMode, int options, int *error)
{
    return il2cpp::os::File::Open(path, openMode, accessMode, shareMode, options, error);
}

bool UnityPalClose(UnityPalFileHandle* handle, int *error)
{
    return il2cpp::os::File::Close(handle, error);
}

bool UnityPalSetFileTime(UnityPalFileHandle* handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, int* error)
{
    return il2cpp::os::File::SetFileTime(handle, creation_time, last_access_time, last_write_time, error);
}

int64_t UnityPalGetLength(UnityPalFileHandle* handle, int *error)
{
    return il2cpp::os::File::GetLength(handle, error);
}

bool UnityPalSetLength(UnityPalFileHandle* handle, int64_t length, int *error)
{
    return il2cpp::os::File::SetLength(handle, length, error);
}

int64_t UnityPalSeek(UnityPalFileHandle* handle, int64_t offset, int origin, int *error)
{
    return il2cpp::os::File::Seek(handle, offset, origin, error);
}

int UnityPalRead(UnityPalFileHandle* handle, char *dest, int count, int *error)
{
    return il2cpp::os::File::Read(handle, dest, count, error);
}

int32_t UnityPalWrite(UnityPalFileHandle* handle, const char* buffer, int count, int *error)
{
    return il2cpp::os::File::Write(handle, buffer, count, error);
}

bool UnityPalFlush(UnityPalFileHandle* handle, int* error)
{
    return il2cpp::os::File::Flush(handle, error);
}

void UnityPalLock(UnityPalFileHandle* handle, int64_t position, int64_t length, int* error)
{
    return il2cpp::os::File::Lock(handle, position, length, error);
}

void UnityPalUnlock(UnityPalFileHandle* handle, int64_t position, int64_t length, int* error)
{
    return il2cpp::os::File::Unlock(handle, position, length, error);
}

bool UnityPalDuplicateHandle(UnityPalFileHandle* source_process_handle, UnityPalFileHandle* source_handle, UnityPalFileHandle* target_process_handle,
    UnityPalFileHandle** target_handle, int access, int inherit, int options, int* error)
{
    return il2cpp::os::File::DuplicateHandle(source_process_handle, source_handle, target_process_handle, target_handle, access, inherit, options, error);
}
}
