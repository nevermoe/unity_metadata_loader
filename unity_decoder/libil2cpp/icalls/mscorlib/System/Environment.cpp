#include "il2cpp-config.h"
#include "object-internals.h"
#include "class-internals.h"

#include "icalls/mscorlib/System/Environment.h"

#include "os/Time.h"
#include "os/Environment.h"

#include "vm/Array.h"
#include "vm/Class.h"
#include "vm/Domain.h"
#include "vm/Runtime.h"
#include "vm/String.h"
#include "vm/Exception.h"

#include "utils/PathUtils.h"
#include "utils/StringUtils.h"
#include "utils/Environment.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
    static int32_t exitcode = 0;
    static bool socket_security_enabled = false;

    static Il2CppArray* ToIl2CppArray(const std::vector<std::string>& strings)
    {
        Il2CppClass *klass = il2cpp::vm::Class::GetArrayClass(il2cpp_defaults.string_class, 1);
        Il2CppArray* array = (Il2CppArray*)il2cpp::vm::Array::NewSpecific(klass, (il2cpp_array_size_t)strings.size());

        for (size_t i = 0, size = strings.size(); i < size; ++i)
            il2cpp_array_setref(array, i, il2cpp::vm::String::New(strings[i].c_str()));

        return array;
    }

    Il2CppString* Environment::get_MachineName()
    {
        return il2cpp::vm::String::New(il2cpp::os::Environment::GetMachineName().c_str());
    }

    Il2CppString* Environment::get_NewLine(void)
    {
        return il2cpp::vm::String::NewUtf16(kIl2CppNewLine);
    }

    bool Environment::get_SocketSecurityEnabled()
    {
        return socket_security_enabled;
    }

    int32_t Environment::get_TickCount()
    {
        return il2cpp::os::Time::GetTicksMillisecondsMonotonic();
    }

    int32_t Environment::get_ProcessorCount()
    {
        return il2cpp::os::Environment::GetProcessorCount();
    }

    Il2CppArray* Environment::GetCommandLineArgs()
    {
        Il2CppArray *res;
        int i;
        int num_main_args = utils::Environment::GetNumMainArgs();
        const std::vector<UTF16String>& mainArgs = utils::Environment::GetMainArgs();

        Il2CppClass *klass = il2cpp::vm::Class::GetArrayClass(il2cpp_defaults.string_class, 1);
        res = (Il2CppArray*)il2cpp::vm::Array::NewSpecific(klass, num_main_args);

        for (i = 0; i < num_main_args; ++i)
            il2cpp_array_setref(res, i, il2cpp::vm::String::NewUtf16(mainArgs[i].c_str(), static_cast<int>(mainArgs[i].length())));

        return res;
    }

    Il2CppArray* Environment::GetEnvironmentVariableNames()
    {
        return ToIl2CppArray(il2cpp::os::Environment::GetEnvironmentVariableNames());
    }

    Il2CppString* Environment::GetWindowsFolderPath(int32_t folder)
    {
        return il2cpp::vm::String::New(il2cpp::os::Environment::GetWindowsFolderPath(folder).c_str());
    }

    Il2CppString* Environment::internalGetHome()
    {
        return il2cpp::vm::String::New(il2cpp::os::Environment::GetHomeDirectory().c_str());
    }

    Il2CppArray* Environment::GetLogicalDrivesInternal()
    {
        return ToIl2CppArray(il2cpp::os::Environment::GetLogicalDrives());
    }

    void Environment::InternalSetEnvironmentVariable(Il2CppString* variable, Il2CppString* value)
    {
        const std::string variableStr(utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(variable)));

        const bool clearValue = value == NULL || utils::StringUtils::GetLength(value) == 0 || utils::StringUtils::GetChars(0) == 0;

        const std::string valueStr = clearValue ? std::string() : utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(value));

        il2cpp::os::Environment::SetEnvironmentVariable(variableStr, valueStr);
    }

    void Environment::internalBroadcastSettingChange()
    {
        NOT_IMPLEMENTED_ICALL(Environment::internalBroadcastSettingChange);
    }

    Il2CppString * Environment::GetMachineConfigPath(void)
    {
        const char* frameworkVersion = vm::Runtime::GetFrameworkVersion();

        std::string path = utils::PathUtils::Combine(
                vm::Runtime::GetConfigDir(), utils::PathUtils::Combine(
                    utils::StringView<char>("mono"), utils::PathUtils::Combine(
                        utils::StringView<char>(frameworkVersion, strlen(frameworkVersion)), utils::StringView<char>("machine.config"))));

        return vm::String::NewWrapper(path.c_str());
    }

    Il2CppString * Environment::internalGetEnvironmentVariable(Il2CppString *name)
    {
        const std::string nameStr(utils::StringUtils::Utf16ToUtf8(utils::StringUtils::GetChars(name)));
        const std::string variable(il2cpp::os::Environment::GetEnvironmentVariable(nameStr));

        return variable.empty() ? NULL : il2cpp::vm::String::New(variable.c_str());
    }

    Il2CppString * Environment::GetOSVersionString(void)
    {
        return il2cpp::vm::String::New(il2cpp::os::Environment::GetOsVersionString().c_str());
    }

    int Environment::get_Platform(void)
    {
#ifdef _MSC_VER
        return 2;
#else
        return 4;
#endif
    }

    int32_t Environment::get_ExitCode()
    {
        return exitcode;
    }

    void Environment::set_ExitCode(int32_t value)
    {
        exitcode = value;
    }

    bool Environment::get_HasShutdownStarted()
    {
        return vm::Runtime::IsShuttingDown();
    }

    Il2CppString* Environment::get_EmbeddingHostName()
    {
        return il2cpp::vm::String::New("IL2CPP");
    }

    Il2CppString* Environment::get_UserName()
    {
        return il2cpp::vm::String::New(il2cpp::os::Environment::GetOsUserName().c_str());
    }

    void Environment::Exit(int32_t exitCode)
    {
        il2cpp::os::Environment::Exit(exitCode);
    }

    Il2CppString* Environment::internalGetGacPath()
    {
        // Not used by the runtime. Used only by the Mono compiler (mcs).
        NOT_IMPLEMENTED_ICALL(Environment::internalGetGacPatH);

        return 0;
    }

#if NET_4_0
    bool Environment::GetIs64BitOperatingSystem()
    {
        if (sizeof(void*) == 8)
            return true;
        return il2cpp::os::Environment::Is64BitOs();
    }

    int32_t Environment::GetPageSize()
    {
        return IL2CPP_PAGE_SIZE;
    }

    Il2CppString* Environment::GetNewLine()
    {
        return get_NewLine();
    }

    Il2CppString* Environment::internalGetEnvironmentVariable_native(Il2CppIntPtr variablePtr)
    {
        const char *value = (char*)variablePtr.m_value;

        if (!value)
            return NULL;

        const std::string nameStr(value);
        const std::string variable(il2cpp::os::Environment::GetEnvironmentVariable(nameStr));

        return variable.empty() ? NULL : il2cpp::vm::String::New(variable.c_str());
    }

    Il2CppString* Environment::get_bundled_machine_config()
    {
        NOT_IMPLEMENTED_ICALL(Environment::get_bundled_machine_config);
        IL2CPP_UNREACHABLE;
    }

#endif
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
