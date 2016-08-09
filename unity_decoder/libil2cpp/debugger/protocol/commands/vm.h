#pragma once

#include <vector>

#include "../../common.h"
#include "../protocol-utils.h"

namespace il2cpp
{
namespace debugger
{

struct VmSetProtocolVersionCommand : public CustomCommand<VmSetProtocolVersionCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(Vm, SetProtocolVersion);

	Property<int32_t> major_version;
	Property<int32_t> minor_version;

	void ReadProperties(Buffer &in)
	{
		major_version.ReadFrom(in);
		minor_version.ReadFrom(in);
	}
};

struct VmVersionCommand : public CustomCommand<VmVersionCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::string> name;
		Property<int32_t> major_version;
		Property<int32_t> minor_version;

		void WriteContentTo(Buffer &out) const
		{
			name.WriteTo(out);
			major_version.WriteTo(out);
			minor_version.WriteTo(out);
		}
	};

	CUSTOM_COMMAND(Vm, Version);
};

struct VmResumeCommand : public CustomCommand<VmResumeCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(Vm, Resume);
};

struct VmDisposeCommand : public CustomCommand<VmDisposeCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(Vm, Dispose);
};

struct VmSuspendCommand : public CustomCommand<VmSuspendCommand>
{
	CUSTOM_COMMAND_EMPTY_REPLY(Vm, Suspend);
};

struct VmAllThreadsCommand : public CustomCommand<VmAllThreadsCommand>
{
	struct Reply : public CustomReply
	{
		Property<std::vector<Il2CppThread*> > threads;

		void WriteContentTo(Buffer &out) const
		{
			threads.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Vm, AllThreads);
};

struct VmExitCommand : public CustomCommand<VmExitCommand>
{
	struct Reply : public CustomReply
	{
		Property<int32_t> exit_code;

		void WriteContentTo(Buffer &out) const
		{
			exit_code.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Vm, Exit);
};

struct VmAbortInvokeCommand : public CustomCommand<VmAbortInvokeCommand>
{
	struct Reply : public CustomReply
	{
		void WriteContentTo(Buffer &out) const
		{
			assert(0 && "Not implemented!");
		}
	};


	CUSTOM_COMMAND(Vm, AbortInvoke);
};

struct VmInvokeMethodCommand : public CustomCommand<VmInvokeMethodCommand>
{
	struct Reply : public CustomReply
	{
		Reply() : skip_return_value(false) {}

		Property<bool> method_did_not_throw_exception;
		Property<uint8_t> return_type;
		Property<int32_t> return_value;

		bool skip_return_value;

		void WriteContentTo(Buffer &out) const
		{
			method_did_not_throw_exception.WriteTo(out);
			return_type.WriteTo(out);

			if (!skip_return_value)
				return_value.WriteTo(out);
		}
	};


	CUSTOM_COMMAND(Vm, InvokeMethod);

	Property<Il2CppThread*> vm_thread;
	Property<int32_t> flags;
	Property<const MethodInfo*> method;

	void ReadProperties(Buffer &in)
	{
		vm_thread.ReadFrom(in);
		flags.ReadFrom(in);
		method.ReadFrom(in);
	}
};

}
}
