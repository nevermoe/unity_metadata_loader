#if IL2CPP_DEBUGGER_ENABLED

#include "protocol.h"

#include "../../il2cpp-api.h"

#include "../data-buffer.h"
#include "../id-bank.h"
#include "../transport.h"
#include "../agent.h"

#include "utils.h"

#include "commands/type.h"
#include "commands/vm.h"
#include "commands/app-domain.h"
#include "commands/array-ref.h"
#include "commands/assembly.h"
#include "commands/event.h"
#include "commands/event-request.h"
#include "commands/internal-error.h"
#include "commands/method.h"
#include "commands/module.h"
#include "commands/object-ref.h"
#include "commands/stack-frame.h"
#include "commands/string-ref.h"
#include "commands/thread.h"

using namespace il2cpp::debugger;

Header::Header(HeaderType type) :
    _packet_size(0),
    _id(0),
    _flags(type),
    _data(0)
{
}

void Header::ReadHeader(Header &header, Buffer &in)
{
    const int32_t length = in.ReadInt();
    const int32_t id = in.ReadInt();
    const HeaderType type = in.ReadEnum<HeaderType>();
    const uint16_t data = (in.ReadByte() << 8) | in.ReadByte();

    IL2CPP_ASSERT(header.type() == type && "Incompatible Header types: incoming header is not of the expected type");

    header.packet_size(length);
    header.id(id);

    if (header.is_command())
    {
        header.set((CommandSet)((data >> 8) & 0xFF));
        header.command(data & 0xFF);
    }
    else
    {
        header.error_code((ErrorCode)data);
    }
}

void Header::WriteTo(Buffer &out) const
{
    out.WriteInt(packet_size());
    out.WriteInt(id());
    out.WriteByte(_flags);
    out.WriteByte((_data >> 8) & 0xFF);
    out.WriteByte(_data & 0xFF);
}

Command::Command(CommandSet set, int32_t command, int32_t id) :
    _id(id),
    _set(set),
    _command(command)
{
}

Command::~Command()
{
}

void Command::ReadProperties(Buffer &in)
{
}

void Command::WriteTo(Buffer &out) const
{
    out.Seek(Header::Length);

    WriteContentTo(out);

    Header header(kHeaderTypeCommand);

    header.packet_size(out.length());
    header.id(_id);
    header.set(set());
    header.command(command());

    out.Seek(0);
    header.WriteTo(out);
    out.Seek(header.packet_size());
}

void Command::WriteContentTo(Buffer &out) const
{
}

Event::Event() :
    _thread(il2cpp_thread_current())
{
}

Event::Event(Il2CppThread *thread) :
    _thread(thread)
{
}

Event::~Event()
{
}

void Event::WriteTo(Buffer &out, int32_t req_id) const
{
    out.WriteByte(kind());
    out.WriteInt(req_id);
    out.WritePtrId(ObjectId((Il2CppObject*)_thread));

    WriteContentTo(out);
}

void Event::WriteContentTo(Buffer &out) const
{
}

Reply::Reply(const Command *command) :
    _command(command),
    _error_code(kErrorCodeNone)
{
}

Reply::~Reply()
{
}

void Reply::WriteTo(Buffer &out) const
{
    // Reserve space for the header
    out.Seek(Header::Length);

    if (error_code() == kErrorCodeNone)
        WriteContentTo(out);
    else
        LOG("Reply returned an error: " << error_code());

    Header header(kHeaderTypeReply);

    header.packet_size(out.length());
    header.id(_command ? _command->id() : 0);
    header.error_code(error_code());

    out.Seek(0);
    header.WriteTo(out);
    out.Seek(header.packet_size());
}

void Reply::WriteContentTo(Buffer &out) const
{
}

EmptyReply::EmptyReply(const Command *command) :
    Reply(command)
{
}

Protocol::Protocol() :
    _minor_version(Protocol::MinorVersion),
    _major_version(Protocol::MajorVersion),
    _transport(0)
{
}

Protocol::~Protocol()
{
    _transport = 0;
}

