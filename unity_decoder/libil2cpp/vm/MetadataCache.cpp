#include "il2cpp-config.h"
#include "MetadataCache.h"

#include <cassert>
#include <map>
#include <limits>
#include "class-internals.h"
#include "tabledefs.h"
#include "gc/GarbageCollector.h"
#include "metadata/ArrayMetadata.h"
#include "metadata/GenericMetadata.h"
#include "metadata/GenericMethod.h"
#include "metadata/Il2CppTypeCompare.h"
#include "metadata/Il2CppTypeHash.h"
#include "metadata/Il2CppTypeVector.h"
#include "metadata/Il2CppGenericContextCompare.h"
#include "metadata/Il2CppGenericContextHash.h"
#include "metadata/Il2CppGenericContextLess.h"
#include "metadata/Il2CppGenericInstCompare.h"
#include "metadata/Il2CppGenericInstHash.h"
#include "metadata/Il2CppGenericInstLess.h"
#include "metadata/Il2CppGenericMethodCompare.h"
#include "metadata/Il2CppGenericMethodHash.h"
#include "metadata/Il2CppGenericMethodLess.h"
#include "os/Atomic.h"
#include "os/Mutex.h"
#include "utils/CallOnce.h"
#include "utils/Collections.h"
#include "utils/HashUtils.h"
#include "utils/Memory.h"
#include "utils/StdUnorderedMap.h"
#include "utils/StdUnorderedSet.h"
#include "vm/Assembly.h"
#include "vm/Class.h"
#include "vm/GenericClass.h"
#include "vm/MetadataAlloc.h"
#include "vm/MetadataLoader.h"
#include "vm/MetadataLock.h"
#include "vm/Object.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Type.h"

using namespace il2cpp;
using namespace il2cpp::utils::collections;
using namespace il2cpp::vm;

using il2cpp::gc::GarbageCollector;
using il2cpp::metadata::ArrayMetadata;
using il2cpp::metadata::GenericMetadata;
using il2cpp::metadata::GenericMethod;
using il2cpp::metadata::Il2CppGenericContextCompare;
using il2cpp::metadata::Il2CppGenericContextHash;
using il2cpp::metadata::Il2CppGenericContextLess;
using il2cpp::metadata::Il2CppGenericInstCompare;
using il2cpp::metadata::Il2CppGenericInstHash;
using il2cpp::metadata::Il2CppGenericInstLess;
using il2cpp::metadata::Il2CppGenericMethodCompare;
using il2cpp::metadata::Il2CppGenericMethodHash;
using il2cpp::metadata::Il2CppGenericMethodLess;
using il2cpp::metadata::Il2CppTypeCompare;
using il2cpp::metadata::Il2CppTypeHash;
using il2cpp::metadata::Il2CppTypeVector;
using il2cpp::os::Atomic;
using il2cpp::os::FastAutoLock;
using il2cpp::os::FastMutex;
using il2cpp::utils::CallOnce;
using il2cpp::utils::HashUtils;
using il2cpp::utils::OnceFlag;

typedef std::map<Il2CppMethodPointer, const MethodInfo*> NativeDelegateMap;
typedef std::map<Il2CppClass*, Il2CppClass*> PointerTypeMap;

typedef unordered_set<const Il2CppGenericMethod*,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	Il2CppGenericMethodHash, Il2CppGenericMethodCompare
#else
	Il2CppGenericMethodLess
#endif
	> Il2CppGenericMethodSet;
typedef Il2CppGenericMethodSet::const_iterator Il2CppGenericMethodSetIter;
static Il2CppGenericMethodSet s_GenericMethodSet;

struct Il2CppMetadataCache
{
	FastMutex m_CacheMutex;
	NativeDelegateMap m_NativeDelegateMethods;
	PointerTypeMap m_PointerTypes;
};

static Il2CppMetadataCache s_MetadataCache;
static Il2CppClass** s_TypeInfoTable = NULL;
static Il2CppClass** s_TypeInfoDefinitionTable = NULL;
static const MethodInfo** s_MethodInfoDefinitionTable = NULL;
static Il2CppString** s_StringLiteralTable = NULL;
static const Il2CppGenericMethod** s_GenericMethodTable = NULL;
static int32_t s_ImagesCount = 0;
static Il2CppImage* s_ImagesTable = NULL;


typedef unordered_set<const Il2CppGenericInst*,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	Il2CppGenericInstHash, Il2CppGenericInstCompare
#else
	Il2CppGenericInstLess
#endif
> Il2CppGenericInstSet;
static Il2CppGenericInstSet s_GenericInstSet;

typedef unordered_map<const Il2CppGenericMethod*,
	const Il2CppGenericMethodIndices*,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	Il2CppGenericMethodHash, Il2CppGenericMethodCompare
#else
	Il2CppGenericMethodLess
#endif
> Il2CppMethodTableMap;
typedef Il2CppMethodTableMap::const_iterator Il2CppMethodTableMapIter;
static Il2CppMethodTableMap s_MethodTableMap;

static const Il2CppCodeRegistration * s_Il2CppCodeRegistration;
static const Il2CppMetadataRegistration * s_Il2CppMetadataRegistration;
static const Il2CppCodeGenOptions* s_Il2CppCodeGenOptions;
static CustomAttributesCache** s_CustomAttributesCaches;
static CustomAttributeTypeCache** s_CustomAttributeTypeCaches;

template <typename T>
static T MetadataOffset (void* metadata, size_t sectionOffset, size_t itemIndex)
{
	return reinterpret_cast<T> (reinterpret_cast<uint8_t*> (metadata) + sectionOffset) + itemIndex;
}

void MetadataCache::Register (const Il2CppCodeRegistration* const codeRegistration, const Il2CppMetadataRegistration* const metadataRegistration, const Il2CppCodeGenOptions* const codeGenOptions)
{
	s_Il2CppCodeRegistration = codeRegistration;
	s_Il2CppMetadataRegistration = metadataRegistration;
	s_Il2CppCodeGenOptions = codeGenOptions;

	for (int32_t j = 0; j < metadataRegistration->genericClassesCount; j++)
		if (metadataRegistration->genericClasses[j]->typeDefinitionIndex != kTypeIndexInvalid)
			metadata::GenericMetadata::RegisterGenericClass (metadataRegistration->genericClasses[j]);

	for (int32_t i = 0; i < metadataRegistration->genericInstsCount; i++)
		s_GenericInstSet.insert (metadataRegistration->genericInsts[i]);
}

static void* s_GlobalMetadata;
static const Il2CppGlobalMetadataHeader* s_GlobalMetadataHeader;

