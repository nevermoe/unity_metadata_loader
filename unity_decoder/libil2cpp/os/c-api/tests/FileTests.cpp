#if ENABLE_UNIT_TESTS

#include "UnitTest++.h"

#include "../../File.h"
#include "../../Process.h"
#include "../File-c-api.h"

static const char* TEST_FILE_NAME = "TESTFILE2";
static const char* DUPLICATE_TEST_FILE_NAME = "DUP_TESTFILE2";
static const char* BACKUP_TEST_FILE_NAME = "BACKUP_TESTFILE2";
static const int64_t TEST_FILE_LENGTH = 1234567891L;

static il2cpp::os::FileHandle*  PrepareTestFile()
{
    int error;
    il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, 1, 0, 0, 0, &error);

    return handle;
}

static void CleanupTestFile(il2cpp::os::FileHandle* handle)
{
    int error;
    il2cpp::os::File::Close(handle, &error);
    il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);
}

static void WriteSomeCharactersToTestFile(il2cpp::os::FileHandle* handle)
{
    static const char* buffer = "THIS IS A TEST\0";
    int error;

    il2cpp::os::File::Write(handle, buffer, (int)strlen(buffer), &error);
}

SUITE(FileTests)
{
    TEST(FileIsAttyWithValidButNoTTY)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();

        bool result = UnityPalIsatty(handle);

        CleanupTestFile(handle);

        CHECK_MSG(!result, "A normal is a TTY, which is not expected.");
    }

    TEST(FileIsAttyMatchesClass)
    {
        il2cpp::os::FileHandle* handle = il2cpp::os::File::GetStdInput();

        bool result = UnityPalIsatty(handle);

        CHECK_EQUAL(il2cpp::os::File::Isatty(handle), UnityPalIsatty(handle));
    }

    TEST(FileIsAttyWithValidButNoTTYMatchesClass)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();

        bool result = UnityPalIsatty(handle);

        CHECK_EQUAL(il2cpp::os::File::Isatty(handle), UnityPalIsatty(handle));

        CleanupTestFile(handle);
    }

    TEST(FileOpenTestNoError)
    {
        const char* FILE_NAME = "TESTFILE2";
        int error;
        UnityPalFileHandle* handle = NULL;

        handle = UnityPalOpen(FILE_NAME, 1, 0, 0, 0, &error);

        CHECK_NOT_NULL(handle);

        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(FILE_NAME, &error);
    }

    TEST(FileOpenTestWithError)
    {
        int error = 0;
        UnityPalFileHandle* handle = NULL;
        handle = UnityPalOpen("", 1, 0, 0, 0, &error);

        CHECK_EQUAL(il2cpp::os::kErrorCodePathNotFound, error);
    }

    TEST(FileOpenWithErrorMatchesClassTest)
    {
        int api_error = 0;
        UnityPalFileHandle* api_handle = NULL;
        int class_error = 0;
        UnityPalFileHandle* class_handle = NULL;

        api_handle = UnityPalOpen("", 1, 0, 0, 0, &api_error);
        class_handle = il2cpp::os::File::Open("", 1, 0, 0, 0, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(GetStdInputTest)
    {
        CHECK_NOT_NULL(UnityPalGetStdInput());
    }

    TEST(GetStdOutputTest)
    {
        CHECK_NOT_NULL(UnityPalGetStdOutput());
    }

    TEST(GetStdErrorTest)
    {
        CHECK_NOT_NULL(UnityPalGetStdError());
    }

    TEST(GetFileTypeNormalTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();

        FileType testFileType = UnityPalGetFileType(handle);

        CleanupTestFile(handle);

        CHECK_EQUAL(kFileTypeDisk, testFileType);
    }

    TEST(GetFileTypeMatchesClass)
    {
        CHECK_EQUAL(il2cpp::os::File::GetFileType(UnityPalGetStdError()), UnityPalGetFileType(UnityPalGetStdError()));
    }

    TEST(GetFileTypeErrorEqualsClass)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);
        CHECK_EQUAL(il2cpp::os::File::GetFileType(handle), UnityPalGetFileType(handle));
    }

    TEST(GetFileAttributesCleanErrorTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        UnityPalFileAttributes attributes = UnityPalGetFileAttributes(TEST_FILE_NAME, &error);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);

        CleanupTestFile(handle);
    }

    TEST(GetFileAttributesCleanErrorMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;

        UnityPalGetFileAttributes(TEST_FILE_NAME, &api_error);
        il2cpp::os::File::GetFileAttributes(TEST_FILE_NAME, &class_error);

        CHECK_EQUAL(class_error, api_error);

        CleanupTestFile(handle);
    }

    TEST(GetFileAttributesMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;

        UnityPalFileAttributes api_attributes = UnityPalGetFileAttributes(TEST_FILE_NAME, &api_error);
        UnityPalFileAttributes class_attributes = il2cpp::os::File::GetFileAttributes(TEST_FILE_NAME, &class_error);

        CHECK_EQUAL(class_attributes, api_attributes);

        CleanupTestFile(handle);
    }

    TEST(GetFileAttributesWithBadPath)
    {
        int error;

        UnityPalFileAttributes attributes = UnityPalGetFileAttributes("sf&236732q#", &error);

        CHECK_EQUAL(-1, attributes);
    }

    TEST(GetFileAttributesWithBadPathMatchesClass)
    {
        int api_error;
        int class_error;

        UnityPalFileAttributes api_attributes = UnityPalGetFileAttributes("#23sfs#", &api_error);
        UnityPalFileAttributes class_attributes = il2cpp::os::File::GetFileAttributes("#23sfs#", &class_error);

        CHECK_EQUAL(class_attributes, api_attributes);
    }

    TEST(SetFileAttributesNormalResultTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool result = UnityPalSetFileAttributes(TEST_FILE_NAME, kFileAttributeTemporary, &error);

        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(SetFileAttributesNormalErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        UnityPalSetFileAttributes(TEST_FILE_NAME, kFileAttributeNormal,  &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(SetFileAttributesBadResultTest)
    {
        int error;

        bool result = UnityPalSetFileAttributes("234232345$$", kFileAttributeTemporary, &error);

        CHECK(!result);
    }

    TEST(SetFileAttributesBadErrorTest)
    {
        int error;

        UnityPalSetFileAttributes("234232345$$", kFileAttributeTemporary, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(SetFileAttributesNormalResultMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        CHECK_EQUAL(il2cpp::os::File::SetFileAttributes(TEST_FILE_NAME, kFileAttributeTemporary, &error), UnityPalSetFileAttributes(TEST_FILE_NAME, kFileAttributeTemporary, &error));

        CleanupTestFile(handle);
    }

    TEST(SetFileAttributesNormalAttributeReadMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        UnityPalSetFileAttributes(TEST_FILE_NAME, kFileAttributeTemporary, &error);
        UnityPalFileAttributes api_attributes = UnityPalGetFileAttributes(TEST_FILE_NAME, &error);
        CleanupTestFile(handle);

        handle = PrepareTestFile();
        il2cpp::os::File::SetFileAttributes(TEST_FILE_NAME, kFileAttributeTemporary, &error);
        UnityPalFileAttributes class_attributes = il2cpp::os::File::GetFileAttributes(TEST_FILE_NAME, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(api_attributes, class_attributes);
    }

    TEST(SetFileAttributesBadResultMatchesClassTest)
    {
        int error;
        CHECK_EQUAL(il2cpp::os::File::SetFileAttributes("234232345$$", kFileAttributeTemporary, &error), UnityPalSetFileAttributes("234232345$$", kFileAttributeTemporary, &error));
    }

    TEST(SetFileAttributesBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;

        UnityPalSetFileAttributes("234232345$$", kFileAttributeTemporary, &api_error);
        il2cpp::os::File::SetFileAttributes("234232345$$", kFileAttributeTemporary, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(GetFileStatNormalResultTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        bool result = UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(GetFileStatNormalErrorTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(GetFileStatBadResultTest)
    {
        int error;
        UnityPalFileStat fileStat;

        bool result = UnityPalGetFileStat("#Q23423", &fileStat, &error);

        CHECK(!result);
    }

    TEST(GetFileStatBadErrorTest)
    {
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat("#Q23423", &fileStat, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }


    TEST(GetFileStatNormalNameTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(0, strcmp(fileStat.name.c_str(), TEST_FILE_NAME));
    }

    TEST(GetFileStatNormalLengthTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(0, fileStat.length);
    }

    TEST(GetFileStatNormalCreationTimeTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK(fileStat.creation_time > 100000000);
    }

    TEST(GetFileStatNormalLastAccessTimeTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK(fileStat.last_access_time > 100000000);
    }

    TEST(GetFileStatNormalLastWriteTimeTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK(fileStat.last_write_time > 100000000);
    }

    TEST(GetFileStatNormalResultMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        CHECK_EQUAL(il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &fileStat, &error), UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error));

        CleanupTestFile(handle);
    }

    TEST(GetFileStatNormalErrorMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &api_error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &fileStat, &class_error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(GetFileStatBadResultMatchesClassTest)
    {
        int error;
        UnityPalFileStat fileStat;

        CHECK_EQUAL(il2cpp::os::File::GetFileStat("#Q23423", &fileStat, &error), UnityPalGetFileStat("#Q23423", &fileStat, &error));
    }

    TEST(GetFileStatBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat("#Q23423", &fileStat, &api_error);
        il2cpp::os::File::GetFileStat("#Q23423", &fileStat, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }


    TEST(GetFileStatNormalNameMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(0, strcmp(fileStat.name.c_str(), TEST_FILE_NAME));
    }

    TEST(GetFileStatNormalAttributesMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat apiFileStat;
        il2cpp::os::FileStat classFileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &apiFileStat, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &classFileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(classFileStat.attributes, apiFileStat.attributes);
    }

    TEST(GetFileStatNormalLengthMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat apiFileStat;
        il2cpp::os::FileStat classFileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &apiFileStat, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &classFileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(classFileStat.length, apiFileStat.length);
    }

    TEST(GetFileStatNormalCreationTimeMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat apiFileStat;
        il2cpp::os::FileStat classFileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &apiFileStat, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &classFileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(classFileStat.creation_time, apiFileStat.creation_time);
    }

    TEST(GetFileStatNormalLastAccessTimeMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat apiFileStat;
        il2cpp::os::FileStat classFileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &apiFileStat, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &classFileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(classFileStat.last_access_time, apiFileStat.last_access_time);
    }

    TEST(GetFileStatNormalLastWriteTimeMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat apiFileStat;
        il2cpp::os::FileStat classFileStat;

        UnityPalGetFileStat(TEST_FILE_NAME, &apiFileStat, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &classFileStat, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(classFileStat.last_write_time, apiFileStat.last_write_time);
    }

    TEST(CopyFileNormalResultTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool result = UnityPalCopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(CopyFileNormalErrorTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        UnityPalCopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }


    TEST(CopyFileBadResultTest)
    {
        int error;
        bool result = UnityPalCopyFile("#453453", "#sdfsdw3", true, &error);

        CHECK(!result);
    }

    TEST(CopyFileBadErrorTest)
    {
        int error;
        UnityPalCopyFile("#453453", "#sdfsdw3", true, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(CopyFileNormalStatTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalCopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &error);
        UnityPalGetFileStat(DUPLICATE_TEST_FILE_NAME, &fileStat, &error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &error);
        CleanupTestFile(handle);

        CHECK(fileStat.last_write_time > 100000000);
    }

    TEST(CopyFileNormalResultMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool api_result = UnityPalCopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &error);

        bool class_result = il2cpp::os::File::CopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &error);
        il2cpp::os::File::DeleteFile(DUPLICATE_TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(CopyFileNormalErrorMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;

        UnityPalCopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &api_error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &api_error);
        il2cpp::os::File::CopyFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, true, &class_error);
        il2cpp::os::File::DeleteFile(DUPLICATE_TEST_FILE_NAME, &class_error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(CopyFileBadResultMatchesClassTest)
    {
        int error;
        CHECK_EQUAL(il2cpp::os::File::CopyFile("#453453", "#sdfsdw3", true, &error), UnityPalCopyFile("#453453", "#sdfsdw3", true, &error));
    }

    TEST(CopyFileBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        UnityPalCopyFile("#453453", "#sdfsdw3", true, &api_error);
        il2cpp::os::File::CopyFile("#453453", "#sdfsdw3", true, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(MoveFileBadResultTest)
    {
        int error;
        bool result = UnityPalMoveFile("#453453", "#sdfsdw3", &error);

        CHECK(!result);
    }

    TEST(MoveFileBadErrorTest)
    {
        int error;
        UnityPalMoveFile("#453453", "#sdfsdw3", &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(MoveFileNormalResultMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool api_result = UnityPalMoveFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, &error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &error);
        CleanupTestFile(handle);
        handle = PrepareTestFile();
        bool class_result = il2cpp::os::File::MoveFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, &error);
        il2cpp::os::File::DeleteFile(DUPLICATE_TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(MoveFileNormalErrorMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;

        UnityPalMoveFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, &api_error);
        UnityPalDeleteFile(DUPLICATE_TEST_FILE_NAME, &api_error);
        CleanupTestFile(handle);
        handle = PrepareTestFile();
        il2cpp::os::File::MoveFile(TEST_FILE_NAME, DUPLICATE_TEST_FILE_NAME, &class_error);
        il2cpp::os::File::DeleteFile(DUPLICATE_TEST_FILE_NAME, &class_error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(MoveFileBadResultMatchesClassTest)
    {
        int error;
        CHECK_EQUAL(il2cpp::os::File::MoveFile("#453453", "#sdfsdw3", &error), UnityPalMoveFile("#453453", "#sdfsdw3", &error));
    }

    TEST(MoveFileBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        UnityPalMoveFile("#453453", "#sdfsdw3", &api_error);
        il2cpp::os::File::MoveFile("#453453", "#sdfsdw3", &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(DeleteFileNormalResultTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool result = UnityPalDeleteFile(TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(DeleteFileNormalErrorTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        UnityPalDeleteFile(TEST_FILE_NAME, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }


    TEST(DeleteFileBadResultTest)
    {
        int error;
        bool result = UnityPalDeleteFile("#453453", &error);

        CHECK(!result);
    }


    TEST(DeleteFileBadErrorTest)
    {
        int error;
        UnityPalDeleteFile("#453453", &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(DeleteFileNormalStatTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;
        UnityPalFileStat fileStat;

        UnityPalDeleteFile(TEST_FILE_NAME, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);
        CleanupTestFile(handle);

        CHECK(fileStat.last_write_time < 1);
    }

    TEST(DeleteFileNormalResultMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int error;

        bool api_result = UnityPalDeleteFile(TEST_FILE_NAME, &error);
        CleanupTestFile(handle);
        handle = PrepareTestFile();
        bool class_result = il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(DeleteFileNormalErrorMatchesClassTest)
    {
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int api_error;
        int class_error;

        UnityPalDeleteFile(TEST_FILE_NAME, &api_error);
        CleanupTestFile(handle);
        handle = PrepareTestFile();
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &class_error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(DeleteFileBadResultMatchesClassTest)
    {
        int error;
        CHECK_EQUAL(il2cpp::os::File::DeleteFile("#453453", &error), UnityPalDeleteFile("#453453", &error));
    }

    TEST(DeleteFileBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        UnityPalDeleteFile("#453453", &api_error);
        il2cpp::os::File::DeleteFile("#453453", &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(ReplaceFileTest)
    {
        // TODO Figure out how to test
        // Replace File does not seem to work in libil2cpp
    }

    TEST(OpenFileNormalResultTest)
    {
        int error;
        UnityPalFileHandle* handle = NULL;

        handle =  UnityPalOpen(TEST_FILE_NAME, 1, 0, 0, 0, &error);

        CHECK_NOT_NULL(handle);

        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);
    }

    TEST(OpenFileNormalErrorTest)
    {
        int error;
        UnityPalFileHandle* handle = NULL;

        handle = UnityPalOpen(TEST_FILE_NAME, 1, 0, 0, 0, &error);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);

        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);
    }

    TEST(OpenFileNormalStatTest)
    {
        int error;
        UnityPalFileStat fileStat;
        UnityPalFileHandle* handle = NULL;

        handle = UnityPalOpen(TEST_FILE_NAME, 1, 0, 0, 0, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);
        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);

        CHECK(fileStat.last_write_time > 1);
    }

    TEST(OpenFileNormalErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;
        UnityPalFileHandle* handle = NULL;

        handle = UnityPalOpen(TEST_FILE_NAME, 1, 0, 0, 0, &api_error);
        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, 1, 0, 0, 0, &class_error);
        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(OpenFileNormalStatMatchesClassTest)
    {
        int error;
        UnityPalFileStat api_fileStat;
        il2cpp::os::FileStat class_fileStat;
        UnityPalFileHandle* handle = NULL;

        handle = UnityPalOpen(TEST_FILE_NAME, 1, 0, 0, 0, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &api_fileStat, &error);
        il2cpp::os::File::Close(handle, &error);
        il2cpp::os::File::DeleteFile(TEST_FILE_NAME, &error);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, 1, 0, 0, 0, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &class_fileStat, &error);
        il2cpp::os::File::Close(handle, &error);


        // Tthey dont have to be exact, just in the ballpark
        CHECK_EQUAL(class_fileStat.last_write_time / 1000000L, api_fileStat.last_write_time / 1000000L);
    }

    TEST(SetFileTimeNormalTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate,  kFileAccessReadWrite, 0, 0, &error);
        UnityPalFileStat fileStat;
        bool result = UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);
        CleanupTestFile(handle);

        CHECK(result);
    }
/*
    TEST(SetFileTimeNormalStatTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        UnityPalFileStat fileStat;
        bool result = UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &fileStat, &error);
        CleanupTestFile(handle);

        CHECK(fileStat.creation_time == 131360602701336952 && fileStat.last_write_time == 131360602701336952 && fileStat.last_access_time == 131360602701336952);
    }
*/
    TEST(SetFileTimeNormalErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        bool result = UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(SetFileTimeNormalMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        UnityPalFileStat fileStat;
        bool api_result = UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        bool class_result = il2cpp::os::File::SetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(SetFileTimeNormalStatMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        UnityPalFileStat api_fileStat;
        UnityPalFileStat class_fileStat;

        UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        UnityPalGetFileStat(TEST_FILE_NAME, &api_fileStat, &error);

        il2cpp::os::File::SetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &error);
        il2cpp::os::File::GetFileStat(TEST_FILE_NAME, &class_fileStat, &error);

        CleanupTestFile(handle);

        CHECK(api_fileStat.creation_time == class_fileStat.creation_time && api_fileStat.last_write_time == class_fileStat.last_write_time && api_fileStat.last_access_time == class_fileStat.last_access_time);
    }

    TEST(SetFileTimeNormalErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        UnityPalSetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &api_error);
        il2cpp::os::File::SetFileTime(handle, 131360602701336952, 131360602701336952, 131360602701336952, &class_error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(GetLengthNormalTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        int64_t length = UnityPalGetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(14, length);
    }

    TEST(GetLengthZeroTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int64_t length = UnityPalGetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(0, length);
    }

    TEST(GetLengthBadFileErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);
        UnityPalGetLength(handle, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(GetLengthNormalMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        int64_t api_length = UnityPalGetLength(handle, &error);
        int64_t class_length = il2cpp::os::File::GetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_length, api_length);
    }

    TEST(GetLengthZeroMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        int64_t api_length = UnityPalGetLength(handle, &error);
        int64_t class_length = il2cpp::os::File::GetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_length, api_length);
    }

    TEST(GetLengthBadFileErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);
        UnityPalGetLength(handle, &api_error);
        il2cpp::os::File::GetLength(handle, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(SetLengthNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        bool result = UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);

        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(SetLengthNormalGetLengthTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        bool result = UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);
        int64_t length = il2cpp::os::File::GetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(TEST_FILE_LENGTH, length);
    }

    TEST(SetLengthBadResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);

        bool result = UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);

        CHECK(!result);
    }

    TEST(SetLengthBadErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);

        UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(SetLengthNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        bool api_result = UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);

        CleanupTestFile(handle);
        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        bool class_result = il2cpp::os::File::SetLength(handle, TEST_FILE_LENGTH, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(SetLengthNormalGetLengthMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);
        int64_t api_length = il2cpp::os::File::GetLength(handle, &error);
        CleanupTestFile(handle);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        il2cpp::os::File::SetLength(handle, TEST_FILE_LENGTH, &error);
        int64_t class_length = il2cpp::os::File::GetLength(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_length, api_length);
    }

    TEST(SetLengthBadResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);

        bool api_result = UnityPalSetLength(handle, TEST_FILE_LENGTH, &error);
        bool class_result = il2cpp::os::File::SetLength(handle, TEST_FILE_LENGTH, &error);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(SetLengthBadErrorMatchesClassTest)
    {
        int api_error;
        int class_error;
        il2cpp::os::FileHandle* handle = PrepareTestFile();
        CleanupTestFile(handle);

        UnityPalSetLength(handle, TEST_FILE_LENGTH, &api_error);
        il2cpp::os::File::SetLength(handle, TEST_FILE_LENGTH, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(SeekNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        int64_t result = UnityPalSeek(handle, 10, 0, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(10, result);
    }

    TEST(SeekNormalBufferTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        int64_t result = UnityPalSeek(handle, 10, 0, &error);
        char buffer[15];
        il2cpp::os::File::Read(handle, buffer, 4, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(0, strncmp("TEST", buffer, 4));
    }

    TEST(SeekBadErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);

        UnityPalSeek(handle, 10, 0, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(SeekNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        int64_t api_result = UnityPalSeek(handle, 10, 0, &error);
        CleanupTestFile(handle);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        int64_t class_result = il2cpp::os::File::Seek(handle, 10, 0, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(SeekNormalBufferMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        UnityPalSeek(handle, 10, 0, &error);
        char api_buffer[15];
        char class_buffer[15];
        il2cpp::os::File::Read(handle, api_buffer, 4, &error);

        CleanupTestFile(handle);


        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        il2cpp::os::File::Seek(handle, 10, 0, &error);
        il2cpp::os::File::Read(handle, class_buffer, 4, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(0, strncmp(class_buffer, api_buffer, 4));
    }

    TEST(SeekBadErrorMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);

        UnityPalSeek(handle, 10, 0, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(ReadNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        char buffer[16];
        UnityPalSeek(handle, 0, 0, &error);
        int result = UnityPalRead(handle, buffer, 16, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(14, result);
    }

    TEST(ReadNormalBufferTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        char buffer[16];
        UnityPalSeek(handle, 0, 0, &error);
        UnityPalRead(handle, buffer, 16, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(0, strncmp("THIS IS A TEST", buffer, 14));
    }

    TEST(ReadBadResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);
        char buffer[16];
        int64_t result = UnityPalRead(handle, buffer, 16, &error);

        CHECK(!result);
    }

    TEST(ReadBadErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);
        char buffer[16];
        UnityPalRead(handle, buffer, 16, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(ReadNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        UnityPalSeek(handle, 0, 0, &error);
        char buffer[16];
        int api_result = UnityPalRead(handle, buffer, 14, &error);
        CleanupTestFile(handle);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        il2cpp::os::File::Seek(handle, 0, 0, &error);
        int class_result = il2cpp::os::File::Read(handle, buffer, 14, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(ReadNormalBufferMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        UnityPalSeek(handle, 0, 0, &error);
        char api_buffer[16];
        char class_buffer[16];
        UnityPalRead(handle, api_buffer, 14, &error);
        CleanupTestFile(handle);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        il2cpp::os::File::Seek(handle, 0, 0, &error);
        il2cpp::os::File::Read(handle, class_buffer, 14, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(0, strncmp(class_buffer, api_buffer, 14));
    }


    TEST(ReadBadResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);

        char buffer[16];

        int64_t api_result = UnityPalRead(handle, buffer, 14, &error);
        int64_t class_result = il2cpp::os::File::Read(handle, buffer, 14, &error);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(ReadBadErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;

        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);

        char buffer[16];

        UnityPalRead(handle, buffer, 14, &api_error);
        il2cpp::os::File::Read(handle, buffer, 14, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(WriteNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        static const char* buffer = "THIS IS A TEST\0";

        int32_t result = UnityPalWrite(handle, buffer, 14, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(14, result);
    }

    TEST(WriteNormalErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        static const char* buffer = "THIS IS A TEST\0";

        UnityPalWrite(handle, buffer, 14, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(WriteNormalBufferCheckTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        static const char* buffer = "THIS IS A TEST\0";
        UnityPalWrite(handle, buffer, 14, &error);

        char read_buffer[16];
        il2cpp::os::File::Seek(handle, 0, 0, &error);
        il2cpp::os::File::Read(handle, read_buffer, 16, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(0, strncmp("THIS IS A TEST", buffer, 14));
    }

    TEST(WriteBadResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);
        static const char* buffer = "THIS IS A TEST\0";

        int32_t result = UnityPalWrite(handle, buffer, 14, &error);


        CHECK_EQUAL(0, result);
    }

    TEST(WriteNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        static const char* buffer = "THIS IS A TEST\0";

        int32_t api_result = UnityPalWrite(handle, buffer, 14, &error);
        int32_t class_result = il2cpp::os::File::Write(handle, buffer, 14, &error);

        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(WriteNormalBufferCheckMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        static const char* buffer = "THIS IS A TEST\0";
        UnityPalWrite(handle, buffer, 14, &error);

        char api_read_buffer[16];
        il2cpp::os::File::Seek(handle, 0, 0, &error);
        il2cpp::os::File::Read(handle, api_read_buffer, 16, &error);
        CleanupTestFile(handle);

        handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        il2cpp::os::File::Write(handle, buffer, 14, &error);
        char class_read_buffer[16];
        il2cpp::os::File::Seek(handle, 0, 0, &error);
        il2cpp::os::File::Read(handle, class_read_buffer, 16, &error);
        CleanupTestFile(handle);


        CHECK_EQUAL(0, strncmp(class_read_buffer, api_read_buffer, 14));
    }

    TEST(WriteBadResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        CleanupTestFile(handle);
        static const char* buffer = "THIS IS A TEST\0";

        int32_t api_result = UnityPalWrite(handle, buffer, 14, &error);
        int32_t class_result = il2cpp::os::File::Write(handle, buffer, 14, &error);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(WriteBadErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;

        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, 1, 1, 0, 0, &error);
        CleanupTestFile(handle);
        static const char* buffer = "THIS IS A TEST\0";

        UnityPalWrite(handle, buffer, 14, &api_error);
        il2cpp::os::File::Write(handle, buffer, 14, &class_error);

        CHECK_EQUAL(api_error, class_error);
    }

    TEST(FlushNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        bool result = UnityPalFlush(handle, &error);
        CleanupTestFile(handle);

        CHECK(result);
    }

    TEST(FlushNormalErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);

        UnityPalFlush(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }

    TEST(FlushBadResultTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        CleanupTestFile(handle);

        bool result = UnityPalFlush(handle, &error);

        CHECK(!result);
    }

    TEST(FlushBadErrorTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        CleanupTestFile(handle);

        UnityPalFlush(handle, &error);

        CHECK_NOT_EQUAL(il2cpp::os::kErrorCodeSuccess, error);
    }


    TEST(FlushNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        WriteSomeCharactersToTestFile(handle);
        bool api_result = UnityPalFlush(handle, &error);
        WriteSomeCharactersToTestFile(handle);
        bool class_result = il2cpp::os::File::Flush(handle, &error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_result, api_result);
    }

    TEST(FlushNormalErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);

        WriteSomeCharactersToTestFile(handle);
        UnityPalFlush(handle, &api_error);
        WriteSomeCharactersToTestFile(handle);
        il2cpp::os::File::Flush(handle, &class_error);
        CleanupTestFile(handle);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(FlushBadResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        CleanupTestFile(handle);

        bool api_result = UnityPalFlush(handle, &error);
        bool class_result = il2cpp::os::File::Flush(handle, &error);

        CHECK_EQUAL(api_result, class_result);
    }

    TEST(FlushBadErrorMatchesClassTest)
    {
        int error;
        int api_error;
        int class_error;

        il2cpp::os::FileHandle* handle = il2cpp::os::File::Open(TEST_FILE_NAME, kFileModeOpenOrCreate, kFileAccessReadWrite, 0, 0, &error);
        WriteSomeCharactersToTestFile(handle);
        CleanupTestFile(handle);

        UnityPalFlush(handle, &api_error);
        il2cpp::os::File::Flush(handle, &class_error);

        CHECK_EQUAL(class_error, api_error);
    }

    TEST(CreatePipeNormalResultTest)
    {
        int error;
        il2cpp::os::FileHandle* read_handle;
        il2cpp::os::FileHandle* write_handle;

        bool result = UnityPalCreatePipe(&read_handle, &write_handle);
        il2cpp::os::File::Close(read_handle, &error);
        il2cpp::os::File::Close(write_handle, &error);

        CHECK(result);
    }

    TEST(CreatePipeNormalBufferTest)
    {
        int error;
        il2cpp::os::FileHandle* read_handle;
        il2cpp::os::FileHandle* write_handle;

        UnityPalCreatePipe(&read_handle, &write_handle);

        WriteSomeCharactersToTestFile(write_handle);
        char buffer[16];
        il2cpp::os::File::Read(read_handle, buffer, 4, &error);

        il2cpp::os::File::Close(read_handle, &error);
        il2cpp::os::File::Close(write_handle, &error);

        CHECK_EQUAL(0, strncmp("THIS", buffer, 4));
    }

    TEST(CreatePipeNormalResultMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* read_handle;
        il2cpp::os::FileHandle* write_handle;

        bool api_result = UnityPalCreatePipe(&read_handle, &write_handle);
        il2cpp::os::File::Close(read_handle, &error);
        il2cpp::os::File::Close(write_handle, &error);

        bool class_result = il2cpp::os::File::CreatePipe(&read_handle, &write_handle);
        il2cpp::os::File::Close(read_handle, &error);
        il2cpp::os::File::Close(write_handle, &error);

        CHECK_EQUAL(class_result, api_result);
    }


    TEST(CreatePipeNormalBufferMatchesClassTest)
    {
        int error;
        il2cpp::os::FileHandle* api_read_handle;
        il2cpp::os::FileHandle* api_write_handle;
        il2cpp::os::FileHandle* class_read_handle;
        il2cpp::os::FileHandle* class_write_handle;

        UnityPalCreatePipe(&api_read_handle, &api_write_handle);

        WriteSomeCharactersToTestFile(api_write_handle);
        char api_buffer[16];
        il2cpp::os::File::Read(api_read_handle, api_buffer, 4, &error);
        il2cpp::os::File::Close(api_read_handle, &error);
        il2cpp::os::File::Close(api_write_handle, &error);

        il2cpp::os::File::CreatePipe(&class_read_handle, &class_write_handle);
        WriteSomeCharactersToTestFile(class_write_handle);
        char class_buffer[16];
        il2cpp::os::File::Read(class_read_handle, class_buffer, 4, &error);
        il2cpp::os::File::Close(class_read_handle, &error);
        il2cpp::os::File::Close(class_write_handle, &error);

        CHECK_EQUAL(0, strncmp(class_buffer, api_buffer, 4));
    }
}

#endif // ENABLE_UNIT_TESTS
