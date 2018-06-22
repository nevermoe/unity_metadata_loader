#pragma once

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct StringRefGetValueCommand : public CustomCommand<StringRefGetValueCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> value;

		void WriteContentTo(Buffer &out) const
		{
			value.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(StringRef, GetValue);
	
	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

}
}