void MetadataCache::Initialize()
{
	s_GlobalMetadata = vm::MetadataLoader::LoadMetadataFile ("global-metadata.dat");
	s_GlobalMetadataHeader = (const Il2CppGlobalMetadataHeader*)s_GlobalMetadata;
	assert (s_GlobalMetadataHeader->sanity == 0xFAB11BAF);
	assert (s_GlobalMetadataHeader->version == 21);

	const Il2CppAssembly* assemblies = (const Il2CppAssembly*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->assembliesOffset);
	for (uint32_t i = 0; i < s_GlobalMetadataHeader->assembliesCount / sizeof(Il2CppAssembly); i++)
		il2cpp::vm::Assembly::Register(assemblies + i);

	// Pre-allocate these arrays so we don't need to lock when reading later.
	// These arrays hold the runtime metadata representation for metadata explicitly
	// referenced during conversion. There is a corresponding table of same size
	// in the converted metadata, giving a description of runtime metadata to construct.
	s_TypeInfoTable = (Il2CppClass**)IL2CPP_CALLOC (s_Il2CppMetadataRegistration->typesCount, sizeof (Il2CppClass*));
	s_TypeInfoDefinitionTable = (Il2CppClass**)IL2CPP_CALLOC (s_GlobalMetadataHeader->typeDefinitionsCount / sizeof (Il2CppTypeDefinition), sizeof (Il2CppClass*));
	s_MethodInfoDefinitionTable = (const MethodInfo**)IL2CPP_CALLOC (s_GlobalMetadataHeader->methodsCount / sizeof (Il2CppMethodDefinition), sizeof (MethodInfo*));
	s_GenericMethodTable = (const Il2CppGenericMethod**)IL2CPP_CALLOC (s_Il2CppMetadataRegistration->methodSpecsCount, sizeof (Il2CppGenericMethod*));
	s_ImagesCount = s_GlobalMetadataHeader->imagesCount / sizeof (Il2CppImageDefinition);
	s_ImagesTable = (Il2CppImage*)IL2CPP_CALLOC (s_ImagesCount, sizeof(Il2CppImage));

	// setup all the Il2CppImages. There are not many and it avoid locks later on
	const Il2CppImageDefinition* imagesDefinitions = (const Il2CppImageDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->imagesOffset);
	for (int32_t imageIndex = 0; imageIndex < s_ImagesCount; imageIndex++)
	{
		const Il2CppImageDefinition* imageDefinition = imagesDefinitions + imageIndex;
		Il2CppImage* image = s_ImagesTable + imageIndex;
		image->name = GetStringFromIndex (imageDefinition->nameIndex);
		image->assemblyIndex = imageDefinition->assemblyIndex;
		image->typeStart = imageDefinition->typeStart;
		image->typeCount = imageDefinition->typeCount;
		image->entryPointIndex = imageDefinition->entryPointIndex;
		image->token = imageDefinition->token;
	}

#if IL2CPP_ENABLE_NATIVE_STACKTRACES
	std::vector<Runtime::MethodDefinitionKey> managedMethods;

	
	const Il2CppTypeDefinition* typeDefinitions = (const Il2CppTypeDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->typeDefinitionsOffset);
	for (size_t i = 0; i < s_GlobalMetadataHeader->assembliesCount / sizeof (Il2CppAssembly); i++)
	{
		const Il2CppImage* image = MetadataCache::GetImageFromIndex (assemblies[i].imageIndex);

		for (size_t j = 0; j < image->typeCount; j++)
		{
			const Il2CppTypeDefinition* type = typeDefinitions + image->typeStart + j;

			for (uint16_t u = 0; u < type->method_count; u++)
			{
				const Il2CppMethodDefinition* methodDefinition = GetMethodDefinitionFromIndex (type->methodStart + u);
				Runtime::MethodDefinitionKey currentMethodList;
				currentMethodList.methodIndex = type->methodStart + u;
				currentMethodList.method = GetMethodPointerFromIndex (methodDefinition->methodIndex);
				if (currentMethodList.method)
					managedMethods.push_back (currentMethodList);
			}
		}
	}

	for (int32_t i = 0; i < s_Il2CppMetadataRegistration->genericMethodTableCount; i++)
	{
		const Il2CppGenericMethodFunctionsDefinitions* genericMethodIndices = s_Il2CppMetadataRegistration->genericMethodTable + i;

		Runtime::MethodDefinitionKey currentMethodList;

		GenericMethodIndex genericMethodIndex = genericMethodIndices->genericMethodIndex;

		assert(genericMethodIndex < s_Il2CppMetadataRegistration->methodSpecsCount);
		const Il2CppMethodSpec* methodSpec = s_Il2CppMetadataRegistration->methodSpecs + genericMethodIndex;

		currentMethodList.methodIndex = methodSpec->methodDefinitionIndex;

		assert(genericMethodIndices->indices.methodIndex < static_cast<int32_t>(s_Il2CppCodeRegistration->genericMethodPointersCount));
		currentMethodList.method = s_Il2CppCodeRegistration->genericMethodPointers[genericMethodIndices->indices.methodIndex];

		managedMethods.push_back(currentMethodList);
	}

	Runtime::RegisterMethods(managedMethods);
#endif
}

// this is called later in the intialization cycle with more systems setup like GC
void MetadataCache::InitializeGCSafe ()
{
	s_StringLiteralTable = (Il2CppString**)GarbageCollector::AllocateFixed (s_GlobalMetadataHeader->stringLiteralCount / sizeof (Il2CppStringLiteral) * sizeof (Il2CppString*), NULL);

	for (int32_t i = 0; i < s_Il2CppMetadataRegistration->genericMethodTableCount; i++)
	{
		const Il2CppGenericMethodFunctionsDefinitions* genericMethodIndices = s_Il2CppMetadataRegistration->genericMethodTable + i;
		const Il2CppGenericMethod* genericMethod = GetGenericMethodFromIndex (genericMethodIndices->genericMethodIndex);
		s_MethodTableMap.insert (std::make_pair (genericMethod, &genericMethodIndices->indices));
	}
}

Il2CppClass* MetadataCache::GetGenericInstanceType (Il2CppClass* genericTypeDefinition, const Il2CppTypeVector& genericArgumentTypes)
{
	const Il2CppGenericInst* inst = MetadataCache::GetGenericInst (genericArgumentTypes);
	Il2CppGenericClass* genericClass = GenericMetadata::GetGenericClass (genericTypeDefinition, inst);
	return GenericClass::GetClass (genericClass);
}

const MethodInfo* MetadataCache::GetGenericInstanceMethod (const MethodInfo* genericMethodDefinition, const Il2CppGenericContext* context)
{
	const MethodInfo* method = genericMethodDefinition;
	const Il2CppGenericInst* classInst = context->class_inst;
	const Il2CppGenericInst* methodInst = context->method_inst;
	if (genericMethodDefinition->is_inflated)
	{
		assert (genericMethodDefinition->declaring_type->generic_class);
		classInst = genericMethodDefinition->declaring_type->generic_class->context.class_inst;
		method = genericMethodDefinition->genericMethod->methodDefinition;
	}

	const Il2CppGenericMethod* gmethod = GetGenericMethod (method, classInst, methodInst);
	return GenericMethod::GetMethod (gmethod);
}

const MethodInfo* MetadataCache::GetGenericInstanceMethod (const MethodInfo* genericMethodDefinition, const Il2CppTypeVector& genericArgumentTypes)
{
	Il2CppGenericContext context = { NULL, GetGenericInst (genericArgumentTypes) };

	return GetGenericInstanceMethod (genericMethodDefinition, &context);
}

