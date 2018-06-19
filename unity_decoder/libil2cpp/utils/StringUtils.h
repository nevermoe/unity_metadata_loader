#pragma once
#include "types.h"
#include <string>
#include <vector>
#include <stdint.h>
#include "il2cpp-config.h"

namespace il2cpp
{
namespace utils
{

class LIBIL2CPP_CODEGEN_API StringUtils
{
public:
	static size_t Hash (const char *str);
	static std::string Printf (const char* format, ...);
	static std::string NPrintf (const char* format, size_t max_n, ...);
	static std::string Utf16ToUtf8 (const Il2CppChar* utf16String);
	static std::string Utf16ToUtf8(const Il2CppChar* utf16String, int maximumSize);
	static UTF16String Utf8ToUtf16(const char* utf8String);
	static UTF16String Utf8ToUtf16 (const char* utf8String, size_t length);
	static char* StringDuplicate (const char *strSource);
	static Il2CppChar* StringDuplicate (const Il2CppChar* strSource, size_t length);
	static bool EndsWith(const std::string& string, const std::string& suffix);
	static Il2CppChar Utf16ToLower(Il2CppChar c);
	static bool CaseSensitiveEquals(Il2CppString* left, const char* right);
	static bool CaseSensitiveEquals(const char* left, const char* right);
	static bool CaseInsensitiveEquals(Il2CppString* left, const char* right);
	static bool CaseInsensitiveEquals(const char* left, const char* right);

#if IL2CPP_PLATFORM_WIN32
	static inline std::string NativeStringToUtf8(const Il2CppNativeString& nativeStr)
	{
		assert(nativeStr.length() < static_cast<size_t>(std::numeric_limits<int>::max()));
		return Utf16ToUtf8(nativeStr.c_str(), static_cast<int>(nativeStr.length()));
	}
#else
	static inline std::string NativeStringToUtf8(Il2CppNativeString nativeStr)
	{
		return nativeStr;
	}
#endif

	template <typename CharType, size_t N>
	static inline size_t LiteralLength(const CharType (&str)[N])
	{
		return N - 1;
	}

	template <typename CharType>
	static size_t StrLen(const CharType* str)
	{
		size_t length = 0;
		while (*str)
		{
			str++;
			length++;
		}

		return length;
	}

	struct CaseSensitiveComparer
	{
		bool operator()(const std::string& left, const std::string& right) const;
		bool operator()(const std::string& left, const char* right) const;
		bool operator()(const char* left, const std::string& right) const;
		bool operator()(const char* left, const char* right) const;
	};

	struct CaseInsensitiveComparer
	{
		bool operator()(const std::string& left, const std::string& right) const;
		bool operator()(const std::string& left, const char* right) const;
		bool operator()(const char* left, const std::string& right) const;
		bool operator()(const char* left, const char* right) const;
	};
};


} /* utils */
} /* il2cpp */
