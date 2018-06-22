#if ENABLE_UNIT_TESTS

#include "UnitTest++.h"
#include "il2cpp-config.h"
#include "../Allocator.h"

// Have to define this due to Unity's customized
// version of UNitTest++ that integrates into the Unity
// editor.

extern "C"
{
void printf_console_log(const char* log, va_list list) {}
}

// We can't use Il2CppNativeChar here due to an old bug in clang handling typdefs in main.
#if _MSC_VER
int main(int argc, const wchar_t* const argv[])
#else
int main(int argc, const char* const argv[])
#endif
{
    register_allocator(malloc);
    return UnitTest::RunAllTests();
}

#if IL2CPP_TARGET_WINDOWS

#if IL2CPP_TARGET_WINDOWS_DESKTOP
#include <windows.h>
#else
#include "ActivateApp.h"
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
#if IL2CPP_TARGET_WINDOWS_DESKTOP
    int argc;
    wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    int returnValue = main(argc, argv);
    LocalFree(argv);
    return returnValue;
#else
    return WinRT::Activate(main);
#endif
}

#endif

#endif // ENABLE_UNIT_TESTS
