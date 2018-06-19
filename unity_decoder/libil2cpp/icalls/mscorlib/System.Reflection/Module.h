#pragma once

#include "il2cpp-config.h"
#include "object-internals.h"

struct Il2CppArray;
struct Il2CppString;
struct mscorlib_System_Reflection_Module;

typedef int32_t PortableExecutableKinds;
typedef int32_t ImageFileMachine;
typedef int32_t ResolveTokenError;

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

class LIBIL2CPP_CODEGEN_API Module
{
public:
	static Il2CppReflectionType* GetGlobalType (Il2CppReflectionModule* self);
	static Il2CppString* GetGuidInternal (mscorlib_System_Reflection_Module * __this);
	static int32_t GetMDStreamVersion (Il2CppIntPtr module_handle);
	static void GetPEKind (Il2CppIntPtr module, PortableExecutableKinds* peKind, ImageFileMachine* machine);
	static Il2CppArray* InternalGetTypes (Il2CppReflectionModule * self);
	static Il2CppIntPtr ResolveFieldToken (Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error);
	static void* /* System.Reflection.MemberInfo */ ResolveMemberToken (Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error);
	static Il2CppIntPtr ResolveMethodToken (Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error);
	static Il2CppArray* ResolveSignature (Il2CppIntPtr module, int32_t metadataToken, ResolveTokenError* error);
	static Il2CppString* ResolveStringToken (Il2CppIntPtr module, int32_t token, ResolveTokenError* error);
	static Il2CppIntPtr ResolveTypeToken (Il2CppIntPtr module, int32_t token, Il2CppArray* type_args, Il2CppArray* method_args, ResolveTokenError* error);
	static int32_t get_MetadataToken (Il2CppReflectionModule* self);
	static Il2CppIntPtr GetHINSTANCE (mscorlib_System_Reflection_Module * __this);
};
	
} /* namespace Reflection */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