const Il2CppGenericContext* MetadataCache::GetMethodGenericContext(const MethodInfo* method)
{
	if (!method->is_inflated)
	{
		NOT_IMPLEMENTED(Image::GetMethodGenericContext);
		return NULL;
	}

	return &method->genericMethod->context;
}

const MethodInfo* MetadataCache::GetGenericMethodDefinition(const MethodInfo* method)
{
	if (!method->is_inflated)
	{
		NOT_IMPLEMENTED (Image::GetGenericMethodDefinition);
		return NULL;
	}

	return method->genericMethod->methodDefinition;
}

const Il2CppGenericContainer* MetadataCache::GetMethodGenericContainer(const MethodInfo* method)
{
	if (!method->is_generic)
	{
		NOT_IMPLEMENTED (Image::GetMethodGenericContainer);
		return NULL;
	}

	return method->genericContainer;
}

const MethodInfo* MetadataCache::GetNativeDelegate (Il2CppMethodPointer nativeFunctionPointer)
{
	os::FastAutoLock lock(&s_MetadataCache.m_CacheMutex);

	NativeDelegateMap::iterator i = s_MetadataCache.m_NativeDelegateMethods.find(nativeFunctionPointer);
	if (i == s_MetadataCache.m_NativeDelegateMethods.end())
		return NULL;

	return i->second;
}

void MetadataCache::AddNativeDelegate(Il2CppMethodPointer nativeFunctionPointer, const MethodInfo* managedMethodInfo)
{
	os::FastAutoLock lock(&s_MetadataCache.m_CacheMutex);
	s_MetadataCache.m_NativeDelegateMethods.insert(std::make_pair(nativeFunctionPointer, managedMethodInfo));
}

Il2CppClass* MetadataCache::GetPointerType(Il2CppClass* type)
{
	os::FastAutoLock lock(&s_MetadataCache.m_CacheMutex);

	PointerTypeMap::const_iterator i = s_MetadataCache.m_PointerTypes.find(type);
	if (i == s_MetadataCache.m_PointerTypes.end())
		return NULL;

	return i->second;
}

void MetadataCache::AddPointerType(Il2CppClass* type, Il2CppClass* pointerType)
{
	os::FastAutoLock lock(&s_MetadataCache.m_CacheMutex);
	s_MetadataCache.m_PointerTypes.insert(std::make_pair(type, pointerType));
}

const Il2CppGenericInst* MetadataCache::GetGenericInst (const Il2CppTypeVector& types)
{
	// temporary inst to lookup a permanent one that may already exist
	Il2CppGenericInst inst;
	inst.type_argc = (uint32_t)types.size ();
	inst.type_argv = (const Il2CppType**)alloca (inst.type_argc * sizeof (Il2CppType*));

	size_t index = 0;
	for (Il2CppTypeVector::const_iterator iter = types.begin (); iter != types.end (); ++iter, ++index)
		inst.type_argv[index] = *iter;

	os::FastAutoLock lock (&s_MetadataCache.m_CacheMutex);
	Il2CppGenericInstSet::const_iterator iter = s_GenericInstSet.find (&inst);
	if (iter != s_GenericInstSet.end ())
		return *iter;

	Il2CppGenericInst* newInst = (Il2CppGenericInst*)IL2CPP_MALLOC (sizeof (Il2CppGenericInst));
	newInst->type_argc = (uint32_t)types.size ();
	newInst->type_argv = (const Il2CppType**)IL2CPP_MALLOC (newInst->type_argc * sizeof (Il2CppType*));

	index = 0;
	for (Il2CppTypeVector::const_iterator iter = types.begin (); iter != types.end (); ++iter, ++index)
		newInst->type_argv[index] = *iter;

	s_GenericInstSet.insert (newInst);

	++il2cpp_runtime_stats.generic_instance_count;

	return newInst;

}

static FastMutex s_GenericMethodMutex;
const Il2CppGenericMethod* MetadataCache::GetGenericMethod (const MethodInfo* methodDefinition, const Il2CppGenericInst* classInst, const Il2CppGenericInst* methodInst)
{
	Il2CppGenericMethod method = { 0 };
	method.methodDefinition = methodDefinition;
	method.context.class_inst = classInst;
	method.context.method_inst = methodInst;

	FastAutoLock lock (&s_GenericMethodMutex);
	Il2CppGenericMethodSet::const_iterator iter = s_GenericMethodSet.find (&method);
	if (iter != s_GenericMethodSet.end ())
		return *iter;

	Il2CppGenericMethod* newMethod = MetadataAllocGenericMethod ();
	newMethod->methodDefinition = methodDefinition;
	newMethod->context.class_inst = classInst;
	newMethod->context.method_inst = methodInst;

	s_GenericMethodSet.insert (newMethod);

	return newMethod;
}

// this logic must match the C# logic in GenericSharingAnalysis.GetSharedTypeForGenericParameter
static const Il2CppGenericInst* GetSharedInst (const Il2CppGenericInst* inst)
{
	if (inst == NULL)
		return NULL;

	Il2CppTypeVector types;
	for (uint32_t i = 0; i < inst->type_argc; ++i)
	{
		if (Type::IsReference (inst->type_argv[i]))
			types.push_back (il2cpp_defaults.object_class->byval_arg);
		else
		{
			const Il2CppType* type = inst->type_argv[i];
			if (s_Il2CppCodeGenOptions->enablePrimitiveValueTypeGenericSharing)
			{
				type = Type::GetUnderlyingType(type);
				if (type->type == IL2CPP_TYPE_BOOLEAN)
					type = il2cpp_defaults.byte_class->byval_arg;
				else if (type->type == IL2CPP_TYPE_CHAR)
					type = il2cpp_defaults.uint16_class->byval_arg;
			}

			if (Type::IsGenericInstance(type))
			{
				const Il2CppGenericInst* sharedInst = GetSharedInst (type->data.generic_class->context.class_inst);
				Il2CppGenericClass* gklass = GenericMetadata::GetGenericClass (type->data.generic_class->typeDefinitionIndex, sharedInst);
				Il2CppClass* klass = GenericClass::GetClass (gklass);
				type = klass->byval_arg;
			}
			types.push_back (type);
		}
	}

	const Il2CppGenericInst* sharedInst = MetadataCache::GetGenericInst (types);

	return sharedInst;
}

