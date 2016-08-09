#pragma once

#include <vector>

#include "../../common.h"
#include "../../variant.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct StackFrameGetValuesCommand : public CustomCommand<StackFrameGetValuesCommand>
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


	CUSTOM_COMMAND(StackFrame, GetValues);
	
	Property<int32_t> frame_id;
	Property<Il2CppThread*> thread;
	Property<std::vector<int32_t> > positions;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
		frame_id.ReadFrom(in);
		positions.ReadFrom(in);
	}
};

struct StackFrameSetValuesCommand : public CustomCommand<StackFrameSetValuesCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(StackFrame, SetValues);

	Property<int32_t> frame_id;
	Property<Il2CppThread*> thread;
	Property<int32_t> values_count;

	inline Buffer &command_buffer() const
	{
		return *_command_buffer;
	}

	void ReadProperties(Buffer &in);

	~StackFrameSetValuesCommand()
	{
		delete _command_buffer;
	}

private:

	Buffer *_command_buffer;
};

struct StackFrameGetThisCommand : public CustomCommand<StackFrameGetThisCommand>
{
	struct Reply : public CustomReply
	{
		Variant this_object;

		void WriteContentTo(Buffer &out) const
		{
			out.WriteVariant(this_object);
		}
	};


	CUSTOM_COMMAND(StackFrame, GetThis);
	
	Property<int32_t> frame_id;
	Property<Il2CppThread*> thread;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
		frame_id.ReadFrom(in);
	}
};

}
}