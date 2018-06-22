#if ENABLE_UNIT_TESTS

#include "UnitTest++.h"

#include "../TimeZone-c-api.h"
#include "../../TimeZone.h"


SUITE(TimeZoneTests)
{
    static const int32_t INVALID_YEAR = -45;
    static const int32_t VALID_YEAR = 2015;

    TEST(TimeZoneResultMatchesClassResultValidYear)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(result_class, result_c_api);
    }

    TEST(TimeZoneResultMatchesClassResultInValidYear)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(INVALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(INVALID_YEAR, data_class, names_class);

        CHECK_EQUAL(result_class, result_c_api);
    }

    TEST(TimeZoneStartOfDayLightSavingsValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(data_class[0], data_c_api[0]);
    }

    TEST(TimeZoneEndOfDayLightSavingsValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(data_class[1], data_c_api[1]);
    }

    TEST(TimeZoneUTFOffsetValidYear)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);

        CHECK(data_c_api[2] < 1L);
    }

    TEST(TimeZoneUTFOffSetValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(data_class[2], data_c_api[2]);
    }

    TEST(TimeZoneAdditionalOffSetValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(data_class[3], data_c_api[3]);
    }

    TEST(TimeZoneNameNotDayLighttValidYear)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);

        CHECK(strlen(names_c_api[0]) > 1);
    }

    TEST(TimeZoneNameNotDayLightValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(names_class[0].c_str(), names_c_api[0]);
    }

    TEST(TimeZoneNameDayLighttValidYear)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);

        CHECK(strlen(names_c_api[1]) > 1);
    }

    TEST(TimeZoneNameDayLightValidYearMatchesClass)
    {
        int64_t data_c_api[4];
        const char* names_c_api[2];
        bool result_c_api;

        int64_t data_class[4];
        std::string names_class[2];
        bool result_class;

        result_c_api = UnityPalGetTimeZoneData(VALID_YEAR, data_c_api, names_c_api);
        result_class = il2cpp::os::TimeZone::GetTimeZoneData(VALID_YEAR, data_class, names_class);

        CHECK_EQUAL(names_class[1].c_str(), names_c_api[1]);
    }
}

#endif // ENABLE_UNIT_TESTS
