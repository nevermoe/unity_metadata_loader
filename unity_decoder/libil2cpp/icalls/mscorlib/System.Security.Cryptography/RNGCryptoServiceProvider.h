#pragma once

#include "il2cpp-config.h"
#include "object-internals.h"

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{
namespace Security
{
namespace Cryptography
{

class LIBIL2CPP_CODEGEN_API RNGCryptoServiceProvider
{
public:
	static void RngClose (Il2CppIntPtr handle);
	static Il2CppIntPtr RngGetBytes (Il2CppIntPtr,Il2CppArray *);
	static Il2CppIntPtr RngInitialize (Il2CppArray *);
	static bool RngOpen ();
};

} /* namespace Cryptography */
} /* namespace Security */
} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
