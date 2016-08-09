#pragma once

namespace il2cpp
{

namespace utils
{

template <typename CharType>
class StringView
{
private:
	const CharType* m_String;
	size_t m_Length;

public:
	template <size_t Length>
	inline StringView(const CharType (&str)[Length]) :
		m_String(str), m_Length(Length - 1)
	{
	}

	inline StringView(const CharType* str, size_t length) :
		m_String(str), m_Length(length)
	{
		assert(str != NULL);
	}

	inline StringView(const StringView<CharType>& str, size_t startIndex, size_t length) :
		m_String(str.Str() + startIndex),
		m_Length(length)
	{
		assert(startIndex + length <= str.Length());
	}

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