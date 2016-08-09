#include "core.h"
#include <iostream>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>
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
#include <windows.h>
#include <fstream>

using namespace boost::interprocess;


//Define file names
//const char *FileName = "D://vs_project/global-metadata.dat_ios";
const char *FileName = "./global-metadata.dat";
const char *OutputFileName = "./string_literal.txt";
static Il2CppGlobalMetadataHeader* s_GlobalMetadataHeader;
static void* s_GlobalMetadata;

typedef Il2CppString* (*function1_ptr) (const char*, unsigned int);
function1_ptr func;

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

char *replaceAllReturns(char* str)
{
	char *pw = str;
	while (*pw) {
		if (*pw == '\r' || *pw == '\n')
			*pw = '^';
		pw++;
	}

	return str;
}

char* GetStringLiteralFromIndex(StringLiteralIndex index)
{
	
	assert(index >= 0 && static_cast<uint32_t>(index) < s_GlobalMetadataHeader->stringLiteralCount / sizeof(Il2CppStringLiteral) && "Invalid string literal index ");

	const Il2CppStringLiteral* stringLiteral = (const Il2CppStringLiteral*)((const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralOffset) + index;
	
	const char* srcStr = (const char*)s_GlobalMetadata + s_GlobalMetadataHeader->stringLiteralDataOffset + stringLiteral->dataIndex;
	
	char *dstStr = new char[stringLiteral->length+1];
	snprintf(dstStr, stringLiteral->length+1, "%s", srcStr);

	dstStr = replaceAllReturns(dstStr);

	return dstStr;
}


template <typename T>
static T MetadataOffset(void* metadata, size_t sectionOffset, size_t itemIndex)
{
	return reinterpret_cast<T> (reinterpret_cast<uint8_t*> (metadata) + sectionOffset) + itemIndex;
}

#define MAX_META_COUNT	100000
char *metadataUsages[MAX_META_COUNT] = {NULL};

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
		
		case kIl2CppMetadataUsageStringLiteral:
			metadataUsages[destinationIndex] = GetStringLiteralFromIndex(decodedIndex);
			break;
		default:
			//std::cout << "not implemented" << std::endl;
			break;
		}
	}
}


int main()
{

	std::ofstream stringLiteralFile;
	//stringLiteralFile.open("D://vs_project/strlit.txt");
	stringLiteralFile.open(OutputFileName);
	
	
	file_mapping m_file(FileName, boost::interprocess::read_only);

	mapped_region region(m_file, boost::interprocess::read_only);

	//Get the address of the mapped region
	void * addr = region.get_address();
	s_GlobalMetadata = addr;
	std::size_t size = region.get_size();


	s_GlobalMetadataHeader = (Il2CppGlobalMetadataHeader*)addr;

	assert(s_GlobalMetadataHeader->sanity == 0xFAB11BAF);
	assert(s_GlobalMetadataHeader->version == 21);

	int usagePairCount = s_GlobalMetadataHeader->metadataUsageListsCount / sizeof(Il2CppMetadataUsagePair);
	for (int i = 0; i < usagePairCount; i++) {
		InitializeMethodMetadata(i);
	}

	int strCount = s_GlobalMetadataHeader->stringLiteralCount / sizeof(Il2CppStringLiteral);
	stringLiteralFile << strCount << std::endl;
	for (int i = MAX_META_COUNT - 1 ; i >= 0 ; i--) {
		if (metadataUsages[i] != NULL) {
			stringLiteralFile << metadataUsages[i] << std::endl;
		}
	}

	
	/*
	int strCount = s_GlobalMetadataHeader->stringLiteralCount / sizeof(Il2CppStringLiteral);
	stringLiteralFile << strCount << std::endl;

	for (int i = strCount - 1; i >= 0; i--) {
	//for (int i = 0; i < strCount; i++) {
		char* str = GetStringLiteralFromIndex(i);
		stringLiteralFile << i << ": " << str << std::endl;
		delete str;
	}
	*/

	stringLiteralFile.close();



	return 0;
}


