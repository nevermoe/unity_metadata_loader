#pragma once

#include "il2cpp-config.h"
#include <stdint.h>

// This file contains the structures specifying how we store converted metadata.
// These structures have 3 constraints:
// 1. These structures will be stored in an external file, and as such must not contain any pointers.
//    All references to other metadata should occur via an index into a corresponding table.
// 2. These structures are assumed to be const. Either const structures in the binary or mapped as
//    readonly memory from an external file. Do not add any 'calculated' fields which will be written to at runtime.
// 3. These structures should be optimized for size. Other structures are used at runtime which can
//    be larger to store cached information

typedef int32_t TypeIndex;
typedef int32_t TypeDefinitionIndex;
typedef int32_t FieldIndex;
typedef int32_t DefaultValueIndex;
typedef int32_t DefaultValueDataIndex;
typedef int32_t CustomAttributeIndex;
typedef int32_t ParameterIndex;
typedef int32_t MethodIndex;
typedef int32_t GenericMethodIndex;
typedef int32_t PropertyIndex;
typedef int32_t EventIndex;
typedef int32_t GenericContainerIndex;
typedef int32_t GenericParameterIndex;
typedef int16_t GenericParameterConstraintIndex;
typedef int32_t NestedTypeIndex;
typedef int32_t InterfacesIndex;
typedef int32_t VTableIndex;
typedef int32_t InterfaceOffsetIndex;
typedef int32_t RGCTXIndex;
typedef int32_t StringIndex;
typedef int32_t StringLiteralIndex;
typedef int32_t GenericInstIndex;
typedef int32_t ImageIndex;
typedef int32_t AssemblyIndex;
typedef int32_t GuidIndex;

const TypeIndex kTypeIndexInvalid = -1;
const TypeDefinitionIndex kTypeDefinitionIndexInvalid = -1;
const DefaultValueDataIndex kDefaultValueIndexNull = -1;
const EventIndex kEventIndexInvalid = -1;
const FieldIndex kFieldIndexInvalid = -1;
const MethodIndex kMethodIndexInvalid = -1;
const PropertyIndex kPropertyIndexInvalid = -1;
const GenericContainerIndex kGenericContainerIndexInvalid = -1;
const GenericParameterIndex kGenericParameterIndexInvalid = -1;
const RGCTXIndex kRGCTXIndexInvalid = -1;
const StringLiteralIndex kStringLiteralIndexInvalid = -1;
const GuidIndex kGuidIndexInvalid = -1;

// Encoded index (1 bit)
// MethodDef - 0
// MethodSpec - 1
// We use the top 3 bits to indicate what table to index into
// Type              Binary            Hex
// Il2CppClass          001               0x20000000
// Il2CppType        010               0x40000000
// MethodInfo        011               0x60000000
// FieldInfo         100               0x80000000
// StringLiteral     101               0xA0000000
// MethodRef         110               0xC0000000

typedef uint32_t EncodedMethodIndex;

enum Il2CppMetadataUsage
{
	kIl2CppMetadataUsageInvalid,
	kIl2CppMetadataUsageTypeInfo,
	kIl2CppMetadataUsageIl2CppType,
	kIl2CppMetadataUsageMethodDef,
	kIl2CppMetadataUsageFieldInfo,
	kIl2CppMetadataUsageStringLiteral,
	kIl2CppMetadataUsageMethodRef,
};

static inline Il2CppMetadataUsage GetEncodedIndexType (EncodedMethodIndex index)
{
	return (Il2CppMetadataUsage)((index & 0xE0000000) >> 29);
}

static inline uint32_t GetDecodedMethodIndex (EncodedMethodIndex index)
{
	return index & 0x1FFFFFFFU;
}

struct Il2CppImage;
struct Il2CppType;
struct Il2CppTypeDefinitionMetadata;

union Il2CppRGCTXDefinitionData
{
	int32_t rgctxDataDummy;
	MethodIndex methodIndex;
	TypeIndex typeIndex;
};

enum Il2CppRGCTXDataType
{
	IL2CPP_RGCTX_DATA_INVALID,
	IL2CPP_RGCTX_DATA_TYPE,
	IL2CPP_RGCTX_DATA_CLASS,
	IL2CPP_RGCTX_DATA_METHOD
};

struct Il2CppRGCTXDefinition
{
	Il2CppRGCTXDataType type;
	Il2CppRGCTXDefinitionData data;
};

struct Il2CppInterfaceOffsetPair
{
	TypeIndex interfaceTypeIndex;
	int32_t offset;
};