InvokerMethod MetadataCache::GetInvokerMethodPointer (const MethodInfo* methodDefinition, const Il2CppGenericContext* context)
{
	Il2CppGenericMethod method = { 0 };
	method.methodDefinition = const_cast<MethodInfo*>(methodDefinition);
	method.context.class_inst = context->class_inst;
	method.context.method_inst = context->method_inst;

	Il2CppMethodTableMapIter iter = s_MethodTableMap.find (&method);
	if (iter != s_MethodTableMap.end ())
	{
		assert(iter->second->invokerIndex >= 0);
		if (static_cast<uint32_t>(iter->second->invokerIndex) < s_Il2CppCodeRegistration->invokerPointersCount)
			return s_Il2CppCodeRegistration->invokerPointers[iter->second->invokerIndex];
		return NULL;
	}
	// get the shared version if it exists
	method.context.class_inst = GetSharedInst (context->class_inst);
	method.context.method_inst = GetSharedInst (context->method_inst);

	iter = s_MethodTableMap.find (&method);
	if (iter != s_MethodTableMap.end ())
	{
		assert(iter->second->invokerIndex >= 0);
		if (static_cast<uint32_t>(iter->second->invokerIndex) < s_Il2CppCodeRegistration->invokerPointersCount)
			return s_Il2CppCodeRegistration->invokerPointers[iter->second->invokerIndex];
		return NULL;
	}

	return NULL;
}

Il2CppMethodPointer MetadataCache::GetMethodPointer (const MethodInfo* methodDefinition, const Il2CppGenericContext* context)
{
	Il2CppGenericMethod method = { 0 };
	method.methodDefinition = const_cast<MethodInfo*>(methodDefinition);
	method.context.class_inst = context->class_inst;
	method.context.method_inst = context->method_inst;

	Il2CppMethodTableMapIter iter = s_MethodTableMap.find (&method);
	if (iter != s_MethodTableMap.end ())
	{
		assert(iter->second->invokerIndex >= 0);
		if (static_cast<uint32_t>(iter->second->methodIndex) < s_Il2CppCodeRegistration->genericMethodPointersCount)
			return s_Il2CppCodeRegistration->genericMethodPointers[iter->second->methodIndex];
		return NULL;
	}

	method.context.class_inst = GetSharedInst (context->class_inst);
	method.context.method_inst = GetSharedInst (context->method_inst);

	iter = s_MethodTableMap.find (&method);
	if (iter != s_MethodTableMap.end ())
	{
		assert(iter->second->invokerIndex >= 0);
		if (static_cast<uint32_t>(iter->second->methodIndex) < s_Il2CppCodeRegistration->genericMethodPointersCount)
			return s_Il2CppCodeRegistration->genericMethodPointers[iter->second->methodIndex];
		return NULL;
	}

	return NULL;
}

Il2CppClass* MetadataCache::GetTypeInfoFromTypeIndex (TypeIndex index)
{
	if (index == kTypeIndexInvalid)
		return NULL;

	assert (index < s_Il2CppMetadataRegistration->typesCount && "Invalid type index ");

	if (s_TypeInfoTable[index])
		return s_TypeInfoTable[index];

	const Il2CppType* type = s_Il2CppMetadataRegistration->types[index];
	s_TypeInfoTable[index] = il2cpp::vm::Class::FromIl2CppType (type);
	Class::Init (s_TypeInfoTable[index]);

	return s_TypeInfoTable[index];
}

const Il2CppType* MetadataCache::GetIl2CppTypeFromIndex (TypeIndex index)
{
	if (index == kTypeIndexInvalid)
		return NULL;

	assert (index < s_Il2CppMetadataRegistration->typesCount && "Invalid type index ");

	return s_Il2CppMetadataRegistration->types[index];
}

const MethodInfo* MetadataCache::GetMethodInfoFromIndex (EncodedMethodIndex methodIndex)
{
	uint32_t index = GetDecodedMethodIndex (methodIndex);

	if (index == 0)
		return NULL;

	if (GetEncodedIndexType (methodIndex) == kIl2CppMetadataUsageMethodRef)
		return GenericMethod::GetMethod (GetGenericMethodFromIndex (index));
	else
		return MetadataCache::GetMethodInfoFromMethodDefinitionIndex (index);
}

const Il2CppGenericMethod* MetadataCache::GetGenericMethodFromIndex (GenericMethodIndex index)
{
	assert (index < s_Il2CppMetadataRegistration->methodSpecsCount);
	if (s_GenericMethodTable[index])
		return s_GenericMethodTable[index];

	const Il2CppMethodSpec* methodSpec = s_Il2CppMetadataRegistration->methodSpecs + index;
	const MethodInfo* methodDefinition = GetMethodInfoFromMethodDefinitionIndex (methodSpec->methodDefinitionIndex);
	const Il2CppGenericInst* classInst = NULL;
	const Il2CppGenericInst* methodInst = NULL;
	if (methodSpec->classIndexIndex != -1)
	{
		assert (methodSpec->classIndexIndex < s_Il2CppMetadataRegistration->genericInstsCount);
		classInst = s_Il2CppMetadataRegistration->genericInsts[methodSpec->classIndexIndex];
	}
	if (methodSpec->methodIndexIndex != -1)
	{
		assert (methodSpec->methodIndexIndex < s_Il2CppMetadataRegistration->genericInstsCount);
		methodInst = s_Il2CppMetadataRegistration->genericInsts[methodSpec->methodIndexIndex];
	}
	s_GenericMethodTable[index] = GetGenericMethod (methodDefinition, classInst, methodInst);

	return s_GenericMethodTable[index];
}

Il2CppMethodPointer MetadataCache::GetMethodPointerFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->methodPointersCount);
	return s_Il2CppCodeRegistration->methodPointers[index];
}

InvokerMethod MetadataCache::GetMethodInvokerFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->invokerPointersCount);
	return s_Il2CppCodeRegistration->invokerPointers[index];
}

Il2CppMethodPointer MetadataCache::GetReversePInvokeWrapperFromIndex(MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->reversePInvokeWrapperCount);
	return s_Il2CppCodeRegistration->reversePInvokeWrappers[index];
}

Il2CppMethodPointer MetadataCache::GetDelegateWrapperManagedToNativeFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;


	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->delegateWrappersFromManagedToNativeCount);
	return s_Il2CppCodeRegistration->delegateWrappersFromManagedToNative[index];
}

Il2CppMethodPointer MetadataCache::GetMarshalToNativeFuncFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;


	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->marshalingFunctionsCount);
	return s_Il2CppCodeRegistration->marshalingFunctions[index].marshal_to_native_func;
}

Il2CppMethodPointer MetadataCache::GetMarshalFromNativeFuncFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;


	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->marshalingFunctionsCount);
	return s_Il2CppCodeRegistration->marshalingFunctions[index].marshal_from_native_func;
}

Il2CppMethodPointer MetadataCache::GetMarshalCleanupFuncFromIndex (MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;


	assert (index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->marshalingFunctionsCount);
	return s_Il2CppCodeRegistration->marshalingFunctions[index].marshal_cleanup_func;
}

Il2CppMethodPointer MetadataCache::GetCreateCcwFuncFromIndex(MethodIndex index)
{
	if (index == kMethodIndexInvalid)
		return NULL;

	assert(index >= 0 && static_cast<uint32_t>(index) < s_Il2CppCodeRegistration->ccwMarshalingFunctionsCount);
	return s_Il2CppCodeRegistration->ccwMarshalingFunctions[index];
}

