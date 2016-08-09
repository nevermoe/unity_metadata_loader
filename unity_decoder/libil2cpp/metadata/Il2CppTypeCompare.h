#pragma once

struct Il2CppType;

namespace il2cpp
{
namespace metadata
{

class Il2CppTypeCompare
{
public:
	bool operator() (const Il2CppType* t1,const Il2CppType* t2) const;
	static bool Compare (const Il2CppType* t1,const Il2CppType* t2);
};

} /* namespace vm */
} /* namespace il2cpp */
