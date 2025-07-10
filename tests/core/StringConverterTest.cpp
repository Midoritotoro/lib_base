#include <base/core/string/StringConverter.h>
#include <base/core/arch/Platform.h>
#include <cuchar>
#include <cwchar>

WARNING_DISABLE_MSVC(VCR001)

using namespace base;

enum StringAlignedSize {
    Large = 4096,
    Medium = 1024,
    Small = 512
};

#define BASE_FIXED_CHAR_ARRAY(name, prefix) \
    template <typename _Type_, size_t N>\
    struct name {\
        _Type_ data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'A' + (i % 26);\
            }\
            data[N] = prefix'\0';\
        }\
    };

#define BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(name, type, prefix) \
    template <size_t N>\
    struct name<type, N> {\
        type data[N + 1]{};\
    \
        constexpr name() {\
            for (size_t i = 0; i < N; ++i) {\
                data[i] = prefix'A' + (i % 26);\
            }\
            data[N] = prefix'\0';\
        }\
    };

BASE_FIXED_CHAR_ARRAY(FixedArray, );
#if __cpp_lib_char8_t
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char8_t, u8);
#endif
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char16_t, u);
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, char32_t, U);
BASE_ADD_SPECIALIZATION_TO_FIXED_CHAR_ARRAY(FixedArray, wchar_t, L);


template <
    typename _FromChar_,
    typename _ToChar_,
    StringAlignedSize stringAlignedSize = StringAlignedSize::Large>
class CRTStringConverterTest {
    static auto ConvertStringHelper(
        const _FromChar_* input,
        size_t length,
        _ToChar_* buffer)
    {
        if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, wchar_t>)
            return mbstowcs(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, wchar_t> && std::is_same_v<_ToChar_, char>)
            return wcstombs(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, char16_t>) {
            mbstate_t state{};

            for (size_t i = 0; i < length; ++i)
                mbrtoc16(&buffer[i], &input[i], 1, &state);
            return 0;
        }
        else if constexpr (std::is_same_v<_FromChar_, char> && std::is_same_v<_ToChar_, char8_t>)
            return memcpy(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char8_t> && std::is_same_v<_ToChar_, char>)
            return memcpy(buffer, input, length);
        else if constexpr (std::is_same_v<_FromChar_, char16_t> && std::is_same_v<_ToChar_, char>) {
            mbstate_t state{};

            for (size_t i = 0; i < length; ++i)
                c16rtomb(&buffer[i], input[i], &state);
            return 0;
        }
    }

public:
    static string::StringConversionResult<_ToChar_> ConvertString() {
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSize>{};
        _ToChar_* ch = static_cast<_ToChar_*>(memory::AllocateAligned(stringAlignedSize * sizeof(_ToChar_), 64));

        ConvertStringHelper(textArray.data, stringAlignedSize, ch);
        return string::StringConversionResult<_ToChar_>(textArray.data, stringAlignedSize, false);
    }
};

template <
    typename _FromChar_,
    typename _ToChar_,
    StringAlignedSize stringAlignedSize = StringAlignedSize::Large>
class StringConverterTest {
    static auto ConvertStringHelper(
        const _FromChar_* input,
        size_t length,
        string::StringConversionResult<_ToChar_>& result)
    {
        string::StringConverter::convertStringStore<_FromChar_, _ToChar_>(input, length, &result);
        return result;
    }

public:
    static NODISCARD bool ConvertString() {
        string::StringConversionResult<_ToChar_> result;
        static constexpr auto textArray = FixedArray<_FromChar_, stringAlignedSize>{};

        result.setData(static_cast<_ToChar_*>(memory::AllocateAligned(stringAlignedSize * sizeof(_ToChar_), 64)));
        ConvertStringHelper(textArray.data, stringAlignedSize, result);
        return true;
        //return (
          //  CRTStringConverterTest<_FromChar_, _ToChar_, stringAlignedSize>::ConvertString() == 
           // string::StringConversionResult<_ToChar_>(textArray.data, stringAlignedSize, false));
    }
};
int main(int argc, char** argv) {
    Assert((StringConverterTest<char, wchar_t, StringAlignedSize::Large>::ConvertString()));
    //StringConverterTest<char, char8_t>::ConvertString();
    //StringConverterTest<char, char16_t>::ConvertString();
    //StringConverterTest<char, char32_t>::ConvertString();

    //StringConverterTest<wchar_t, char>::ConvertString();
    //StringConverterTest<wchar_t, char8_t>::ConvertString();
    //StringConverterTest<wchar_t, char16_t>::ConvertString();
    //StringConverterTest<wchar_t, char32_t>::ConvertString();
    
    //StringConverterTest<char8_t, char>::ConvertString();
    //StringConverterTest<char8_t, wchar_t>::ConvertString();
    //StringConverterTest<char8_t, char16_t>::ConvertString();
    //StringConverterTest<char8_t, char32_t>::ConvertString();

    //StringConverterTest<char16_t, char>::ConvertString();
    //StringConverterTest<char16_t, wchar_t>::ConvertString();
    //StringConverterTest<char16_t, char8_t>::ConvertString();
    //StringConverterTest<char16_t, char32_t>::ConvertString();

    //StringConverterTest<char32_t, char>::ConvertString();
    //StringConverterTest<char32_t, wchar_t>::ConvertString();
    //StringConverterTest<char32_t, char8_t>::ConvertString();
    //StringConverterTest<char32_t, char16_t>::ConvertString();

	return 0;
}