const Il2CppGuid* MetadataCache::GetGuid(GuidIndex index)
{
	if (index == kGuidIndexInvalid)
		return NULL;

	assert(index >= 0 && index < s_Il2CppCodeRegistration->guidCount);
	return s_Il2CppCodeRegistration->guids[index];
}

static const Il2CppImage* GetImageForTypeDefinitionIndex (TypeDefinitionIndex index)
{
	for (int32_t imageIndex = 0; imageIndex < s_ImagesCount; imageIndex++)
	{
		const Il2CppImage* image = s_ImagesTable + imageIndex;
		assert(index >= 0);
		if (index >= image->typeStart && static_cast<uint32_t>(index) < (image->typeStart + image->typeCount))
			return image;
	}

	assert (0 && "Failed to find owning image for type");
	return NULL;
}

enum PackingSize
{
	Zero,
	One,
	Two,
	Four,
	Eight,
	Sixteen,
	ThirtyTwo,
	SixtyFour,
	OneHundredTwentyEight
};

static uint8_t ConvertPackingSizeEnumToValue(PackingSize packingSize)
{
	switch (packingSize)
	{
	case Zero:
		return 0;
	case One:
		return 1;
	case Two:
		return 2;
	case Four:
		return 4;
	case Eight:
		return 8;
	case Sixteen:
		return 16;
	case ThirtyTwo:
		return 32;
	case SixtyFour:
		return 64;
	case OneHundredTwentyEight:
		return 128;
	default:
		Assert(0 && "Invalid packing size!");
		return 0;
	}
}

static const int kBitIsValueType = 1;
static const int kBitIsEnum = 2;
static const int kBitHasFinalizer = 3;
static const int kBitHasStaticConstructor = 4;
static const int kBitIsBlittable = 5;
static const int kBitIsImportOrWindowsRuntime = 6;
static const int kPackingSize = 7; // This uses 4 bits from bit 7 to bit 10

static Il2CppClass* FromTypeDefinition (TypeDefinitionIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->typeDefinitionsCount / sizeof (Il2CppTypeDefinition));
	const Il2CppTypeDefinition* typeDefinitions = (const Il2CppTypeDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->typeDefinitionsOffset);
	const Il2CppTypeDefinition* typeDefinition = typeDefinitions + index;
	const Il2CppTypeDefinitionSizes* typeDefinitionSizes = s_Il2CppMetadataRegistration->typeDefinitionsSizes + index;
	Il2CppClass* typeInfo = (Il2CppClass*)IL2CPP_CALLOC (1, sizeof(Il2CppClass));
	typeInfo->image = GetImageForTypeDefinitionIndex (index);
	typeInfo->name = MetadataCache::GetStringFromIndex (typeDefinition->nameIndex);
	typeInfo->namespaze = MetadataCache::GetStringFromIndex (typeDefinition->namespaceIndex);
	typeInfo->customAttributeIndex = typeDefinition->customAttributeIndex;
	typeInfo->byval_arg = MetadataCache::GetIl2CppTypeFromIndex (typeDefinition->byvalTypeIndex);
	typeInfo->this_arg = MetadataCache::GetIl2CppTypeFromIndex (typeDefinition->byrefTypeIndex);
	typeInfo->typeDefinition = typeDefinition;
	typeInfo->genericContainerIndex = typeDefinition->genericContainerIndex;
	typeInfo->instance_size = typeDefinitionSizes->instance_size;
	typeInfo->actualSize = typeDefinitionSizes->instance_size; // actualySize is instance_size for compiler generated values
	typeInfo->native_size = typeDefinitionSizes->native_size;
	typeInfo->static_fields_size = typeDefinitionSizes->static_fields_size;
	typeInfo->thread_static_fields_size = typeDefinitionSizes->thread_static_fields_size;
	typeInfo->thread_static_fields_offset = -1;
	typeInfo->flags = typeDefinition->flags;
	typeInfo->valuetype = (typeDefinition->bitfield >> (kBitIsValueType - 1)) & 0x1;
	typeInfo->enumtype = (typeDefinition->bitfield >> (kBitIsEnum - 1)) & 0x1;
	typeInfo->is_generic = typeDefinition->genericContainerIndex != kGenericContainerIndexInvalid; // generic if we have a generic container
	typeInfo->has_finalize = (typeDefinition->bitfield >> (kBitHasFinalizer - 1)) & 0x1;
	typeInfo->has_cctor = (typeDefinition->bitfield >> (kBitHasStaticConstructor - 1)) & 0x1;
	typeInfo->is_blittable = (typeDefinition->bitfield >> (kBitIsBlittable - 1)) & 0x1;
	typeInfo->is_import_or_windows_runtime = (typeDefinition->bitfield >> (kBitIsImportOrWindowsRuntime - 1)) & 0x1;
	typeInfo->packingSize = ConvertPackingSizeEnumToValue(static_cast<PackingSize>((typeDefinition->bitfield >> (kPackingSize - 1)) & 0xF));
	typeInfo->method_count = typeDefinition->method_count;
	typeInfo->property_count = typeDefinition->property_count;
	typeInfo->field_count = typeDefinition->field_count;
	typeInfo->event_count = typeDefinition->event_count;
	typeInfo->nested_type_count = typeDefinition->nested_type_count;
	typeInfo->vtable_count = typeDefinition->vtable_count;
	typeInfo->interfaces_count = typeDefinition->interfaces_count;
	typeInfo->interface_offsets_count = typeDefinition->interface_offsets_count;
	typeInfo->token = typeDefinition->token;
	
	if (typeDefinition->parentIndex != kTypeIndexInvalid)
		typeInfo->parent = Class::FromIl2CppType (MetadataCache::GetIl2CppTypeFromIndex (typeDefinition->parentIndex));

	if (typeDefinition->declaringTypeIndex != kTypeIndexInvalid)
		typeInfo->declaringType = Class::FromIl2CppType (MetadataCache::GetIl2CppTypeFromIndex (typeDefinition->declaringTypeIndex));
	
	typeInfo->castClass = typeInfo->element_class = typeInfo;
	if (typeInfo->enumtype)
		typeInfo->castClass = typeInfo->element_class = Class::FromIl2CppType (MetadataCache::GetIl2CppTypeFromIndex (typeDefinition->elementTypeIndex));

	return typeInfo;
}

const Il2CppAssembly* MetadataCache::GetAssemblyFromIndex (AssemblyIndex index)
{
	if (index == kGenericContainerIndexInvalid)
		return NULL;
	
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->assembliesCount / sizeof (Il2CppAssembly));
	const Il2CppAssembly* assemblies = (const Il2CppAssembly*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->assembliesOffset);
	return assemblies + index;
}

Il2CppImage* MetadataCache::GetImageFromIndex (ImageIndex index)
{
	if (index == kGenericContainerIndexInvalid)
		return NULL;

	assert(index <= s_ImagesCount);
	return s_ImagesTable + index;
}

