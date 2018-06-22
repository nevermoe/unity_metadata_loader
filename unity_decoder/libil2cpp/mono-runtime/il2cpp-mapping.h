#pragma once
#include "il2cpp-metadata.h"

struct MonoMethodInfo
{
    int64_t hash;
    int32_t invoker_index;
    int32_t method_pointer_index;
};

struct MonoRGCTXDefinition
{
    Il2CppRGCTXDataType type;
    AssemblyIndex assemblyIndex;
    int32_t token;
    int32_t generic_parameter_index;
};

struct RuntimeGenericContextInfo
{
    uint64_t hash;
    int32_t rgctxStart;
    int32_t rgctxCount;
};

struct MonoMetadataToken
{
    AssemblyIndex assemblyIndex;
    int32_t token;
};

#pragma pack(push, p1,4)
struct MonoMethodMetadata
{
    MonoMetadataToken metadataToken;
    MethodIndex reversePInvokeWrapperIndex;
    uint64_t hash;
};
#pragma pack(pop, p1)

#pragma pack(push, p1,4)
struct MonoClassMetadata
{
    MonoMetadataToken metadataToken;
    int32_t genericParametersOffset;
    int32_t genericParametersCount;
    int32_t isPointer;
    int32_t rank; //if rank == 0, the token is for a non-array type, otherwise the rank is valid and the token represents the element type of the array
    int32_t elementTypeIndex;
};
#pragma pack(pop, p1)

struct MonoFieldMetadata
{
    TypeIndex parentTypeIndex;
    int32_t token;
};

struct MonoGenericInstMetadata
{
    uint32_t type_argc;
    const TypeIndex *type_argv_indices;
};
