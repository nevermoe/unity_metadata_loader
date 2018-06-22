#pragma once

#include "os/OSGlobalEnums.h"

#if defined(__cplusplus)
#include "os/ErrorCodes.h"
#include "os/File.h"

typedef il2cpp::os::FileHandle UnityPalFileHandle;
typedef il2cpp::os::FileStat UnityPalFileStat;

#else

typedef struct UnityPalFileHandle UnityPalFileHandle;

struct UnityPalFileStat
{
    char* name;
    int32_t attributes;
    int64_t length;
    int64_t creation_time;
    int64_t last_access_time;
    int64_t last_write_time;
} UnityPalFileStat;

#endif //__cplusplus

#if defined(__cplusplus)
extern "C"
{
#endif

bool UnityPalIsatty(UnityPalFileHandle* fileHandle);
UnityPalFileHandle* UnityPalGetStdInput();
UnityPalFileHandle* UnityPalGetStdOutput();
UnityPalFileHandle* UnityPalGetStdError();
bool UnityPalCreatePipe(UnityPalFileHandle** read_handle, UnityPalFileHandle** write_handle);
bool UnityPalCreatePipe_with_error(UnityPalFileHandle** read_handle, UnityPalFileHandle** write_handle, int* error);
FileType UnityPalGetFileType(UnityPalFileHandle* handle);
UnityPalFileAttributes UnityPalGetFileAttributes(const char* path, int* error);
bool UnityPalSetFileAttributes(const char* path, UnityPalFileAttributes attributes, int* error);
bool UnityPalGetFileStat(const char* path, UnityPalFileStat * stat, int* error);
bool UnityPalCopyFile(const char* src, const char* dest, bool overwrite, int* error);
bool UnityPalMoveFile(const char* src, const char* dest, int* error);
bool UnityPalDeleteFile(const char* path, int *error);
bool UnityPalReplaceFile(const char* sourceFileName, const char* destinationFileName, const char* destinationBackupFileName, bool ignoreMetadataErrors, int* error);
UnityPalFileHandle* UnityPalOpen(const char* path, int openMode, int accessMode, int shareMode, int options, int *error);
bool UnityPalClose(UnityPalFileHandle* handle, int *error);
bool UnityPalSetFileTime(UnityPalFileHandle* handle, int64_t creation_time, int64_t last_access_time, int64_t last_write_time, int* error);
int64_t UnityPalGetLength(UnityPalFileHandle* handle, int *error);
bool UnityPalSetLength(UnityPalFileHandle* handle, int64_t length, int *error);
int64_t UnityPalSeek(UnityPalFileHandle* handle, int64_t offset, int origin, int *error);
int UnityPalRead(UnityPalFileHandle* handle, char *dest, int count, int *error);
int32_t UnityPalWrite(UnityPalFileHandle* handle, const char* buffer, int count, int *error);
bool UnityPalFlush(UnityPalFileHandle* handle, int* error);
void UnityPalLock(UnityPalFileHandle* handle, int64_t position, int64_t length, int* error);
void UnityPalUnlock(UnityPalFileHandle* handle, int64_t position, int64_t length, int* error);
bool UnityPalDuplicateHandle(UnityPalFileHandle* source_process_handle, UnityPalFileHandle* source_handle, UnityPalFileHandle* target_process_handle,
    UnityPalFileHandle** target_handle, int access, int inherit, int options, int* error);


#if defined(__cplusplus)
}
#endif