Il2CppClass* MetadataCache::GetTypeInfoFromTypeDefinitionIndex (TypeDefinitionIndex index)
{
	if (index == kTypeIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->typeDefinitionsCount / sizeof (Il2CppTypeDefinition));

	if (!s_TypeInfoDefinitionTable[index])
	{
		// we need to use the metadata lock, since we may need to retrieve other Il2CppClass's when setting. Our parent may be a generic instance for example
		FastAutoLock lock (&g_MetadataLock);
		// double checked locking
		if (!s_TypeInfoDefinitionTable[index])
			s_TypeInfoDefinitionTable[index] = FromTypeDefinition (index);
	}

	return s_TypeInfoDefinitionTable[index];
}

const Il2CppTypeDefinition* MetadataCache::GetTypeDefinitionFromIndex (TypeDefinitionIndex index)
{
	if (index == kTypeDefinitionIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->typeDefinitionsCount / sizeof (Il2CppTypeDefinition));
	const Il2CppTypeDefinition* typeDefinitions = (const Il2CppTypeDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->typeDefinitionsOffset);
	return typeDefinitions + index;
}

const Il2CppGenericContainer* MetadataCache::GetGenericContainerFromIndex (GenericContainerIndex index)
{
	if (index == kGenericContainerIndexInvalid)
		return NULL;
	
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->genericContainersCount / sizeof (Il2CppGenericContainer));
	const Il2CppGenericContainer* genericContainers = (const Il2CppGenericContainer*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->genericContainersOffset);
	return genericContainers + index;
}

const Il2CppGenericParameter* MetadataCache::GetGenericParameterFromIndex (GenericParameterIndex index)
{
	if (index == kGenericParameterIndexInvalid)
		return NULL;

	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->genericParametersCount / sizeof (Il2CppGenericParameter));
	const Il2CppGenericParameter* genericParameters = (const Il2CppGenericParameter*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->genericParametersOffset);
	return genericParameters + index;
}

const Il2CppType* MetadataCache::GetGenericParameterConstraintFromIndex (GenericParameterConstraintIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->genericParameterConstraintsCount / sizeof (TypeIndex));
	const TypeIndex* constraintIndices = (const TypeIndex*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->genericParameterConstraintsOffset);

	return GetIl2CppTypeFromIndex (constraintIndices[index]);
}

Il2CppClass* MetadataCache::GetNestedTypeFromIndex (NestedTypeIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->nestedTypesCount / sizeof (TypeDefinitionIndex));
	const TypeDefinitionIndex* nestedTypeIndices = (const TypeDefinitionIndex*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->nestedTypesOffset);

	return GetTypeInfoFromTypeDefinitionIndex (nestedTypeIndices[index]);
}

const Il2CppType* MetadataCache::GetInterfaceFromIndex (InterfacesIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->interfacesCount / sizeof (TypeIndex));
	const TypeIndex* interfaceIndices = (const TypeIndex*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->interfacesOffset);

	return GetIl2CppTypeFromIndex (interfaceIndices[index]);
}

EncodedMethodIndex MetadataCache::GetVTableMethodFromIndex (VTableIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->vtableMethodsCount / sizeof (EncodedMethodIndex));
	const EncodedMethodIndex* methodReferences = (const EncodedMethodIndex*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->vtableMethodsOffset);

	return methodReferences[index];
}

Il2CppInterfaceOffsetPair MetadataCache::GetInterfaceOffsetIndex (InterfaceOffsetIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->interfaceOffsetsCount / sizeof (Il2CppInterfaceOffsetPair));
	const Il2CppInterfaceOffsetPair* interfaceOffsets = (const Il2CppInterfaceOffsetPair*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->interfaceOffsetsOffset);

	return interfaceOffsets[index];
}

const Il2CppRGCTXDefinition* MetadataCache::GetRGCTXDefinitionFromIndex (RGCTXIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->rgctxEntriesCount / sizeof (Il2CppRGCTXDefinition));
	const Il2CppRGCTXDefinition* rgctxEntries = (const Il2CppRGCTXDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->rgctxEntriesOffset);

	return rgctxEntries  + index;
}

const Il2CppEventDefinition* MetadataCache::GetEventDefinitionFromIndex (EventIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->eventsCount / sizeof (Il2CppEventDefinition));
	const Il2CppEventDefinition* events = (const Il2CppEventDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->eventsOffset);
	return events + index;
}

const Il2CppFieldDefinition* MetadataCache::GetFieldDefinitionFromIndex (FieldIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->fieldsCount / sizeof (Il2CppFieldDefinition));
	const Il2CppFieldDefinition* fields = (const Il2CppFieldDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldsOffset);
	return fields + index;
}

const Il2CppFieldDefaultValue* MetadataCache::GetFieldDefaultValueFromIndex (FieldIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->fieldDefaultValuesCount / sizeof (Il2CppFieldDefaultValue));
	const Il2CppFieldDefaultValue* defaultValues = (const Il2CppFieldDefaultValue*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldDefaultValuesOffset);
	return defaultValues + index;
}

const uint8_t* MetadataCache::GetFieldDefaultValueDataFromIndex (FieldIndex index)
{
	assert (index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->fieldAndParameterDefaultValueDataCount / sizeof (uint8_t));
	const uint8_t* defaultValuesData = (const uint8_t*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldAndParameterDefaultValueDataOffset);
	return defaultValuesData + index;
}

const Il2CppFieldDefaultValue* MetadataCache::GetFieldDefaultValueForField (const FieldInfo* field)
{
	Il2CppClass* parent = field->parent;
	size_t fieldIndex = (field - parent->fields);
	if (Type::IsGenericInstance(parent->byval_arg))
		fieldIndex += GenericClass::GetTypeDefinition(parent->generic_class)->typeDefinition->fieldStart;
	else
		fieldIndex += parent->typeDefinition->fieldStart;
	const Il2CppFieldDefaultValue *start = (const Il2CppFieldDefaultValue *)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldDefaultValuesOffset);
	const Il2CppFieldDefaultValue *entry = start;
	while (entry < start + s_GlobalMetadataHeader->fieldDefaultValuesCount)
	{
		if (fieldIndex == entry->fieldIndex)
		{
			return entry;
		}
		entry++;
	}
	assert (0);
	return NULL;
}

const Il2CppParameterDefaultValue * il2cpp::vm::MetadataCache::GetParameterDefaultValueForParameter(const MethodInfo* method, const ParameterInfo* parameter)
{
	if (method->methodDefinition == NULL)
		return NULL;

	size_t parameterIndex = method->methodDefinition->parameterStart + parameter->position;
	const Il2CppParameterDefaultValue *start = (const Il2CppParameterDefaultValue *)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->parameterDefaultValuesOffset);
	const Il2CppParameterDefaultValue *entry = start;
	while (entry < start + s_GlobalMetadataHeader->parameterDefaultValuesCount)
	{
		if (parameterIndex == entry->parameterIndex)
			return entry;
		entry++;
	}

	return NULL;
}

const uint8_t* MetadataCache::GetParameterDefaultValueDataFromIndex(ParameterIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->fieldAndParameterDefaultValueDataCount / sizeof(uint8_t));
	const uint8_t* defaultValuesData = (const uint8_t*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldAndParameterDefaultValueDataOffset);
	return defaultValuesData + index;
}

