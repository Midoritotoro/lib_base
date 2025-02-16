#pragma once 

#include <optional>

#include <base/system/Platform.h>
#include <base/utility/CommonMacros.h>

#include <base/utility/Flags.h>

#include <base/string/Char.h>
#include <cstring>

#include <base/io/ByteArrayView.h>


namespace base::string {
    using StringList = std::vector<Char>;

    class io::ByteArrayView;
    class Char;
    class io::ByteArrayView;
    class StringView;

    class StringConverterBase
    {
    public:
        enum class Flag {
            Default = 0,
            Stateless = 0x1,
            ConvertInvalidToNull = 0x2,
            WriteBom = 0x4,
            ConvertInitialBom = 0x8,
            UsesIcu = 0x10,
        };
        DECLARE_FLAGS(Flags, Flag)

        struct State {
            constexpr State(Flags f = Flag::Default) noexcept
                : flags(f), state_data{0, 0, 0, 0} {}
            ~State() { clear(); }

            State(State &&other) noexcept
                : flags(other.flags),
                  remainingChars(other.remainingChars),
                  invalidChars(other.invalidChars),
                  state_data{other.state_data[0], other.state_data[1],
                             other.state_data[2], other.state_data[3]},
                  clearFn(other.clearFn)
            { other.clearFn = nullptr; }
            State &operator=(State &&other) noexcept
            {
                clear();
                flags = other.flags;
                remainingChars = other.remainingChars;
                invalidChars = other.invalidChars;
                std::memmove(state_data, other.state_data, sizeof state_data); // self-assignment-safe
                clearFn = other.clearFn;
                other.clearFn = nullptr;
                return *this;
            }
            void clear() noexcept;
            void reset() noexcept;

            Flags flags;
            int internalState = 0;
            sizetype remainingChars = 0;
            sizetype invalidChars = 0;

            union {
                uint state_data[4];
                void *d[2];
            };
            using ClearDataFn = void (*)(State *) noexcept;
            ClearDataFn clearFn = nullptr;
        private:
            DISABLE_COPY(State)
        };
    protected:
        ~StringConverterBase() = default;
    };

class StringConverter : public StringConverterBase
{
public:

    enum Encoding {
        Utf8,
        Utf16,
        Utf16LE,
        Utf16BE,
        Utf32,
        Utf32LE,
        Utf32BE,
        Latin1,
        System,
        LastEncoding = System
    };

protected:

    struct Interface
    {
        using DecoderFn = Char * (*)(Char *out, io::ByteArrayView in, State *state);
        using LengthFn = sizetype (*)(sizetype inLength);

        using EncoderFn = char * (*)(char *out, StringView in, State *state);
        const char *name = nullptr;

        DecoderFn toUtf16 = nullptr;
        LengthFn toUtf16Len = nullptr;

        EncoderFn fromUtf16 = nullptr;
        LengthFn fromUtf16Len = nullptr;
    };

    constexpr StringConverter() noexcept
        : iface(nullptr)
    {}
    constexpr explicit StringConverter(Encoding encoding, Flags f)
        : iface(&encodingInterfaces[qsizetype(encoding)]), state(f)
    {}
    constexpr explicit StringConverter(const Interface *i) noexcept
        : iface(i)
    {}
    explicit StringConverter(const char *name, Flags f);


    ~StringConverter() = default;

public:
    StringConverter(StringConverter &&) = default;
    StringConverter &operator=(StringConverter &&) = default;

    bool isValid() const noexcept { return iface != nullptr; }

    void resetState() noexcept
    {
        state.reset();
    }
    bool hasError() const noexcept { return state.invalidChars != 0; }

    const char *name() const noexcept;

    static std::optional<Encoding> encodingForName(const char *name) noexcept;
    static const char *nameForEncoding(Encoding e);
    static std::optional<Encoding>
    encodingForData(io::ByteArrayView data, char16_t expectedFirstCharacter = 0) noexcept;
    static std::optional<Encoding> encodingForHtml(io::ByteArrayView data);

    static StringList availableCodecs();

protected:
    const Interface *iface;
    State state;
private:
    static const Interface encodingInterfaces[Encoding::LastEncoding + 1];
};

} // namespace base::string