#pragma once

struct Il2CppGenericInst;

namespace il2cpp
{
namespace metadata
{

class Il2CppGenericInstLess
{
public:
	bool operator() (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2) const;
	static bool Compare (const Il2CppGenericInst* t1,const Il2CppGenericInst* t2);
};

} /* namespace vm */
} /* namespace il2cpp */
