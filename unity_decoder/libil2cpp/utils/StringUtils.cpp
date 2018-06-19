#include "il2cpp-config.h"
#include "../char-conversions.h"
#include "../object-internals.h"
#include "utils/Functional.h"
#include "utils/Memory.h"
#include "utils/StringUtils.h"
#include "utils/utf8-cpp/source/utf8/unchecked.h"
#include <stdarg.h>
#include <cassert>

namespace il2cpp
{
namespace utils
{

size_t StringUtils::Hash (const char *str)
{
	unsigned char *ustr = (unsigned char *)str;
	size_t hash = 5381;
	int c;

	while ((c = *ustr++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

std::string StringUtils::Printf(const char* format, ...)
{
	va_list argsToCheckSize;
	int n;
	std::string ret;

	va_start (argsToCheckSize, format);
#if IL2CPP_COMPILER_MSVC
	// MS vsnprintf always returns -1 if string doesn't fit, rather than
	// the needed size. Used their 'special' function instead to get required size
	n = _vscprintf_p(format, argsToCheckSize);
#else
	// use a temporary buffer as some docs indicate we cannot pass NULL to vsnprintf
	char buf[1];
	n = vsnprintf (buf, 0, format, argsToCheckSize);
#endif
	if (n == -1)
		return NULL;

	ret.resize(n+1, 0);
	va_end (argsToCheckSize);

	va_list argsToFormat;

	va_start (argsToFormat, format);
	n = vsnprintf (&ret[0], ret.size(), format, argsToFormat);
	va_end(argsToFormat);

	assert(n < (int)ret.size());

	if (n == -1)
		return NULL;

	return ret;
}

std::string StringUtils::NPrintf(const char* format, size_t max_n, ...)
{
	va_list argsToCheckSize;
	size_t n;
	std::string ret;

	va_start (argsToCheckSize, max_n);
#if IL2CPP_COMPILER_MSVC
	// MS vsnprintf always returns -1 if string doesn't fit, rather than
	// the needed size. Used their 'special' function instead to get required size
	n = _vscprintf_p(format, argsToCheckSize);
#else
	// use a temporary buffer as some docs indicate we cannot pass NULL to vsnprintf
	char buf[1];
	n = vsnprintf (buf, 0, format, argsToCheckSize);
#endif
	if (n == -1)
		return NULL;

	n = (max_n < ++n) ? max_n : n;

	ret.resize(n, 0);
	va_end (argsToCheckSize);

	va_list argsToFormat;

	va_start(argsToFormat, max_n);
	n = vsnprintf (&ret[0], n, format, argsToFormat);
	va_end (argsToFormat);

	assert(n < ret.size());

	if (n == -1)
		return NULL;

	return ret;
}

std::string StringUtils::Utf16ToUtf8(const Il2CppChar* utf16String)
{
	return Utf16ToUtf8(utf16String, -1);
}

std::string StringUtils::Utf16ToUtf8(const Il2CppChar* utf16String, int maximumSize)
{
	const Il2CppChar* ptr = utf16String;
	size_t length = 0;
	while (*ptr)
	{
		ptr++;
		length++;
		if (maximumSize != -1 && length == maximumSize)
			break;
	}

	std::string utf8String;
	utf8String.reserve(length);
	utf8::unchecked::utf16to8(utf16String, ptr, std::back_inserter(utf8String));

	return utf8String;
}

UTF16String StringUtils::Utf8ToUtf16 (const char* utf8String)
{
	return Utf8ToUtf16 (utf8String, strlen(utf8String));
}

UTF16String StringUtils::Utf8ToUtf16 (const char* utf8String, size_t length)
{
	UTF16String utf16String;

	if (utf8::is_valid(utf8String, utf8String + length))
	{
		utf16String.reserve(length);
		utf8::unchecked::utf8to16(utf8String, utf8String + length, std::back_inserter(utf16String));
	}

	return utf16String;
}

char* StringUtils::StringDuplicate (const char *strSource)
{
	char* result = NULL;

	if (!strSource)
		return NULL;

	size_t length = strlen(strSource) + 1;

	if ((result = (char*)IL2CPP_MALLOC(length)))
#if IL2CPP_COMPILER_MSVC
		strcpy_s(result, length, strSource);
#elif IL2CPP_TARGET_LINUX || IL2CPP_TARGET_TIZEN
		strncpy (result, strSource, length);
#else
		strlcpy(result, strSource, length);
#endif

	return result;
}

Il2CppChar* StringUtils::StringDuplicate (const Il2CppChar* strSource, size_t length)
{
	size_t byteLengthWithNullTerminator = sizeof (Il2CppChar) * (length + 1);
	Il2CppChar* utf16name = (Il2CppChar*)IL2CPP_MALLOC (byteLengthWithNullTerminator);
	memcpy (utf16name, strSource, byteLengthWithNullTerminator);
	
	return utf16name;
}

bool StringUtils::CaseSensitiveComparer::operator()(const std::string& left, const std::string& right) const
{
	return left == right;
}

bool StringUtils::CaseSensitiveComparer::operator()(const std::string& left, const char* right) const
{
	return left.compare(right) == 0;
}

bool StringUtils::CaseSensitiveComparer::operator()(const char* left, const std::string& right) const
{
	return right.compare(left) == 0;
}

bool StringUtils::CaseSensitiveComparer::operator()(const char* left, const char* right) const
{
	return strcmp(left, right) == 0;
}

static inline void Utf32CharToSurrogatePair(uint32_t c, Il2CppChar (&surrogatePair)[2])
{
	const Il2CppChar kLeadOffset = 55232;
	const Il2CppChar kTrailSurrogateMin = 56320;

	if (c > 0xffff)
	{
		surrogatePair[0] = static_cast<Il2CppChar>((c >> 10) + kLeadOffset);
		surrogatePair[1] = static_cast<Il2CppChar>((c & 0x3ff) + kTrailSurrogateMin);
	}
	else
	{
		surrogatePair[0] = static_cast<Il2CppChar>(c);
		surrogatePair[1] = 0;
	}
}

Il2CppChar StringUtils::Utf16ToLower(Il2CppChar c)
{
	const Il2CppChar kDataLowThreshold = 9423;
	const Il2CppChar kDataHighThreshold = 65313;

	if (c <= kDataLowThreshold)
	{
		c = ToLowerDataLow[c];
	}
	else if (c >= kDataHighThreshold)
	{
		c = ToLowerDataHigh[c - kDataHighThreshold];
	}

	return c;
}

static inline bool Utf16CharEqualsIgnoreCase(Il2CppChar left, Il2CppChar right)
{
	return StringUtils::Utf16ToLower(left) == StringUtils::Utf16ToLower(right);
}

bool StringUtils::CaseInsensitiveComparer::operator()(const std::string& left, const std::string& right) const
{
	return operator()(left.c_str(), right.c_str());
}

bool StringUtils::CaseInsensitiveComparer::operator()(const std::string& left, const char* right) const
{
	return operator()(left.c_str(), right);
}

bool StringUtils::CaseInsensitiveComparer::operator()(const char* left, const std::string& right) const
{
	return operator()(left, right.c_str());
}

bool StringUtils::CaseInsensitiveComparer::operator()(const char* left, const char* right) const
{
#if IL2CPP_DEBUG	// Invalid UTF8 strings shouldn't be passed here, so let's assert in debug mode
	assert(utf8::is_valid(left, left + strlen(left)));
	assert(utf8::is_valid(right, right + strlen(right)));
#endif

	Il2CppChar utf16Left[2];
	Il2CppChar utf16Right[2];

	while (*left && *right)
	{
		Utf32CharToSurrogatePair(utf8::unchecked::next(left), utf16Left);
		Utf32CharToSurrogatePair(utf8::unchecked::next(right), utf16Right);

		if (!Utf16CharEqualsIgnoreCase(utf16Left[0], utf16Right[0]) ||
		    !Utf16CharEqualsIgnoreCase(utf16Left[1], utf16Right[1]))
		{
			return false;
		}
	}

	return *left == '\0' && *right == '\0';
}

bool StringUtils::EndsWith(const std::string& string, const std::string& suffix)
{
	const size_t stringLength = string.length();
	const size_t suffixLength = suffix.length();

	if(suffixLength > stringLength)
		return false;

	return string.rfind(suffix.c_str(), stringLength - suffixLength, suffixLength) != std::string::npos;
}

bool StringUtils::CaseSensitiveEquals(Il2CppString* left, const char* right)
{
	std::string leftString = Utf16ToUtf8(left->chars);
	functional::Filter<const char*, StringUtils::CaseSensitiveComparer> equalsLeft(leftString.c_str());
	return equalsLeft(right);
}

bool StringUtils::CaseSensitiveEquals(const char* left, const char* right)
{
	functional::Filter<const char*, StringUtils::CaseSensitiveComparer> equalsLeft(left);
	return equalsLeft(right);
}

bool StringUtils::CaseInsensitiveEquals(Il2CppString* left, const char* right)
{
	std::string leftString = Utf16ToUtf8(left->chars);
	functional::Filter<const char*, StringUtils::CaseInsensitiveComparer> equalsLeft(leftString.c_str());
	return equalsLeft(right);
}

bool StringUtils::CaseInsensitiveEquals(const char* left, const char* right)
{
	functional::Filter<const char*, StringUtils::CaseInsensitiveComparer> equalsLeft(left);
	return equalsLeft(right);
}

} /* utils */
} /* il2cpp */
