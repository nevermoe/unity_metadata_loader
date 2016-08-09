#pragma once
#include "il2cpp-config.h"

struct Il2CppString;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace Mono
{

class LIBIL2CPP_CODEGEN_API Runtime
{
public:
	static void mono_runtime_install_handlers ();
	static Il2CppString* GetDisplayName ();
};

} /* namespace Mono */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
