#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "il2cpp-config.h"
#include "il2cpp-metadata.h"
#include "object-internals.h"
#include "metadata/GenericMethod.h"
#include "vm/Exception.h"
#include "vm/Class.h"
#include "vm/MetadataCache.h"
#include "utils/StringUtils.h"

struct Il2CppArray;
struct Il2CppDelegate;
struct Il2CppObject;
struct MethodInfo;
struct Il2CppClass;

struct VirtualInvokeData
{
	Il2CppMethodPointer methodPtr;
	const MethodInfo* method;
};

namespace il2cpp
{
namespace vm
{

class LIBIL2CPP_CODEGEN_API Runtime
{
public:
	static void Init (const char* filename, const char *runtime_version);
	static void Shutdown ();
	static void SetConfigDir (const char *path);
	static void SetDataDir(const char *path);
	static std::string GetConfigDir();
	static std::string GetDataDir();
	static const char *GetFrameworkVersion();
	static Il2CppObject* DelegateInvoke (Il2CppDelegate *obj, void **params, Il2CppException **exc);
	static Il2CppObject* Invoke (const MethodInfo *method, void *obj, void **params, Il2CppException **exc);
	static Il2CppObject* InvokeConvertArgs (const MethodInfo *method, void *obj, Il2CppObject **params, int paramCount, Il2CppException **exc);
	static Il2CppObject* InvokeArray (const MethodInfo *method, void *obj, Il2CppArray *params, Il2CppException **exc);
	static void ObjectInit (Il2CppObject* object);
	static void ObjectInitException (Il2CppObject* object, Il2CppException **exc);
	static void SetUnhandledExceptionPolicy (Il2CppRuntimeUnhandledExceptionPolicy value);

	static inline void GetVirtualInvokeData(Il2CppMethodSlot slot, void* obj, VirtualInvokeData* invokeData)
	{
		Assert(slot != 65535 && "GetVirtualInvokeData got called on a non-virtual method");

		*invokeData = ((Il2CppObject*)obj)->klass->vtable[slot];

		if (!invokeData->methodPtr)
			RaiseExecutionEngineExceptionIfMethodIsNotFound(invokeData->method);
	}

	static inline void GetInterfaceInvokeData(Il2CppMethodSlot slot, Il2CppClass* declaringInterface, void* obj, VirtualInvokeData* invokeData)
	{
		Assert(slot != 65535 && "GetInterfaceInvokeData got called on a non-virtual method");

		Il2CppClass* typeInfo = ((Il2CppObject*)obj)->klass;
		slot += Class::GetInterfaceOffset(typeInfo, declaringInterface);

		*invokeData = typeInfo->vtable[slot];

		if (!invokeData->methodPtr)
			RaiseExecutionEngineExceptionIfMethodIsNotFound(invokeData->method);
	}

	static inline void GetComInterfaceInvokeData(Il2CppMethodSlot slot, Il2CppClass* declaringInterface, void* obj, VirtualInvokeData* invokeData)
	{
		// check for managed object that implements com interface
		if (static_cast<Il2CppObject*>(obj)->klass != il2cpp_defaults.il2cpp_com_object_class)
		{
			GetInterfaceInvokeData(slot, declaringInterface, obj, invokeData);
			return;
		}

		// class is il2cpp_com_object_class, which means we created the object from native interface and we don't know its concrete type
		// in this case, invoke COM interface method directly

		// declaringInterface vtable starts at the very end (after implemeted interfaces' vtables)
		int32_t itf_offset = 0;
		if (declaringInterface->interface_offsets_count)
		{
			const Il2CppRuntimeInterfaceOffsetPair* pair = declaringInterface->interfaceOffsets + declaringInterface->interface_offsets_count - 1;
			itf_offset = pair->offset + pair->interfaceType->vtable_count;
		}
		assert(itf_offset != -1);
		slot += itf_offset;

		*invokeData = declaringInterface->vtable[slot];

#if IL2CPP_DEBUG
		assert(invokeData->method);
#endif

		if (!invokeData->methodPtr)
			RaiseExecutionEngineExceptionIfMethodIsNotFound(invokeData->method);
	}

