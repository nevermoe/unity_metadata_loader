#pragma once

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct ObjectRefGetTypeCommand : public CustomCommand<ObjectRefGetTypeCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppClass*> type;

		void WriteContentTo(Buffer &out) const
		{
			type.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(ObjectRef, GetType);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ObjectRefGetDomainCommand : public CustomCommand<ObjectRefGetDomainCommand>
{
	struct Reply : public CustomReply
	{
		Property<Il2CppDomain*> domain;

		void WriteContentTo(Buffer &out) const
		{
			domain.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(ObjectRef, GetDomain);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ObjectRefIsCollectedCommand : public CustomCommand<ObjectRefIsCollectedCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(ObjectRef, IsCollected);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ObjectRefSetValuesCommand : public CustomCommand<ObjectRefSetValuesCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(ObjectRef, SetValues);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ObjectRefGetAddressCommand : public CustomCommand<ObjectRefGetAddressCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(ObjectRef, GetAddress);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ObjectRefGetValuesCommand : public CustomCommand<ObjectRefGetValuesCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(ObjectRef, GetValues);

	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

}
}
