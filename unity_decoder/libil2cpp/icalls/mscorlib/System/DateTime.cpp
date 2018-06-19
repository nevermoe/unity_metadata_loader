#include "il2cpp-config.h"
#include <cassert>
#include "icalls/mscorlib/System/DateTime.h"
#include "os/Time.h"

using namespace il2cpp::os;

namespace il2cpp
{
namespace icalls
{
namespace mscorlib
{
namespace System
{

int64_t DateTime::GetNow(void)
{
	return Time::GetTicks100NanosecondsDateTime ();
}

int64_t DateTime::GetTimeMonotonic ()
{
	return Time::GetTicks100NanosecondsMonotonic ();
}

} /* namespace System */
} /* namespace mscorlib */
} /* namespace icalls */
} /* namespace il2cpp */
