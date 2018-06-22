#if ENABLE_UNIT_TESTS

#include "UnitTest++.h"

#include "../Time-c-api.h"
#include "../../Time.h"

SUITE(TimeTests)
{
    TEST(TicksMillisecondsMonotonicTestValid)
    {
        CHECK(UnityPalGetTicksMillisecondsMonotonic() > 0);
    }

    TEST(TicksMillisecondsMonotonicEqualsClassTest)
    {
        CHECK_EQUAL(il2cpp::os::Time::GetTicksMillisecondsMonotonic(), UnityPalGetTicksMillisecondsMonotonic());
    }

    TEST(Ticks100NanosecondsMonotonicTestValid)
    {
        CHECK(UnityPalGetTicks100NanosecondsMonotonic() > 0);
    }

    TEST(Ticks100NanosecondsMonotonicEqualsClassTest)
    {
        // This number is fairly sensitive to time between calls, check to see if they are close enough, chop off the last few digits
        CHECK_EQUAL(il2cpp::os::Time::GetTicks100NanosecondsMonotonic() / 1000L, UnityPalGetTicks100NanosecondsMonotonic() / 1000L);
    }

    TEST(GetTicks100NanosecondsDateTimeTest)
    {
        CHECK(UnityPalGetTicks100NanosecondsDateTime() > 0);
    }

    TEST(GetTicks100NanosecondsDateTimeEqualsClassTest)
    {
        CHECK_EQUAL(il2cpp::os::Time::GetTicks100NanosecondsDateTime() / 1000L, UnityPalGetTicks100NanosecondsDateTime() / 1000L);
    }

    TEST(GetSystemTimeAsFileTimeTest)
    {
        CHECK(UnityPalGetSystemTimeAsFileTime() > 0);
    }

    TEST(GetSystemTimeAsFileTimeEqualsClassTest)
    {
        CHECK_EQUAL(il2cpp::os::Time::GetSystemTimeAsFileTime(), UnityPalGetSystemTimeAsFileTime());
    }
}

#endif // ENABLE_UNIT_TESTS
