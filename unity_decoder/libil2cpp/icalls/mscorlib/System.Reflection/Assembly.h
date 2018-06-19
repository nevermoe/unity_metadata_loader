#pragma once

#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppString;
struct Il2CppAssemblyName;
struct Il2CppReflectionAssembly;
struct mscorlib_System_Reflection_Assembly;
struct mscorlib_System_Reflection_Module;
struct mscorlib_System_Security_Policy_Evidence;
struct mscorlib_System_Reflection_AssemblyName;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Reflection
{

class LIBIL2CPP_CODEGEN_API Assembly
{
public:
	static Il2CppReflectionAssembly* GetExecutingAssembly ();
	static Il2CppReflectionAssembly* GetEntryAssembly ();
	static Il2CppReflectionAssembly* GetCallingAssembly();
	static void FillName(Il2CppReflectionAssembly* ass, mscorlib_System_Reflection_AssemblyName* aname);
	static Il2CppObject* GetFilesInternal (Il2CppAssembly* self, Il2CppString* name, bool getResourceModules);
	static Il2CppReflectionModule* GetManifestModuleInternal (Il2CppAssembly* self);
	static bool GetManifestResourceInfoInternal(Il2CppReflectionAssembly* assembly, Il2CppString* name, Il2CppManifestResourceInfo* info);
	static Il2CppIntPtr GetManifestResourceInternal(Il2CppReflectionAssembly* assembly, Il2CppString* name, int* size, Il2CppReflectionModule** module);
	static Il2CppArray* GetManifestResourceNames(Il2CppReflectionAssembly* assembly);
	static Il2CppArray* GetModulesInternal (mscorlib_System_Reflection_Assembly* __this);
	static Il2CppArray* GetNamespaces (Il2CppAssembly* self);
	static Il2CppArray* GetReferencedAssemblies (Il2CppReflectionAssembly* self);
	static void InternalGetAssemblyName (Il2CppString* assemblyFile, Il2CppAssemblyName* aname);
	static Il2CppReflectionType* InternalGetType (Il2CppReflectionAssembly* ,mscorlib_System_Reflection_Module* ,Il2CppString* ,bool,bool);
	static Il2CppString* InternalImageRuntimeVersion (Il2CppAssembly* self);
	static Il2CppAssembly* LoadFrom (Il2CppString* assemblyFile, bool refonly);
	static bool LoadPermissions (mscorlib_System_Reflection_Assembly* a, Il2CppIntPtr* minimum, int32_t* minLength, Il2CppIntPtr* optional, int32_t* optLength, Il2CppIntPtr* refused, int32_t* refLength);
	static int32_t MonoDebugger_GetMethodToken (void* /* System.Reflection.MethodBase */ method);
	static Il2CppReflectionMethod* get_EntryPoint (Il2CppReflectionAssembly* self);
	static bool get_ReflectionOnly (Il2CppAssembly* self);
	static Il2CppString* get_code_base(Il2CppReflectionAssembly* assembly, bool escaped);
	static Il2CppString* get_fullname (Il2CppReflectionAssembly *ass);
	static bool get_global_assembly_cache (Il2CppAssembly* self);
	static Il2CppString* get_location (Il2CppReflectionAssembly *assembly);
	static Il2CppReflectionAssembly* load_with_partial_name(Il2CppString* name, mscorlib_System_Security_Policy_Evidence* evidence);
	static Il2CppArray* GetTypes(Il2CppReflectionAssembly* __this, bool exportedOnly);

};

} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