	static inline void GetGenericVirtualInvokeData(const MethodInfo* method, void* obj, VirtualInvokeData* invokeData)
	{
		const Il2CppClass* typeInfo = ((Il2CppObject *)obj)->klass;
		uint16_t slot = method->slot;
		const MethodInfo* methodDefinition = typeInfo->vtable[slot].method;
		const MethodInfo* targetMethodInfo = GetGenericVirtualMethod(methodDefinition, method);
#if IL2CPP_DEBUG
		assert(targetMethodInfo);
#endif

		invokeData->methodPtr = targetMethodInfo->methodPointer;
		invokeData->method = targetMethodInfo;
	}

	static inline void GetGenericInterfaceInvokeData(const MethodInfo* method, void* obj, VirtualInvokeData* invokeData)
	{
		Il2CppClass* typeInfo = ((Il2CppObject *)obj)->klass;
		uint16_t slot = method->slot + Class::GetInterfaceOffset(typeInfo, method->declaring_type);

		const MethodInfo* methodDefinition = typeInfo->vtable[slot].method;
		const MethodInfo* targetMethodInfo = GetGenericVirtualMethod(methodDefinition, method);

#if IL2CPP_DEBUG
		assert(targetMethodInfo);
#endif

		invokeData->methodPtr = targetMethodInfo->methodPointer;
		invokeData->method = targetMethodInfo;
	}

	static void RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method);

public:
	// internal
	static Il2CppRuntimeUnhandledExceptionPolicy GetUnhandledExceptionPolicy ();
	static void UnhandledException (Il2CppException* exc);
	static void ClassInit (Il2CppClass *klass);

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
	struct MethodDefinitionKey
	{
		Il2CppMethodPointer method;
		MethodIndex methodIndex;
	};

	static void RegisterMethods (const std::vector<MethodDefinitionKey>& managedMethods);
	static const MethodInfo* GetMethodFromNativeSymbol (Il2CppMethodPointer nativeMethod);
#endif
	
	static const char *GetBundledMachineConfig ();
	static void RegisterBundledMachineConfig (const char *config_xml);

private:
	static void CallUnhandledExceptionDelegate (Il2CppDomain* domain, Il2CppDelegate* delegate, Il2CppException* exc);
	static Il2CppObject* CreateUnhandledExceptionEventArgs (Il2CppException* exc);

	static inline void RaiseExecutionEngineExceptionIfMethodIsNotFound(const MethodInfo* method, const Il2CppGenericMethod* genericMethod)
	{
		if (method->methodPointer == NULL)
			RaiseExecutionEngineException(metadata::GenericMethod::GetFullName(genericMethod).c_str());
	}

	static inline void RaiseExecutionEngineException(const char* methodFullName)
	{
		Exception::Raise(Exception::GetExecutionEngineException(utils::StringUtils::Printf("Attempting to call method '%s' for which no ahead of time (AOT) code was generated.", methodFullName).c_str()));
	}

	static inline const MethodInfo* GetGenericVirtualMethod(const MethodInfo* methodDefinition, const MethodInfo* inflatedMethod)
	{
		NOT_IMPLEMENTED_NO_ASSERT(GetGenericVirtualMethod, "We should only do the following slow method lookup once and then cache on type itself.");

		const Il2CppGenericInst* classInst = NULL;
		if (methodDefinition->is_inflated)
		{
			classInst = methodDefinition->genericMethod->context.class_inst;
			methodDefinition = methodDefinition->genericMethod->methodDefinition;
		}

		const Il2CppGenericMethod* gmethod = MetadataCache::GetGenericMethod(const_cast<MethodInfo*>(methodDefinition), classInst, inflatedMethod->genericMethod->context.method_inst);
		const MethodInfo* method = metadata::GenericMethod::GetMethod(gmethod);

		RaiseExecutionEngineExceptionIfMethodIsNotFound(method, gmethod);

		return method;
	}
};

} /* namespace vm */
} /* namespace il2cpp */
