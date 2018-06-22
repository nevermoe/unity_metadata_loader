#include "os/TimeZone.h"
#include "Allocator.h"

#include <string>

extern "C"
{
bool UnityPalGetTimeZoneData(int32_t year, int64_t data[4], const char* names[2])
{
    std::string namesBuffer[2];
    bool result = il2cpp::os::TimeZone::GetTimeZoneData(year, data, namesBuffer);

    names[0] = Allocator::CopyToAllocatedStringBuffer(namesBuffer[0]);
    names[1] = Allocator::CopyToAllocatedStringBuffer(namesBuffer[1]);

    return result;
}
}