struct Il2CppTypeDefinition
{
	StringIndex nameIndex;
	StringIndex namespaceIndex;
	CustomAttributeIndex customAttributeIndex;
	TypeIndex byvalTypeIndex;
	TypeIndex byrefTypeIndex;
	
	TypeIndex declaringTypeIndex;
	TypeIndex parentIndex;
	TypeIndex elementTypeIndex; // we can probably remove this one. Only used for enums
	
	RGCTXIndex rgctxStartIndex;
	int32_t rgctxCount;

	GenericContainerIndex genericContainerIndex;

	MethodIndex reversePInvokeWrapperIndex;
	int32_t marshalingFunctionsIndex;
	int32_t ccwFunctionIndex;
	GuidIndex guidIndex;

	uint32_t flags;
	
	FieldIndex fieldStart;
	MethodIndex methodStart;
	EventIndex eventStart;
	PropertyIndex propertyStart;
	NestedTypeIndex nestedTypesStart;
	InterfacesIndex interfacesStart;
	VTableIndex vtableStart;
	InterfacesIndex interfaceOffsetsStart;

	uint16_t method_count;
	uint16_t property_count;
	uint16_t field_count;
	uint16_t event_count;
	uint16_t nested_type_count;
	uint16_t vtable_count;
	uint16_t interfaces_count;
	uint16_t interface_offsets_count;

	// bitfield to portably encode boolean values as single bits
	// 01 - valuetype;
	// 02 - enumtype;
	// 03 - has_finalize;
	// 04 - has_cctor;
	// 05 - is_blittable;
	// 06 - is_import_or_windows_runtime;
	// 07-10 - One of nine possible PackingSize values (0, 1, 2, 4, 8, 16, 32, 64, or 128)
	uint32_t bitfield;
	uint32_t token;
};

struct Il2CppFieldDefinition
{
	StringIndex nameIndex;
	TypeIndex typeIndex;
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct Il2CppFieldDefaultValue
{
	FieldIndex fieldIndex;
	TypeIndex typeIndex;
	DefaultValueDataIndex dataIndex;
};

struct Il2CppFieldMarshaledSize
{
	FieldIndex fieldIndex;
	TypeIndex typeIndex;
	int32_t size;
};

struct Il2CppFieldRef
{
	TypeIndex typeIndex;
	FieldIndex fieldIndex; // local offset into type fields
};

struct Il2CppParameterDefinition
{
	StringIndex nameIndex;
	uint32_t token;
	CustomAttributeIndex customAttributeIndex;
	TypeIndex typeIndex;
};

struct Il2CppParameterDefaultValue
{
	ParameterIndex parameterIndex;
	TypeIndex typeIndex;
	DefaultValueDataIndex dataIndex;
};

struct Il2CppMethodDefinition
{
	StringIndex nameIndex;
	TypeDefinitionIndex declaringType;
	TypeIndex returnType;
	ParameterIndex parameterStart;
	CustomAttributeIndex customAttributeIndex;
	GenericContainerIndex genericContainerIndex;
	MethodIndex methodIndex;
	MethodIndex invokerIndex;
	MethodIndex reversePInvokeWrapperIndex;
	RGCTXIndex rgctxStartIndex;
	int32_t rgctxCount;
	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint16_t parameterCount;
};

struct Il2CppEventDefinition
{
	StringIndex nameIndex;
	TypeIndex typeIndex;
	MethodIndex add;
	MethodIndex remove;
	MethodIndex raise;
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct Il2CppPropertyDefinition
{
	StringIndex nameIndex;
	MethodIndex get;
	MethodIndex set;
	uint32_t attrs;
	CustomAttributeIndex customAttributeIndex;
	uint32_t token;
};

struct Il2CppMethodSpec
{
	MethodIndex methodDefinitionIndex;
	GenericInstIndex classIndexIndex;
	GenericInstIndex methodIndexIndex;
};

struct Il2CppStringLiteral
{
	uint32_t length;
	StringLiteralIndex dataIndex;
};

struct Il2CppGenericMethodIndices
{
	MethodIndex methodIndex;
	MethodIndex invokerIndex;
};

struct Il2CppGenericMethodFunctionsDefinitions
{
	GenericMethodIndex genericMethodIndex;
	Il2CppGenericMethodIndices indices;
};

const int kPublicKeyByteLength = 8;

struct Il2CppAssemblyName
{
	StringIndex nameIndex;
	StringIndex cultureIndex;
	StringIndex hashValueIndex;
	StringIndex publicKeyIndex;
	uint32_t hash_alg;
	int32_t hash_len;
	uint32_t flags;
	int32_t major;
	int32_t minor;
	int32_t build;
	int32_t revision;
	uint8_t publicKeyToken[kPublicKeyByteLength];
};

struct Il2CppImageDefinition
{
	StringIndex nameIndex;
	AssemblyIndex assemblyIndex;

