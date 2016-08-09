#pragma once

#include <stdint.h>
#include "object-internals.h"
#include "il2cpp-config.h"

struct Il2CppObject;
struct Il2CppDelegate;
struct Il2CppReflectionType;
struct Il2CppReflectionMethod;
struct Il2CppReflectionField;
struct Il2CppArray;
struct Il2CppException;
struct Il2CppReflectionModule;
struct Il2CppAssembly;
struct Il2CppAssemblyName;
struct Il2CppAppDomain;

namespace il2cpp
{
namespace icalls
{
namespace System
{
namespace System
{
namespace IO
{

struct MonoIOError;

class LIBIL2CPP_CODEGEN_API MonoIO
{
public:
	static Il2CppIntPtr get_ConsoleError ();
	static Il2CppIntPtr get_ConsoleInput ();
	static Il2CppIntPtr get_ConsoleOutput ();
	static bool CreatePipe (Il2CppIntPtr* read_handle, Il2CppIntPtr* write_handle);
	static bool DuplicateHandle (Il2CppIntPtr source_process_handle, Il2CppIntPtr source_handle, Il2CppIntPtr target_process_handle, Il2CppIntPtr* target_handle, int32_t access, int32_t inherit, int32_t options);
	static int32_t GetTempPath (Il2CppString** path);
	static bool Close (Il2CppIntPtr handle, MonoIOError* error);
};

} /* namespace IO */
} /* namespace System */
} /* namespace System */
} /* namespace icalls */
} /* namespace il2cpp */