int MetadataCache::GetFieldMarshaledSizeForField(const FieldInfo* field)
{
	Il2CppClass* parent = field->parent;
	size_t fieldIndex = (field - parent->fields);
	fieldIndex += parent->typeDefinition->fieldStart;
	const Il2CppFieldMarshaledSize *start = (const Il2CppFieldMarshaledSize *)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->fieldMarshaledSizesOffset);
	const Il2CppFieldMarshaledSize *entry = start;
	while ((intptr_t)entry < (intptr_t)start + s_GlobalMetadataHeader->fieldMarshaledSizesCount)
	{
		if (fieldIndex == entry->fieldIndex)
			return entry->size;
		entry++;
	}

	return -1;
}

const Il2CppMethodDefinition* MetadataCache::GetMethodDefinitionFromIndex (MethodIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->methodsCount / sizeof (Il2CppMethodDefinition));
	const Il2CppMethodDefinition* methods = (const Il2CppMethodDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->methodsOffset);
	return methods + index;
}

const MethodInfo* MetadataCache::GetMethodInfoFromMethodDefinitionIndex (MethodIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->methodsCount / sizeof (Il2CppMethodDefinition));

	if (!s_MethodInfoDefinitionTable[index])
	{
		const Il2CppMethodDefinition* methodDefinition = GetMethodDefinitionFromIndex (index);
		Il2CppClass* typeInfo = GetTypeInfoFromTypeDefinitionIndex (methodDefinition->declaringType);
		Class::SetupMethods (typeInfo);
		s_MethodInfoDefinitionTable[index] = typeInfo->methods[index - typeInfo->typeDefinition->methodStart];
	}
	
	return s_MethodInfoDefinitionTable[index];
}

const Il2CppPropertyDefinition* MetadataCache::GetPropertyDefinitionFromIndex (PropertyIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->propertiesCount / sizeof (Il2CppPropertyDefinition));
	const Il2CppPropertyDefinition* properties = (const Il2CppPropertyDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->propertiesOffset);
	return properties + index;
}

const Il2CppParameterDefinition* MetadataCache::GetParameterDefinitionFromIndex (ParameterIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->parametersCount / sizeof (Il2CppParameterDefinition));
	const Il2CppParameterDefinition* parameters = (const Il2CppParameterDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->parametersOffset);
	return parameters + index;
}

int32_t MetadataCache::GetFieldOffsetFromIndex (FieldIndex index)
{
	assert (index <= s_Il2CppMetadataRegistration->fieldOffsetsCount);
	return s_Il2CppMetadataRegistration->fieldOffsets [index];
}


int32_t MetadataCache::GetReferenceAssemblyIndexIntoAssemblyTable (int32_t referencedAssemblyTableIndex)
{
	assert(referencedAssemblyTableIndex >= 0 && static_cast<uint32_t>(referencedAssemblyTableIndex) <= s_GlobalMetadataHeader->referencedAssembliesCount / sizeof(int32_t));
	const int32_t* referenceAssemblyIndicies = (const int32_t*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->referencedAssembliesOffset);
	return referenceAssemblyIndicies[referencedAssemblyTableIndex];
}

const TypeDefinitionIndex MetadataCache::GetIndexForTypeDefinition (const Il2CppClass* typeDefinition)
{
	assert(typeDefinition->typeDefinition);
	const Il2CppTypeDefinition* typeDefinitions = (const Il2CppTypeDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->typeDefinitionsOffset);

	assert (typeDefinition->typeDefinition >= typeDefinitions && typeDefinition->typeDefinition < typeDefinitions + s_GlobalMetadataHeader->typeDefinitionsCount);
	
	ptrdiff_t index = typeDefinition->typeDefinition - typeDefinitions;
	assert(index <= std::numeric_limits<TypeDefinitionIndex>::max());
	return static_cast<TypeDefinitionIndex>(index);
}

const GenericParameterIndex MetadataCache::GetIndexForGenericParameter (const Il2CppGenericParameter* genericParameter)
{
	const Il2CppGenericParameter* genericParameters = (const Il2CppGenericParameter*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->genericParametersOffset);

	assert (genericParameter >= genericParameters && genericParameter < genericParameters + s_GlobalMetadataHeader->genericParametersCount);
	
	ptrdiff_t index = genericParameter - genericParameters;
	assert(index <= std::numeric_limits<GenericParameterIndex>::max());
	return static_cast<GenericParameterIndex>(index);
}

static OnceFlag s_CustomAttributesOnceFlag;

static void InitializeCustomAttributesCaches (void* arg)
{
	s_CustomAttributesCaches = (CustomAttributesCache**)IL2CPP_CALLOC (s_Il2CppCodeRegistration->customAttributeCount, sizeof (CustomAttributesCache*));
	s_CustomAttributeTypeCaches = (CustomAttributeTypeCache**)IL2CPP_CALLOC(s_Il2CppCodeRegistration->customAttributeCount, sizeof(CustomAttributeTypeCache*));
}

CustomAttributesCache* MetadataCache::GenerateCustomAttributesCache (CustomAttributeIndex index)
{
	if (index == 0)
		return NULL;

	assert (index > 0 && index <= s_Il2CppCodeRegistration->customAttributeCount);
	assert (index > 0 && index <= static_cast<int32_t>(s_GlobalMetadataHeader->attributesInfoCount / sizeof (Il2CppCustomAttributeTypeRange)));

	CallOnce(s_CustomAttributesOnceFlag, &InitializeCustomAttributesCaches, NULL);

	// use atomics rather than a Mutex here to avoid deadlock. The attribute generators call arbitrary managed code
	CustomAttributesCache* cache = Atomic::ReadPointer(&s_CustomAttributesCaches[index]);
	if (cache == NULL)
	{
		const Il2CppCustomAttributeTypeRange* attributeTypeRange = MetadataOffset<const Il2CppCustomAttributeTypeRange*>(s_GlobalMetadata, s_GlobalMetadataHeader->attributesInfoOffset, index);

		cache = (CustomAttributesCache*)IL2CPP_CALLOC (1, sizeof(CustomAttributesCache));
		cache->count = attributeTypeRange->count;
		cache->attributes = (Il2CppObject**)GarbageCollector::AllocateFixed (sizeof(Il2CppObject *) * cache->count, 0);

		for (int32_t i = 0; i < attributeTypeRange->count; i++)
		{
			assert (attributeTypeRange->start + i < s_GlobalMetadataHeader->attributeTypesCount);
			TypeIndex typeIndex = *MetadataOffset<TypeIndex*> (s_GlobalMetadata, s_GlobalMetadataHeader->attributeTypesOffset, attributeTypeRange->start + i);
			cache->attributes[i] = il2cpp::vm::Object::New (GetTypeInfoFromTypeIndex (typeIndex));
		}

		// generated code calls the attribute constructor and sets any fields/properties
		s_Il2CppCodeRegistration->customAttributeGenerators[index](cache);

		CustomAttributesCache* original = Atomic::CompareExchangePointer(&s_CustomAttributesCaches[index], cache, (CustomAttributesCache*)NULL);
		if (original)
		{
			// A non-NULL return value indicates some other thread already generated this cache.
			// We need to cleanup the resources we allocated
			GarbageCollector::FreeFixed (cache->attributes);
			IL2CPP_FREE(cache);

			cache = original;
		}
	}

	return cache;
}

