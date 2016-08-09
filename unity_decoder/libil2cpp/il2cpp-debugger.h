#pragma once

#include "il2cpp-config.h"

struct Il2CppClass;
struct MethodInfo;
struct Il2CppThread;
struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppStackFrameInfo;

#if IL2CPP_DEBUGGER_ENABLED
void il2cpp_debugger_agent_init();
void il2cpp_debugger_agent_init(const char *host, int32_t port);
void il2cpp_debugger_agent_dispose();
bool il2cpp_debugger_agent_is_initialized();
bool il2cpp_debugger_parse_command_line(int argc, char* argv[], char** host, int32_t* port, bool* print_help);
void il2cpp_debugger_print_usage();

void il2cpp_debugger_notify_vm_start();
void il2cpp_debugger_notify_vm_death();
void il2cpp_debugger_notify_user_break();
void il2cpp_debugger_notify_appdomain_create(Il2CppDomain *domain);
void il2cpp_debugger_notify_assembly_load(const Il2CppAssembly *assembly);
void il2cpp_debugger_sequence_point_hit(int64_t uid, int32_t offset);
void il2cpp_debugger_notify_thread_attach(Il2CppThread *thread);
void il2cpp_debugger_notify_thread_detach(Il2CppThread *thread);
void il2cpp_debugger_notify_type_load(Il2CppClass *type);
void il2cpp_debugger_method_entry(const Il2CppStackFrameInfo &info);
void il2cpp_debugger_method_exit(const Il2CppStackFrameInfo &info);
#endif
