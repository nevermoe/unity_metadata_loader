#pragma once

#include <string>

#include "../common.h"
#include "../meta.h"
#include "protocol.h"

namespace il2cpp
{
namespace debugger
{

template<typename Cmd>
struct CustomCommand : public Command
{
	explicit CustomCommand(int32_t id) :
		Command(Cmd::SetKind, Cmd::CommandKind, id)
	{

	}
};

#define _CUSTOM_COMMAND(TSet, TCommand) \
	public: \
		static const CommandSet SetKind = kCommandSet ## TSet; \
		static const int32_t CommandKind = kCommandSet ## TSet ## TCommand; \
		inline static Command *Factory(Header &header, Buffer &in) \
		{ \
			Command * command = new TSet ## TCommand ## Command(header.id()); \
			command->ReadProperties(in); \
			return command; \
		} \
		explicit TSet ## TCommand ## Command(int32_t id) : \
			CustomCommand(id) {}

#define CUSTOM_COMMAND(TSet, TCommand) \
	_CUSTOM_COMMAND(TSet, TCommand) \
	TSet ## TCommand ## Command::Reply * reply() const \
	{ \
		TSet ## TCommand ## Command::Reply *reply = new TSet ## TCommand ## Command::Reply(); \
		reply->command(this); \
		return reply; \
	}

#define CUSTOM_COMMAND_EMPTY_REPLY(TSet, TCommand) \
	_CUSTOM_COMMAND(TSet, TCommand) \
	struct Reply : public EmptyReply \
	{ \
		explicit Reply(const Command *command) : EmptyReply(command) {} \
	}; \
	TSet ## TCommand ## Command::Reply * reply() const \
	{ \
		return new TSet ## TCommand ## Command::Reply(this); \
	}

struct CustomReply : public Reply
{
	CustomReply() :
		Reply(NULL)
	{

	}

	inline void command(const Command *v)
	{
		_command = v;
	}
};

}
}
