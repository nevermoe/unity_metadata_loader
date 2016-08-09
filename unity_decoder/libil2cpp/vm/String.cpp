#include "il2cpp-config.h"
#include "gc/Allocator.h"
#include "gc/GCHandle.h"
#include "os/Mutex.h"
#include "vm/Exception.h"
#include "vm/String.h"
#include "vm/Object.h"
#include "vm/Profiler.h"
#include "utils/StdUnorderedMap.h"
#include "utils/StringUtils.h"
#include <string>
#include <memory.h>
#include <cassert>
#include "class-internals.h"
#include "object-internals.h"

namespace il2cpp
{
namespace vm
{

int32_t String::GetLength (Il2CppString* str)
{
	return str->length;
}

Il2CppChar* String::GetChars (Il2CppString* str)
{
	return str->chars;
}

int32_t String::GetHash (Il2CppString* str)
{
	const Il2CppChar* p = GetChars (str);
	int i, len = GetLength (str);
	uint32_t h = 0;

	for (i = 0; i < len; i++) {
		h = (h << 5) - h + *p;
		p++;
	}

	return h;
}

Il2CppString* String::New (const char* str)
{
	return NewLen (str, (uint32_t)strlen(str));
}

Il2CppString* String::NewWrapper (const char* str)
{
	return New (str);
}

Il2CppString* String::NewLen (const char* str, uint32_t length)
{
	UTF16String utf16Chars = il2cpp::utils::StringUtils::Utf8ToUtf16 (str, length);
	
	return NewUtf16 (utf16Chars.c_str(), (uint32_t)utf16Chars.length ());
}

Il2CppString* String::NewUtf16 (const Il2CppChar* text, int32_t len)
{
	Il2CppString *s;
	
	s = NewSize (len);
	assert (s != NULL);

	memcpy (String::GetChars (s), text, len * 2);

	return s;
}

Il2CppString* String::NewSize (int32_t len)
{
	Il2CppString *s;
	assert(len >= 0);
	size_t size = (sizeof (Il2CppString) + ((len + 1) * 2));

	/* overflow ? can't fit it, can't allocate it! */
	if (static_cast<uint32_t>(len) > size)
		Exception::RaiseOutOfMemoryException ();

	s = reinterpret_cast<Il2CppString*> (Object::AllocatePtrFree (size, il2cpp_defaults.string_class));

	s->length = len;
#if NEED_TO_ZERO_PTRFREE
	s->chars [len] = 0;
#endif

#if IL2CPP_ENABLE_PROFILER
	if (Profiler::ProfileAllocations ())
		Profiler::Allocation ((Il2CppObject*)s, il2cpp_defaults.string_class);
#endif

	return s;
}

struct InternedString
{
	int32_t length;
	const Il2CppChar* chars;
};

class InternedStringHash
{
public:
	size_t operator( )( const InternedString& ea ) const
	{
		return ea.length;
	}
};

class InternedStringCompare
{
public:
	bool operator()(const InternedString& ea, const InternedString& eb) const
	{
		return (ea.length == eb.length) && (0 == memcmp (ea.chars, eb.chars, sizeof(Il2CppChar)*ea.length));
	}
};

class InternedStringLess
{
public:
	bool operator()(const InternedString& ea, const InternedString& eb) const
	{
		if (ea.length < eb.length)
			return true;
		return memcmp (ea.chars, eb.chars, sizeof(Il2CppChar)*eb.length) < 0;
	}
};

typedef unordered_map<
	InternedString,
	Il2CppString*,
#if IL2CPP_HAS_UNORDERED_CONTAINER
	InternedStringHash,
	InternedStringCompare,
#else
	InternedStringLess,
#endif
	il2cpp::gc::Allocator<std::pair<const InternedString, Il2CppString*> > > InternedStringMap;

static os::FastMutex s_InternedStringMapMutex;
static InternedStringMap* s_InternedStringMap;

Il2CppString* String::Intern (Il2CppString* str)
{
	os::FastAutoLock lockMap (&s_InternedStringMapMutex);

	// allocate this are runtime since it uses GC allocator to keep managed strings alive and needs GC initialized
	if (s_InternedStringMap == NULL)
		s_InternedStringMap = new InternedStringMap ();

	InternedString internedString = { str->length, str->chars };
	InternedStringMap::const_iterator iter = s_InternedStringMap->find (internedString);
	if (iter != s_InternedStringMap->end ())
		return iter->second;

	internedString.chars = String::GetChars (str);
	s_InternedStringMap->insert (std::make_pair (internedString, str));

	return str;
}

Il2CppString* String::IsInterned(Il2CppString* str)
{
	os::FastAutoLock lockMap (&s_InternedStringMapMutex);

	// if this is NULL, it means we have no interned strings
	if (s_InternedStringMap == NULL)
		return NULL;

	InternedString internedString = { str->length, str->chars };
	InternedStringMap::const_iterator iter = s_InternedStringMap->find(internedString);
	if (iter != s_InternedStringMap->end())
		return iter->second;

	return NULL;
}

} /* namespace vm */
} /* namespace il2cpp */
