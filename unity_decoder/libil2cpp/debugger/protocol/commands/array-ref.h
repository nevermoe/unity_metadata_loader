#pragma once

#include "../../common.h"
#include "../../variant.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct ArrayRefGetValuesCommand : public CustomCommand<ArrayRefGetValuesCommand>
{
	struct Reply : public CustomReply
	{
		inline std::vector<Variant> &values()
		{
			return _values;
		}

		void WriteContentTo(Buffer &out) const;

	private:

		std::vector<Variant> _values;
	};


	CUSTOM_COMMAND(ArrayRef, GetValues);
	
	Property<Il2CppObject*> object;
	Property<int32_t> index;
	Property<int32_t> length;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
		index.ReadFrom(in);
		length.ReadFrom(in);
	}
};

struct ArrayRefGetLengthCommand : public CustomCommand<ArrayRefGetLengthCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> rank;
		Property<std::vector<int32_t> > lengths;
		Property<std::vector<int32_t> > lower_bounds;

		void WriteContentTo(Buffer &out) const
		{
			rank.WriteTo(out);

			for(size_t i = 0; i < lengths().size(); ++i)
			{
				out.WriteInt(lengths().at(i));
				out.WriteInt(lower_bounds().at(i));
			}
		}
	};


	CUSTOM_COMMAND(ArrayRef, GetLength);
	
	Property<Il2CppObject*> object;

	void ReadProperties(Buffer &in)
	{
		object.ReadFrom(in);
	}
};

struct ArrayRefSetValuesCommand : public CustomCommand<ArrayRefSetValuesCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented");
		}
	};


	CUSTOM_COMMAND(ArrayRef, SetValues);
};

}
}