void Protocol::Setup(IDataTransport *transport)
{
    IL2CPP_ASSERT(transport != NULL && "Invalid data transport");

    _transport = transport;

    RegisterCommand<VmSetProtocolVersionCommand>();
    RegisterCommand<VmVersionCommand>();
    RegisterCommand<VmResumeCommand>();
    RegisterCommand<VmAllThreadsCommand>();
    RegisterCommand<VmSuspendCommand>();
    RegisterCommand<VmDisposeCommand>();
    RegisterCommand<VmAbortInvokeCommand>();
    RegisterCommand<VmExitCommand>();
    RegisterCommand<VmInvokeMethodCommand>();

    RegisterCommand<TypeGetInfoCommand>();
    RegisterCommand<TypeGetSourceFilesCommand>();
    RegisterCommand<TypeGetPropertiesCommand>();
    RegisterCommand<TypeGetSourceFiles2Command>();
    RegisterCommand<TypeGetPropertyCAttrsCommand>();
    RegisterCommand<TypeGetFieldCAttrsCommand>();
    RegisterCommand<TypeIsAssignableFromCommand>();
    RegisterCommand<TypeGetCAttrsCommand>();
    RegisterCommand<TypeGetValuesCommand>();
    RegisterCommand<TypeGetObjectCommand>();
    RegisterCommand<TypeGetFieldsCommand>();
    RegisterCommand<TypeSetValuesCommand>();
    RegisterCommand<TypeGetMethodsCommand>();

    RegisterCommand<ThreadGetIdCommand>();
    RegisterCommand<ThreadGetNameCommand>();
    RegisterCommand<ThreadGetInfoCommand>();
    RegisterCommand<ThreadGetFrameInfoCommand>();
    RegisterCommand<ThreadGetStateCommand>();

    RegisterCommand<StringRefGetValueCommand>();

    RegisterCommand<StackFrameGetValuesCommand>();
    RegisterCommand<StackFrameSetValuesCommand>();
    RegisterCommand<StackFrameGetThisCommand>();

    RegisterCommand<ObjectRefGetTypeCommand>();
    RegisterCommand<ObjectRefGetDomainCommand>();
    RegisterCommand<ObjectRefIsCollectedCommand>();
    RegisterCommand<ObjectRefSetValuesCommand>();
    RegisterCommand<ObjectRefGetAddressCommand>();
    RegisterCommand<ObjectRefGetValuesCommand>();

    RegisterCommand<ModuleGetInfoCommand>();

    RegisterCommand<MethodGetDeclaringTypeCommand>();
    RegisterCommand<MethodGetDebugInfoCommand>();
    RegisterCommand<MethodGetNameCommand>();
    RegisterCommand<MethodGetParamInfoCommand>();
    RegisterCommand<MethodGetBodyCommand>();
    RegisterCommand<MethodGetLocalsInfoCommand>();
    RegisterCommand<MethodGetInfoCommand>();
    RegisterCommand<MethodResolveTokenCommand>();

    RegisterCommand<InternalErrorNotImplementedCommand>();

    RegisterCommand<EventCompositeCommand>();

    RegisterCommand<EventRequestSetCommand>();
    RegisterCommand<EventRequestClearCommand>();
    RegisterCommand<EventRequestClearAllBreakpointsCommand>();

    RegisterCommand<AssemblyGetTypeCommand>();
    RegisterCommand<AssemblyGetObjectCommand>();
    RegisterCommand<AssemblyGetEntryPointCommand>();
    RegisterCommand<AssemblyGetManifestModuleCommand>();
    RegisterCommand<AssemblyGetNameCommand>();
    RegisterCommand<AssemblyGetLocationCommand>();

    RegisterCommand<ArrayRefGetValuesCommand>();
    RegisterCommand<ArrayRefGetLengthCommand>();
    RegisterCommand<ArrayRefSetValuesCommand>();

    RegisterCommand<AppDomainGetRootDomainCommand>();
    RegisterCommand<AppDomainGetCorlibCommand>();
    RegisterCommand<AppDomainGetFriendlyNameCommand>();
    RegisterCommand<AppDomainCreateStringCommand>();
    RegisterCommand<AppDomainGetEntryAssemblyCommand>();
    RegisterCommand<AppDomainCreateBoxedValueCommand>();
    RegisterCommand<AppDomainGetAssembliesCommand>();
}

