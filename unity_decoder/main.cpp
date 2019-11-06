#include "core.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <il2cpp-metadata.h>
#include <vm/String.h>
#include <vm/MetadataCache.h>
#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstdlib>


//Define file names
const char *MetadataFileName = "./global-metadata.dat";
const char *StringLiteralFileName = "./string_literal.txt";
const char *MethodNameFileName = "./method_name.txt";

static Il2CppGlobalMetadataHeader* s_GlobalMetadataHeader;
static void* s_GlobalMetadata;


#define MAX_META_COUNT	100000
char *g_metadataUsages[MAX_META_COUNT] = { NULL };
//Il2CppMethodPointer g_methodPointers[MAX_META_COUNT] = { NULL };
Il2CppClass* g_classes[MAX_META_COUNT] = { NULL };
char *g_methodNames[MAX_META_COUNT] = { NULL };

char* removeAllChars(char* str, char c)
{
	char *pr = str, *pw = str;
	while (*pr) {
		*pw = *pr++;
		pw += (*pw != c);
	}
	*pw = '\0';

	return str;
}

const char* GetStringFromIndex(StringIndex index)
{
	assert(index <= s_GlobalMetadataHeader->stringCount);
	const char* strings = ((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringOffset) + index;
	return strings;
}

static Il2CppClass* FromTypeDefinition(TypeDefinitionIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->typeDefinitionsCount / sizeof(Il2CppTypeDefinition));
	const Il2CppTypeDefinition* typeDefinitions = (const Il2CppTypeDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->typeDefinitionsOffset);
	const Il2CppTypeDefinition* typeDefinition = typeDefinitions + index;
	//const Il2CppTypeDefinitionSizes* typeDefinitionSizes = s_Il2CppMetadataRegistration->typeDefinitionsSizes + index;
	Il2CppClass* typeInfo = (Il2CppClass*)malloc(sizeof(Il2CppClass));
	//typeInfo->image = GetImageForTypeDefinitionIndex(index);
	typeInfo->name = GetStringFromIndex(typeDefinition->nameIndex);
	typeInfo->namespaze = GetStringFromIndex(typeDefinition->namespaceIndex);
	typeInfo->customAttributeIndex = typeDefinition->customAttributeIndex;
	//typeInfo->byval_arg = MetadataCache::GetIl2CppTypeFromIndex(typeDefinition->byvalTypeIndex);
	//typeInfo->this_arg = MetadataCache::GetIl2CppTypeFromIndex(typeDefinition->byrefTypeIndex);
	typeInfo->typeDefinition = typeDefinition;
	typeInfo->genericContainerIndex = typeDefinition->genericContainerIndex;
	//typeInfo->instance_size = typeDefinitionSizes->instance_size;
	//typeInfo->actualSize = typeDefinitionSizes->instance_size; // actualySize is instance_size for compiler generated values
	//typeInfo->native_size = typeDefinitionSizes->native_size;
	//typeInfo->static_fields_size = typeDefinitionSizes->static_fields_size;
	//typeInfo->thread_static_fields_size = typeDefinitionSizes->thread_static_fields_size;
	typeInfo->thread_static_fields_offset = -1;
	typeInfo->flags = typeDefinition->flags;
	//typeInfo->valuetype = (typeDefinition->bitfield >> (kBitIsValueType - 1)) & 0x1;
	//typeInfo->enumtype = (typeDefinition->bitfield >> (kBitIsEnum - 1)) & 0x1;
	typeInfo->is_generic = typeDefinition->genericContainerIndex != kGenericContainerIndexInvalid; // generic if we have a generic container
	//typeInfo->has_finalize = (typeDefinition->bitfield >> (kBitHasFinalizer - 1)) & 0x1;
	//typeInfo->has_cctor = (typeDefinition->bitfield >> (kBitHasStaticConstructor - 1)) & 0x1;
	//typeInfo->is_blittable = (typeDefinition->bitfield >> (kBitIsBlittable - 1)) & 0x1;
	//typeInfo->is_import_or_windows_runtime = (typeDefinition->bitfield >> (kBitIsImportOrWindowsRuntime - 1)) & 0x1;
	//typeInfo->packingSize = ConvertPackingSizeEnumToValue(static_cast<PackingSize>((typeDefinition->bitfield >> (kPackingSize - 1)) & 0xF));
	typeInfo->method_count = typeDefinition->method_count;
	typeInfo->property_count = typeDefinition->property_count;
	typeInfo->field_count = typeDefinition->field_count;
	typeInfo->event_count = typeDefinition->event_count;
	typeInfo->nested_type_count = typeDefinition->nested_type_count;
	typeInfo->vtable_count = typeDefinition->vtable_count;
	typeInfo->interfaces_count = typeDefinition->interfaces_count;
	typeInfo->interface_offsets_count = typeDefinition->interface_offsets_count;
	typeInfo->token = typeDefinition->token;

	/*
	if (typeDefinition->parentIndex != kTypeIndexInvalid)
		typeInfo->parent = Class::FromIl2CppType(MetadataCache::GetIl2CppTypeFromIndex(typeDefinition->parentIndex));

	if (typeDefinition->declaringTypeIndex != kTypeIndexInvalid)
		typeInfo->declaringType = Class::FromIl2CppType(MetadataCache::GetIl2CppTypeFromIndex(typeDefinition->declaringTypeIndex));

	typeInfo->castClass = typeInfo->element_class = typeInfo;
	if (typeInfo->enumtype)
		typeInfo->castClass = typeInfo->element_class = Class::FromIl2CppType(MetadataCache::GetIl2CppTypeFromIndex(typeDefinition->elementTypeIndex));
	*/

	return typeInfo;
}

