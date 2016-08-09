#pragma once

#include <vector>

#include "../../common.h"
#include "../protocol-utils.h"

struct MethodInfo;

namespace il2cpp
{
namespace debugger
{

struct ThreadGetIdCommand : public CustomCommand<ThreadGetIdCommand>
{
	struct Reply : public CustomReply
	{
		Property<uint64_t> id;

		void WriteContentTo(Buffer &out) const
		{
			id.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Thread, GetId);

	Property<Il2CppThread*> thread;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
	}
};

struct ThreadGetNameCommand : public CustomCommand<ThreadGetNameCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> name;

		void WriteContentTo(Buffer &out) const
		{
			name.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Thread, GetName);

	Property<Il2CppThread*> thread;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
	}
};

struct ThreadGetInfoCommand : public CustomCommand<ThreadGetInfoCommand>
{
	struct Reply : public CustomReply
	{
		Property<bool> is_thread_pool;

		void WriteContentTo(Buffer &out) const
		{
			is_thread_pool.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Thread, GetInfo);

	Property<Il2CppThread*> thread;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
	}
};

struct ThreadGetFrameInfoCommand : public CustomCommand<ThreadGetFrameInfoCommand>
{
	struct Reply : public CustomReply
	{
		struct FrameInfo
		{
			int32_t frame_id;
			const MethodInfo *method;
			int32_t il_offset;
			uint8_t flags;
		};

		inline std::vector<FrameInfo> &infos()
		{
			return _infos;
		}

		void WriteContentTo(Buffer &out) const;

	private:
		
		std::vector<FrameInfo> _infos;
	};


	CUSTOM_COMMAND(Thread, GetFrameInfo);

	Property<Il2CppThread*> thread;
	Property<int32_t> stack_frame;
	Property<int32_t> length;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
		stack_frame.ReadFrom(in);
		length.ReadFrom(in);
	}
};

struct ThreadGetStateCommand : public CustomCommand<ThreadGetStateCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> state;

		void WriteContentTo(Buffer &out) const
		{
			state.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Thread, GetState);

	Property<Il2CppThread*> thread;

	void ReadProperties(Buffer &in)
	{
		thread.ReadFrom(in);
	}
};

}
}