	TypeDefinitionIndex typeStart;
	uint32_t typeCount;

	MethodIndex entryPointIndex;
	uint32_t token;
};

struct Il2CppAssembly
{
	ImageIndex imageIndex;
	CustomAttributeIndex customAttributeIndex;
	int32_t referencedAssemblyStart;
	int32_t referencedAssemblyCount;
	Il2CppAssemblyName aname;
};

struct Il2CppMetadataUsageList
{
	uint32_t start;
	uint32_t count;
};

struct Il2CppMetadataUsagePair
{
	uint32_t destinationIndex;
	uint32_t encodedSourceIndex;
};

struct Il2CppCustomAttributeTypeRange
{
	int32_t start;
	int32_t count;
};

#pragma pack(push, p1,4)
struct Il2CppGlobalMetadataHeader
{
	int32_t sanity;
	int32_t version;
	int32_t stringLiteralOffset; // string data for managed code
	int32_t stringLiteralCount;
	int32_t stringLiteralDataOffset;
	int32_t stringLiteralDataCount;
	int32_t stringOffset; // string data for metadata
	int32_t stringCount;
	int32_t eventsOffset; // Il2CppEventDefinition
	int32_t eventsCount;
	int32_t propertiesOffset; // Il2CppPropertyDefinition
	int32_t propertiesCount;
	int32_t methodsOffset; // Il2CppMethodDefinition
	int32_t methodsCount;
	int32_t parameterDefaultValuesOffset; // Il2CppParameterDefaultValue
	int32_t parameterDefaultValuesCount;
	int32_t fieldDefaultValuesOffset; // Il2CppFieldDefaultValue
	int32_t fieldDefaultValuesCount;
	int32_t fieldAndParameterDefaultValueDataOffset; // uint8_t
	int32_t fieldAndParameterDefaultValueDataCount;
	int32_t fieldMarshaledSizesOffset; // Il2CppFieldMarshaledSize
	int32_t fieldMarshaledSizesCount;
	int32_t parametersOffset; // Il2CppParameterDefinition
	int32_t parametersCount;
	int32_t fieldsOffset; // Il2CppFieldDefinition
	int32_t fieldsCount;
	int32_t genericParametersOffset; // Il2CppGenericParameter
	int32_t genericParametersCount;
	int32_t genericParameterConstraintsOffset; // TypeIndex
	int32_t genericParameterConstraintsCount;
	int32_t genericContainersOffset; // Il2CppGenericContainer
	int32_t genericContainersCount;
	int32_t nestedTypesOffset; // TypeDefinitionIndex
	int32_t nestedTypesCount;
	int32_t interfacesOffset; // TypeIndex
	int32_t interfacesCount;
	int32_t vtableMethodsOffset; // EncodedMethodIndex
	int32_t vtableMethodsCount;
	int32_t interfaceOffsetsOffset; // Il2CppInterfaceOffsetPair
	int32_t interfaceOffsetsCount;
	int32_t typeDefinitionsOffset; // Il2CppTypeDefinition
	int32_t typeDefinitionsCount;
	int32_t rgctxEntriesOffset; // Il2CppRGCTXDefinition
	int32_t rgctxEntriesCount;
	int32_t imagesOffset; // Il2CppImageDefinition
	int32_t imagesCount;
	int32_t assembliesOffset; // Il2CppAssemblyDefinition
	int32_t assembliesCount;
	int32_t metadataUsageListsOffset; // Il2CppMetadataUsageList
	int32_t metadataUsageListsCount;
	int32_t metadataUsagePairsOffset; // Il2CppMetadataUsagePair
	int32_t metadataUsagePairsCount;
	int32_t fieldRefsOffset; // Il2CppFieldRef
	int32_t fieldRefsCount;
	int32_t referencedAssembliesOffset; // int32_t
	int32_t referencedAssembliesCount;
	int32_t attributesInfoOffset; // Il2CppCustomAttributeTypeRange
	int32_t attributesInfoCount;
	int32_t attributeTypesOffset; // TypeIndex
	int32_t attributeTypesCount;
};
#pragma pack(pop, p1)