const Il2CppMethodDefinition* GetMethodDefinitionFromIndex(MethodIndex index)
{
	assert(index >= 0 && static_cast<uint32_t>(index) <= s_GlobalMetadataHeader->methodsCount / sizeof(Il2CppMethodDefinition));
	const Il2CppMethodDefinition* methods = (const Il2CppMethodDefinition*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->methodsOffset);
	return methods + index;
}


void SetupMethods(Il2CppClass *klass)
{

	if (klass->method_count == 0)
	{
		klass->methods = NULL;
		return;
	}

	klass->methods = (const MethodInfo**)malloc(klass->method_count * sizeof(MethodInfo*));
	MethodInfo* methods = (MethodInfo*)malloc(klass->method_count * sizeof(MethodInfo));
	memset(methods, 0, klass->method_count * sizeof(MethodInfo));
	MethodInfo* newMethod = methods;

	MethodIndex start = klass->typeDefinition->methodStart;
	assert(start != kFieldIndexInvalid);
	MethodIndex end = start + klass->method_count;

	for (MethodIndex index = start; index < end; ++index)
	{
		const Il2CppMethodDefinition* methodDefinition = GetMethodDefinitionFromIndex(index);

		newMethod->name = GetStringFromIndex(methodDefinition->nameIndex);
		//newMethod->methodPointer = GetMethodPointerFromIndex(methodDefinition->methodIndex);
		//newMethod->invoker_method = GetMethodInvokerFromIndex(methodDefinition->invokerIndex);
		newMethod->declaring_type = klass;
		//newMethod->return_type = GetIl2CppTypeFromIndex(methodDefinition->returnType);

		/*
		ParameterInfo* parameters = (ParameterInfo*)malloc(methodDefinition->parameterCount * sizeof(ParameterInfo));
		ParameterInfo* newParameter = parameters;
		for (uint16_t paramIndex = 0; paramIndex < methodDefinition->parameterCount; ++paramIndex)
		{
			const Il2CppParameterDefinition* parameterDefinition = GetParameterDefinitionFromIndex(methodDefinition->parameterStart + paramIndex);
			newParameter->name = GetStringFromIndex(parameterDefinition->nameIndex);
			newParameter->position = paramIndex;
			newParameter->token = parameterDefinition->token;
			newParameter->customAttributeIndex = parameterDefinition->customAttributeIndex;
			newParameter->parameter_type = GetIl2CppTypeFromIndex(parameterDefinition->typeIndex);

			newParameter++;
		}
		newMethod->parameters = parameters;

		newMethod->customAttributeIndex = methodDefinition->customAttributeIndex;
		newMethod->flags = methodDefinition->flags;
		newMethod->iflags = methodDefinition->iflags;
		newMethod->slot = methodDefinition->slot;
		newMethod->parameters_count = static_cast<const uint8_t>(methodDefinition->parameterCount);
		newMethod->is_inflated = false;
		newMethod->token = methodDefinition->token;
		newMethod->methodDefinition = methodDefinition;
		newMethod->genericContainer = GetGenericContainerFromIndex(methodDefinition->genericContainerIndex);
		*/

		if (newMethod->genericContainer)
			newMethod->is_generic = true;


		klass->methods[index - start] = newMethod;

		if (methodDefinition->methodIndex >= 0 && newMethod->name != NULL) {
			int totalLen = strlen(klass->name) + strlen(newMethod->name) + 3;
			char *name = new char[totalLen];
			snprintf(name, totalLen, "%s$$%s\0", klass->name, newMethod->name);
			g_methodNames[methodDefinition->methodIndex] = name;
		}

		newMethod++;
		
	}

}


Il2CppClass* GetTypeInfoFromTypeDefinitionIndex(TypeDefinitionIndex index)
{
	if (index == kTypeIndexInvalid)
		return NULL;

	assert(index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->typeDefinitionsCount / sizeof(Il2CppTypeDefinition));

	return FromTypeDefinition(index);
}