CustomAttributeTypeCache* MetadataCache::GenerateCustomAttributeTypeCache (CustomAttributeIndex index)
{
	if (index == 0)
		return NULL;

	assert (index > 0 && index <= s_Il2CppCodeRegistration->customAttributeCount);
	assert (index > 0 && index <= static_cast<int32_t>(s_GlobalMetadataHeader->attributesInfoCount / sizeof(Il2CppCustomAttributeTypeRange)));

	CallOnce (s_CustomAttributesOnceFlag, &InitializeCustomAttributesCaches, NULL);

	// use atomics rather than a Mutex here to avoid deadlock. The attribute generators call arbitrary managed code
	CustomAttributeTypeCache* cache = Atomic::ReadPointer (&s_CustomAttributeTypeCaches[index]);
	if (cache == NULL)
	{
		const Il2CppCustomAttributeTypeRange* attributeTypeRange = MetadataOffset<const Il2CppCustomAttributeTypeRange*>(s_GlobalMetadata, s_GlobalMetadataHeader->attributesInfoOffset, index);

		cache = (CustomAttributeTypeCache*)IL2CPP_CALLOC (1, sizeof (CustomAttributeTypeCache));
		cache->count = attributeTypeRange->count;
		cache->attributeTypes = (Il2CppClass**)IL2CPP_CALLOC (cache->count, sizeof(Il2CppClass*));

		for (int32_t i = 0; i < attributeTypeRange->count; i++)
		{
			assert (attributeTypeRange->start + i < s_GlobalMetadataHeader->attributeTypesCount);
			TypeIndex typeIndex = *MetadataOffset<TypeIndex*> (s_GlobalMetadata, s_GlobalMetadataHeader->attributeTypesOffset, attributeTypeRange->start + i);
			cache->attributeTypes[i] = GetTypeInfoFromTypeIndex (typeIndex);
		}

		CustomAttributeTypeCache* original = Atomic::CompareExchangePointer (&s_CustomAttributeTypeCaches[index], cache, (CustomAttributeTypeCache*)NULL);
		if (original)
		{
			// A non-NULL return value indicates some other thread already generated this cache.
			// We need to cleanup the resources we allocated
			IL2CPP_FREE (cache->attributeTypes);
			IL2CPP_FREE (cache);

			cache = original;
		}
	}

	return cache;
}

Il2CppString* MetadataCache::GetStringLiteralFromIndex (StringLiteralIndex index)
{
	if (index == kStringLiteralIndexInvalid)
		return NULL;

	assert(index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->stringLiteralCount / sizeof (Il2CppStringLiteral) && "Invalid string literal index ");

	if (s_StringLiteralTable[index])
		return s_StringLiteralTable[index];

	const Il2CppStringLiteral* stringLiteral = (const Il2CppStringLiteral*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralOffset) + index;
	s_StringLiteralTable[index] = String::NewLen ((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralDataOffset + stringLiteral->dataIndex, stringLiteral->length);

	return s_StringLiteralTable[index];
}

const char* MetadataCache::GetStringFromIndex (StringIndex index)
{
	assert (index <= s_GlobalMetadataHeader->stringCount);
	const char* strings = ((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringOffset) + index;
	return strings;
}

FieldInfo* MetadataCache::GetFieldInfoFromIndex(EncodedMethodIndex index)
{
	assert(s_GlobalMetadataHeader->fieldRefsCount >= 0 && index <= static_cast<uint32_t>(s_GlobalMetadataHeader->fieldRefsCount));

	const Il2CppFieldRef* fieldRef = MetadataOffset<const Il2CppFieldRef*>(s_GlobalMetadata, s_GlobalMetadataHeader->fieldRefsOffset, index);
	Il2CppClass* typeInfo = GetTypeInfoFromTypeIndex(fieldRef->typeIndex);
	return typeInfo->fields + fieldRef->fieldIndex;
}

void MetadataCache::InitializeMethodMetadata (uint32_t index)
{
	assert(s_GlobalMetadataHeader->metadataUsageListsCount >= 0 && index <= static_cast<uint32_t>(s_GlobalMetadataHeader->metadataUsageListsCount));

	const Il2CppMetadataUsageList* metadataUsageLists = MetadataOffset<const Il2CppMetadataUsageList*>(s_GlobalMetadata, s_GlobalMetadataHeader->metadataUsageListsOffset, index);

	uint32_t start = metadataUsageLists->start;
	uint32_t count = metadataUsageLists->count;

	for (uint32_t i = 0; i < count; i++)
	{
		uint32_t offset = start + i;
		assert(s_GlobalMetadataHeader->metadataUsagePairsCount >= 0 && offset <= static_cast<uint32_t>(s_GlobalMetadataHeader->metadataUsagePairsCount));
		const Il2CppMetadataUsagePair* metadataUsagePairs = MetadataOffset<const Il2CppMetadataUsagePair*>(s_GlobalMetadata, s_GlobalMetadataHeader->metadataUsagePairsOffset, offset);
		uint32_t destinationIndex = metadataUsagePairs->destinationIndex;
		uint32_t encodedSourceIndex = metadataUsagePairs->encodedSourceIndex;

		Il2CppMetadataUsage usage = GetEncodedIndexType (encodedSourceIndex);
		uint32_t decodedIndex = GetDecodedMethodIndex (encodedSourceIndex);
		switch (usage)
		{
		case kIl2CppMetadataUsageTypeInfo:
			*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetTypeInfoFromTypeIndex (decodedIndex);
			break;
		case kIl2CppMetadataUsageIl2CppType:
			*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = const_cast<Il2CppType*>(GetIl2CppTypeFromIndex (decodedIndex));
			break;
		case kIl2CppMetadataUsageMethodDef:
		case kIl2CppMetadataUsageMethodRef:
			*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = const_cast<MethodInfo*>(GetMethodInfoFromIndex (encodedSourceIndex));
			break;
		case kIl2CppMetadataUsageFieldInfo:
			*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetFieldInfoFromIndex(decodedIndex);
			break;
		case kIl2CppMetadataUsageStringLiteral:
			*s_Il2CppMetadataRegistration->metadataUsages[destinationIndex] = GetStringLiteralFromIndex (decodedIndex);
			break;
		default:
			NOT_IMPLEMENTED (MetadataCache::InitializeMethodMetadata);
			break;
		}
	}
}

void MetadataCache::WalkPointerTypes(WalkTypesCallback callback, void* context)
{
	os::FastAutoLock lock(&s_MetadataCache.m_CacheMutex);
	for (PointerTypeMap::iterator it = s_MetadataCache.m_PointerTypes.begin(); it != s_MetadataCache.m_PointerTypes.end(); it++)
	{
		callback(it->second, context);
	}
}
