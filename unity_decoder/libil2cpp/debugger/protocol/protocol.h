#pragma once

#include "../common.h"

#include <map>

#include "globals.h"
#include "utils.h"

struct Il2CppThread;

namespace il2cpp
{
namespace debugger
{

class Reply;
class Buffer;
class Command;
class IDataTransport;

enum HeaderType
{
	kHeaderTypeCommand = 0x00,
	kHeaderTypeReply = 0x80
};

class Header
{

public:

	static const int32_t Length = 11;

	explicit Header(HeaderType type);

	inline HeaderType type() const
	{
		return (HeaderType)_flags;
	}

	inline bool is_command() const
	{
		return _flags == kHeaderTypeCommand;
	}

	inline bool is_reply() const
	{
		return _flags == kHeaderTypeReply;
	}

	inline size_t message_size() const
	{
		return _packet_size - Header::Length;
	}

	inline int32_t id() const
	{
		return _id;
	}

	inline void id(int32_t v)
	{
		_id = v;
	}

	inline size_t packet_size() const
	{
		return _packet_size;
	}

	inline void packet_size(size_t v)
	{
		_packet_size = v;
	}

	inline CommandSet set() const
	{
		assert(is_command() && "Header::set() can only be invoked on `Command` headers");

		return (CommandSet)((_data >> 8) & 0xFF);
	}

	inline void set(CommandSet v)
	{
		assert(is_command() && "Header::set(CommandSet) can only be invoked on `Command` headers");

		_data = (v << 8) | (_data & 0xFF);
	}

	inline int32_t command() const
	{
		assert(is_command() && "Header::command() can only be invoked on `Command` headers");

		return _data & 0xFF;
	}

	inline void command(int32_t v)
	{
		assert(is_command() && "Header::command(int32_t) can only be invoked on `Command` headers");

		_data = (_data & 0xFF00) | (v & 0xFF);
	}

	inline ErrorCode error_code() const
	{
		assert(is_reply() && "Header::error_code() can only be invoked on `Reply` headers");

		return (ErrorCode)_data;
	}

	inline void error_code(ErrorCode v)
	{
		assert(is_reply() && "Header::error_code(ErrorCode) can only be invoked on `Command` headers");

		_data = v;
	}

	static void ReadHeader(Header &header, Buffer &in);

	void WriteTo(Buffer &out) const;

private:

	size_t _packet_size;
	int32_t _id;
	uint8_t _flags;
	uint16_t _data;

	DISALLOW_COPY(Header);
};

#define PROTOCOL_COMMAND(TSet, TCommand) \
	public: \
		static const CommandSet SetKind = kCommandSet ## TSet; \
		static const int32_t CommandKind = kCommandSet ## TSet ## TCommand; \
		inline static Command *Factory(Header &header, Buffer &in) \
		{ \
			Command * command = new TSet ## TCommand ## Command(header.id()); \
			command->ReadProperties(in); \
			return command; \
		}

class Command
{

public:

	Command(CommandSet set, int32_t command, int32_t id);
	virtual ~Command();

	inline CommandSet set() const
	{
		return _set;
	}

	inline int32_t command() const
	{
		return _command;
	}

	inline int32_t id() const
	{
		return _id;
	}

	inline bool is(CommandSet set, int32_t command) const
	{
		return _set == set && _command == command;
	}
	
	void WriteTo(Buffer &out) const;

	virtual void ReadProperties(Buffer &in);

protected:
	
	virtual void WriteContentTo(Buffer &out) const;

	int32_t _id;
	CommandSet _set;
	int32_t _command;

	DISALLOW_COPY(Command);
};

class Reply
{

public:
	
	explicit Reply(const Command *command);
	virtual ~Reply();

	inline const Command *command() const
	{
		return _command;
	}

	inline ErrorCode error_code() const
	{
		return _error_code;
	}

	inline void error_code(ErrorCode code)
	{
		_error_code = code;
	}

	void WriteTo(Buffer &out) const;

protected:
	
	virtual void WriteContentTo(Buffer &out) const;
	
	const Command *_command;
	ErrorCode _error_code;

	DISALLOW_COPY(Reply);
};

class EmptyReply : public Reply
{

public:

	explicit EmptyReply(const Command *command);

	DISALLOW_COPY(EmptyReply);
};

class Event
{

public:

	Event();
	explicit Event(Il2CppThread *thread);
	virtual ~Event();

	inline virtual EventKind kind() const
	{
		return kEventKindInvalid;
	}

	inline Il2CppThread *thread() const
	{
		return _thread;
	}

	inline void thread(Il2CppThread *v)
	{
		_thread = v;
	}

	void WriteTo(Buffer &out, int32_t req_id) const;

protected:
	
	virtual void WriteContentTo(Buffer &out) const;

	Il2CppThread *_thread;

	DISALLOW_COPY(Event);
};

class Protocol
{

	typedef uint16_t CommandUId;
	typedef Command* (*CommandGenerator)(Header &header, Buffer &in);

public:

	static const int32_t MajorVersion = 2;
	static const int32_t MinorVersion = 1;

	Protocol();
	~Protocol();

	inline IDataTransport *transport()
	{
		return _transport;
	}

	inline int32_t major_version() const
	{
		return _major_version;
	}

	inline void major_version(int32_t v)
	{
		_major_version = v;
	}

	inline int32_t minor_version() const
	{
		return _minor_version;
	}

	inline void minor_version(int32_t v)
	{
		_minor_version = v;
	}

	inline bool is_disposed() const
	{
		return _transport == 0;
	}

	void Setup(IDataTransport *transport);
	void Dispose();

	bool Handshake();

	// Note WaitForCommand gives ownership of the Command object away for now.
	// It is responsability of the invoker to delete the Command when done.
	const Command  *WaitForCommand();
	bool SendReply(const Reply *reply);
	bool SendCommand(const Command *command);

private:

	template<typename TCommand>
	inline void RegisterCommand()
	{
		LOG("Registering " << command_to_string(TCommand::SetKind, TCommand::CommandKind) << " with id: " << CommandIdFor(TCommand::SetKind, TCommand::CommandKind));
		_command_factories[CommandIdFor(TCommand::SetKind, TCommand::CommandKind)] = &TCommand::Factory;
	}

	Command *CreateCommand(Header &header, Buffer &in) const;
	CommandUId CommandIdFor(Header &header) const;
	CommandUId CommandIdFor(CommandSet set, int32_t command) const;
	
	int32_t _major_version;
	int32_t _minor_version;
	IDataTransport *_transport;
	std::map<CommandUId, CommandGenerator> _command_factories;

	DISALLOW_COPY(Protocol);
};

} /* namespace debugger */
} /* namespace il2cpp */