char* GetStringLiteralFromIndex(StringLiteralIndex index)
{
	
	assert(index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->stringLiteralCount / sizeof(Il2CppStringLiteral) && "Invalid string literal index ");

	const Il2CppStringLiteral* stringLiteral = (const Il2CppStringLiteral*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralOffset) + index;
	
	const char* srcStr = (const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralDataOffset + stringLiteral->dataIndex;
	
	char *dstStr = new char[stringLiteral->length+1];
	snprintf(dstStr, stringLiteral->length+1, "%s", srcStr);

	dstStr = removeAllChars(dstStr, '\r');
	dstStr = removeAllChars(dstStr, '\n');

	return dstStr;
}


template <typename T>
static T MetadataOffset(void* metadata, size_t sectionOffset, size_t itemIndex)
{
	return reinterpret_cast<T> (reinterpret_cast<uint8_t*> (metadata) + sectionOffset) + itemIndex;
}


void InitializeMethodMetadata(uint32_t index)
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

		Il2CppMetadataUsage usage = GetEncodedIndexType(encodedSourceIndex);
		uint32_t decodedIndex = GetDecodedMethodIndex(encodedSourceIndex);
		switch (usage)
		{
		case kIl2CppMetadataUsageTypeInfo:
			//metadataUsages[destinationIndex] = GetTypeInfoFromTypeIndex(decodedIndex);
			break;
		case kIl2CppMetadataUsageIl2CppType:
			//metadataUsages[destinationIndex] = const_cast<Il2CppType*>(GetIl2CppTypeFromIndex(decodedIndex));
			//break;
		case kIl2CppMetadataUsageMethodDef:
			//metadataUsages_method[destinationIndex] = const_cast<MethodInfo*>(GetMethodInfoFromMethodDefinitionIndex(encodedSourceIndex));
			break;
		case kIl2CppMetadataUsageMethodRef:
			//metadataUsages_method[destinationIndex] = const_cast<MethodInfo*>(GetMethodInfoFromMethodDefinitionIndex(encodedSourceIndex));
			//metadataUsages[destinationIndex] = const_cast<char*>(GetMethodInfoFromIndex(encodedSourceIndex));
			break;
		case kIl2CppMetadataUsageFieldInfo:
			//metadataUsages[destinationIndex] = GetFieldInfoFromIndex(decodedIndex);
			break;
		case kIl2CppMetadataUsageStringLiteral:
			g_metadataUsages[destinationIndex] = GetStringLiteralFromIndex(decodedIndex);
			break;
		default:
			//std::cout << "not implemented" << std::endl;
			break;
		}
	}
}


int main()
{

	std::ofstream stringLiteralFile, methodNameFile;
	std::ifstream metadataFile;
	
	metadataFile.open(MetadataFileName, std::ios::binary | std::ios::ate);
	std::streamsize size = metadataFile.tellg();
	metadataFile.seekg(0, std::ios::beg);
	char *addr = (char*)malloc(size + 1);
	metadataFile.read(&addr[0], size);
	addr[size] = 0;

	s_GlobalMetadata = addr;
	s_GlobalMetadataHeader = (Il2CppGlobalMetadataHeader*)addr;


	std::cout << "metadata version is: " << s_GlobalMetadataHeader->version << std::endl;


	assert(s_GlobalMetadataHeader->sanity == 0xFAB11BAF);
	assert(s_GlobalMetadataHeader->version == 24);



	stringLiteralFile.open(StringLiteralFileName);
	int metholdUsageListsCount = s_GlobalMetadataHeader->metadataUsageListsCount / sizeof(Il2CppMetadataUsageList);
	for (int i = 0; i < metholdUsageListsCount; i++) {
		InitializeMethodMetadata(i);
	}


	int strCount = s_GlobalMetadataHeader->stringLiteralCount / sizeof(Il2CppStringLiteral);
	
	/*
	for (int i = MAX_META_COUNT - 1 ; i >= 0 ; i--) {
		if (g_metadataUsages[i] != NULL) {
			stringLiteralFile << g_metadataUsages[i] << std::endl;
		}
	}
	*/
	int skip = 0;
	int i = 0;
	for (i = 0; i < MAX_META_COUNT; i++) {
		if (g_metadataUsages[i] == NULL) {
			skip++;
		}
		else {
			break;
		}
	}


	int totalStrNum = skip + strCount;

	stringLiteralFile << totalStrNum << std::endl << strCount << std::endl;
	for (; i < MAX_META_COUNT; i++) {
		if (g_metadataUsages[i] == NULL) {
			break;
		}
		stringLiteralFile << g_metadataUsages[i] << std::endl;
	}
	
	
	stringLiteralFile.close();

	methodNameFile.open(MethodNameFileName);

	int typeCount = s_GlobalMetadataHeader->typeDefinitionsCount / sizeof(Il2CppTypeDefinition);
	for (int i = 0; i < typeCount; i++) {
		g_classes[i] = GetTypeInfoFromTypeDefinitionIndex(i);
		SetupMethods(g_classes[i]);
	}

	int methodCount = 0;
	for (int i = 0; i < MAX_META_COUNT; i++) {
		if (g_methodNames[i] != NULL) {
			methodCount++;
		}
	}

	methodNameFile << methodCount << std::endl;
	for (int i = 0; i < MAX_META_COUNT; i++) {
		if (g_methodNames[i] != NULL) {
			methodNameFile << g_methodNames[i] << std::endl;
		}
	}
	methodNameFile.close();

	return 0;
}


