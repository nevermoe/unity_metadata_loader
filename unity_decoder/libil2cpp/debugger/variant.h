#pragma once

#include "common.h"

#include "protocol/globals.h"

namespace il2cpp
{
namespace debugger
{

class Buffer;

class Variant
{

public:

	Variant();
	Variant(const Il2CppType *type, void *value);

	inline const Il2CppType *type() const 
	{
		return _type;
	}
	
	inline void *value() const
	{
		return _value;
	}

	virtual void WriteTo(Buffer &out) const;
	
	static ErrorCode ReadInto(Buffer &in, const Il2CppType *expected_type, void *addr);

private:

	const Il2CppType *_type;
	void *_value;
};

class VTypeVariant : public Variant
{

public:

	VTypeVariant();
	VTypeVariant(const Il2CppType *type, void *value);

	virtual void WriteTo(Buffer &out) const;
};

} /* namespace debugger */
} /* namespace il2cpp */