Command *Protocol::CreateCommand(Header &header, Buffer &in) const
{
    IL2CPP_ASSERT(header.is_command() && "CommandFactory::Create can only be used with `command` headers.");

    const CommandUId uid = CommandIdFor(header.set(), header.command());
    std::map<CommandUId, CommandGenerator>::const_iterator it = _command_factories.find(uid);

    if (it == _command_factories.end())
    {
        LOG("Command not found: " << command_set_to_string(header.set()) << "+" << command_to_string(header.set(), header.command()) << "(" << header.set() << ":" << header.command() << ")");
        return NULL;
    }

    return it->second(header, in);
}

Protocol::CommandUId Protocol::CommandIdFor(Header &header) const
{
    return CommandIdFor(header.set(), header.command());
}

Protocol::CommandUId Protocol::CommandIdFor(CommandSet set, int32_t command) const
{
    return ((((int32_t)set) & 0xFF) << 8) | (command & 0xFF);
}

bool Protocol::Handshake()
{
    const char *handshake_msg = "DWP-Handshake";
    const size_t msg_length = strlen(handshake_msg);

    Buffer in(128);
    Buffer out(128);

    out.WriteData((const uint8_t*)handshake_msg, msg_length);

    int32_t sent_bytes = _transport->Send(out.raw(), out.length());
    if (sent_bytes != out.length())
    {
        LOG("DWP handshake failed.");
        return false;
    }

    int32_t received_bytes = _transport->Recv(in.raw(), msg_length);
    if ((received_bytes != msg_length) || (memcmp(in.raw(), handshake_msg, msg_length != 0)))
    {
        LOG("DWP handshake failed.");
        return false;
    }

    return true;
}

const Command *Protocol::WaitForCommand()
{
    // In/Out buffers are allocated at every iteration. This needs to be improved
    Buffer in(Header::Length);

    int32_t read_bytes = _transport->Recv(in.raw(), Header::Length);
    if (read_bytes != Header::Length)
    {
        LOG("Couldn't parse the header. Expecting " << Header::Length << " bytes, but got " << read_bytes << ". Shutting down.");
        return NULL;
    }

    in.MarkReadRaw(read_bytes);

    Header header(kHeaderTypeCommand);

    Header::ReadHeader(header, in);

    LOG("Received command " << command_set_to_string(header.set()) << " " << command_to_string(header.set(), header.command()) << "(" << header.command() << "), id=" << header.id() << ".");

    if (header.message_size() > 0)
    {
        in.Reset(header.message_size());

        if (is_disposed())
            return NULL;

        int32_t received = _transport->Recv(in.raw(), header.message_size());
        if (received != header.message_size())
        {
            LOG("Expecting a command of " << header.message_size() << " bytes. Got " << received << ". Shutting down.");
            exit(1);
        }

        in.MarkReadRaw(received);
    }

    Command *command = CreateCommand(header, in);
    if (command == NULL)
    {
        LOG("Not implemented command");
        command = new InternalErrorNotImplementedCommand(header.id());
    }

    return command;
}

bool Protocol::SendReply(const Reply *reply)
{
    // For now, every reply has its own 'out' buffer as long as SendReply might
    // be called from different threads. This needs to be improved.

    if (is_disposed())
    {
        LOG("The protocol was disposed. Reply can not be sent.");
        return false;
    }

    Buffer out(128);

    reply->WriteTo(out);

    if (!_transport->Send(out.raw(), out.length()))
    {
        LOG("The socket was shut down while sending the header");
        return false;
    }

    return true;
}

bool Protocol::SendCommand(const Command *command)
{
    // For now, every reply has its own 'out' buffer as long as SendReply might
    // be called from different threads. This needs to be improved.

    if (is_disposed())
    {
        LOG("The protocol was disposed. Command can not be sent.");
        return false;
    }

    Buffer out(128);

    command->WriteTo(out);

    if (!_transport->Send(out.raw(), out.length()))
    {
        LOG("The socket was shut down while sending the header");
        return false;
    }

    return true;
}

void Protocol::Dispose()
{
    if (_transport == 0)
        return;

    _transport->Close();
    _transport = 0;
}

#endif
