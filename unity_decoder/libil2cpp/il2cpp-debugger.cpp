#if IL2CPP_DEBUGGER_ENABLED

#include "il2cpp-debugger.h"

#include "debugger/agent.h"
#include "debugger/command-line-parser.h"

using namespace il2cpp;

bool il2cpp_debugger_parse_command_line(int argc, char* argv[], char** host, int32_t* port, bool* print_help)
{
    debugger::CommandLineOptions options;
    bool properArgumentsProvided = debugger::CommandLineParser::Parse(argc, argv, &options);

    *print_help = options.print_help;

    if (properArgumentsProvided)
    {
        *host = options.host;
        *port = options.port;
    }

    return properArgumentsProvided;
}

void il2cpp_debugger_print_usage()
{
    debugger::CommandLineParser::PrintUsage(std::cout);
}

void il2cpp_debugger_agent_init()
{
    debugger::Agent::instance().Initialize(
        debugger::AgentConfig::Default);
}

void il2cpp_debugger_agent_init(const char *host, int32_t port)
{
    debugger::AgentConfig config;

    config.server = true;
    config.host = host;
    config.port = port;

    debugger::Agent::instance().Initialize(config);
}

bool il2cpp_debugger_agent_is_initialized()
{
    return debugger::Agent::instance().IsInitialized();
}

void il2cpp_debugger_agent_dispose()
{
    debugger::Agent::instance().Dispose();
}

void il2cpp_debugger_notify_vm_start()
{
    debugger::Agent::instance().NotifyVmStart();
}

void il2cpp_debugger_notify_vm_death()
{
    debugger::Agent::instance().NotifyVmDeath();
}

void il2cpp_debugger_notify_user_break()
{
    debugger::Agent::instance().NotifyUserBreak();
}

void il2cpp_debugger_notify_appdomain_create(Il2CppDomain *domain)
{
    debugger::Agent::instance().NotifyAppDomainCreate(domain);
}

void il2cpp_debugger_notify_assembly_load(const Il2CppAssembly *assembly)
{
    debugger::Agent::instance().NotifyAssemblyLoad(assembly);
}

void il2cpp_debugger_sequence_point_hit(int64_t uid, int32_t offset)
{
    // TODO: this is time critical, it might make sense to inline it directly?
    debugger::Agent::instance().SequencePointHit(uid, offset);
}

void il2cpp_debugger_notify_type_load(Il2CppClass *type)
{
    debugger::Agent::instance().NotifyTypeLoad(type);
}

void il2cpp_debugger_method_entry(const Il2CppStackFrameInfo &info)
{
    debugger::Agent::instance().OnMethodEntry(info);
}

void il2cpp_debugger_method_exit(const Il2CppStackFrameInfo &info)
{
    debugger::Agent::instance().OnMethodExit(info);
}

void il2cpp_debugger_notify_thread_attach(Il2CppThread *thread)
{
    debugger::Agent::instance().OnThreadAttach(thread);
}

void il2cpp_debugger_notify_thread_detach(Il2CppThread *thread)
{
    debugger::Agent::instance().OnThreadDetach(thread);
}

#endif
