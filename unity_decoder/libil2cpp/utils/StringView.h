#pragma once
#include <string>

namespace il2cpp
{
namespace utils
{
    template<typename CharType>
    class StringView
    {
    private:
        const CharType* m_String;
        size_t m_Length;

        // Intended to only be used by Empty()
        inline StringView() :
            m_String(NULL),
            m_Length(0)
        {
        }

    public:
        template<size_t Length>
        inline StringView(const CharType(&str)[Length]) :
            m_String(str), m_Length(Length - 1)
        {
        }

        inline StringView(const CharType* str, size_t length) :
            m_String(str), m_Length(length)
        {
            IL2CPP_ASSERT(str != NULL);
        }

        inline StringView(const StringView<CharType>& str, size_t startIndex, size_t length) :
            m_String(str.Str() + startIndex),
            m_Length(length)
        {
            IL2CPP_ASSERT(startIndex + length <= str.Length());
        }

        template<typename CharTraits, typename StringAlloc>
        inline StringView(const std::basic_string<CharType, CharTraits, StringAlloc>& str) :
            m_String(str.c_str()), m_Length(str.length())
        {
        }

        // This will prevent accidentally assigning temporary values (like function return values)
        // to a string view. While this protection will only be enabled on C++11 compiles, even those
        // are enough to catch the bug in our runtime
#if IL2CPP_HAS_DELETED_FUNCTIONS
        template<typename CharTraits, typename StringAlloc>
        StringView(std::basic_string<CharType, CharTraits, StringAlloc>&&) = delete;
#endif

        inline const CharType* Str() const
        {
            return m_String;
        }

        inline size_t Length() const
        {
            return m_Length;
        }

        inline CharType operator[](size_t index) const
        {
            return m_String[index];
        }

        inline bool IsNullTerminated() const
        {
            return m_String[m_Length] == 0;
        }

        inline bool IsEmpty() const
        {
            return Length() == 0;
        }

        static inline StringView<CharType> Empty()
        {
            return StringView<CharType>();
        }

        inline size_t RFind(CharType c) const
        {
            for (const CharType* ptr = m_String + m_Length - 1; ptr >= m_String; ptr--)
            {
                if (*ptr == c)
                    return ptr - m_String;
            }

            return NPos();
        }

        inline static size_t NPos()
        {
            return static_cast<size_t>(-1);
        }
    };

#define StringViewAsNullTerminatedStringOf(CharType, stringView, variableName) \
    const CharType* variableName; \
    do \
    { \
        if (stringView.IsNullTerminated()) \
        { \
            variableName = stringView.Str(); \
        } \
        else \
        { \
            CharType* buffer = static_cast<CharType*>(alloca((stringView.Length() + 1) * sizeof(CharType))); \
            memcpy(buffer, stringView.Str(), stringView.Length() * sizeof(CharType)); \
            buffer[stringView.Length()] = 0; \
            variableName = buffer; \
        } \
    } \
    while (false)
}
}
