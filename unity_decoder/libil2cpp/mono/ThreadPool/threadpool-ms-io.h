#pragma once
#if NET_4_0

#include "il2cpp-config.h"

struct Il2CppIOSelectorJob;
struct Il2CppIntPtr;

void threadpool_ms_io_remove_socket(int fd);
//void mono_threadpool_ms_io_remove_domain_jobs (MonoDomain *domain);
void threadpool_ms_io_cleanup(void);

LIBIL2CPP_CODEGEN_API void ves_icall_System_IOSelector_Add(Il2CppIntPtr handle, Il2CppIOSelectorJob *job);
LIBIL2CPP_CODEGEN_API void ves_icall_System_IOSelector_Remove(Il2CppIntPtr handle);

#endif
