#if ENABLE_UNIT_TESTS

#include "UnitTest++.h"

#include "../Path-c-api.h"
#include "../../Path.h"

SUITE(PathTests)
{
    TEST(GetTempPathNotNullTest)
    {
        CHECK_NOT_NULL(UnityPalGetTempPath());
    }

    TEST(GetTempPathSanityTest)
    {
        // Path will never be the same depeding on user, platform,machine etc.  Check if the string is
        // at least a few chars long
        CHECK(strlen(UnityPalGetTempPath()) > 5);
    }

    TEST(GetTempPathTestMatchesClassTest)
    {
        CHECK_EQUAL(il2cpp::os::Path::GetTempPath().c_str(), UnityPalGetTempPath());
    }

    TEST(GetExecutablePathNotNullTest)
    {
        CHECK_NOT_NULL(UnityPalGetExecutablePath());
    }

    TEST(GetExecutablePathSanityTest)
    {
        // Path will never be the same depeding on user, platform,machine etc.  Check if the string is
        // at least a few chars long
        CHECK(strlen(UnityPalGetExecutablePath()) > 5);
    }

    TEST(GetExecutablePathMatchesClassTest)
    {
        CHECK_EQUAL(il2cpp::os::Path::GetExecutablePath().c_str(), UnityPalGetExecutablePath());
    }
}

#endif // ENABLE_UNIT_TESTS
