#pragma once 

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <typename _Char_>
class StringConversionResult {
public:
	constexpr StringConversionResult() noexcept {}

	constexpr inline StringConversionResult(const StringConversionResult& other) noexcept :
		_dataStart(other._dataStart),
		_dataLength(other._dataLength),
		_isNarrowingConversion(other._isNarrowingConversion)
	{}

	constexpr inline StringConversionResult(
		_Char_* dataStart,
		size_t dataLength,
		bool isNarrowingConversion
	) noexcept :
		_dataStart(dataStart),
		_dataLength(dataLength),
		_isNarrowingConversion(isNarrowingConversion)
	{}

	~StringConversionResult() noexcept 
	{}

	constexpr inline NODISCARD bool isNull() const noexcept {
		return (_dataStart == nullptr || _dataLength == 0);
	}

	constexpr inline NODISCARD _Char_* data() noexcept {
		return _dataStart;
	}

	constexpr inline NODISCARD size_t length() const noexcept {
		return _dataLength;
	}

	constexpr inline NODISCARD size_t size() const noexcept {
		return _dataLength;
	}

	constexpr inline NODISCARD bool isNarrowingConversion() const noexcept {
		return _isNarrowingConversion;
	}

	constexpr inline void setData(_Char_* newData) noexcept {
		_dataStart = newData;
	}

	constexpr inline void setSize(size_t newSize) noexcept {
		_dataLength = newSize;
	}

	constexpr inline void setNarrowingConversion(bool isNarrowingConversion) noexcept {
		_isNarrowingConversion = isNarrowingConversion;
	}

	bool operator==(const StringConversionResult& other) noexcept {
		return true;// return base_any_strcmp()
	}
private:
	_Char_* _dataStart = nullptr;
	size_t _dataLength = 0;
	// true if data loss occurred during conversion from a more 
	// spacious type to a less spacious one (narrowing conversion)
	bool _isNarrowingConversion = false;
};

__BASE_STRING_NAMESPACE_END
