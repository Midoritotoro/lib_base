#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

#define DEFAULT_BUF_SIZE 1024

CONSTEXPR_CXX20 String::String()
{}

CONSTEXPR_CXX20 String::~String() {

}

CONSTEXPR_CXX20 String::String(const String& string) :
	_data(string._data)
{}

CONSTEXPR_CXX20 String::String(
	const Char* chs,
	size_type length)
{
	for (size_type i = 0; i < length; ++i)
		_data.push_back(chs[i]);
}

CONSTEXPR_CXX20 String::String(
	Iterator first,
	Iterator last)
{
	for (auto i = first; i < last; ++i)
		_data.push_back(*i);
}

Char& String::operator[](const sizetype index) noexcept {
	return _data[index];
}

const Char& String::operator[](const sizetype index) const noexcept {
	return _data[index];
}

Char String::at(const sizetype index) const noexcept {
	return _data.at(index);
}

NativeString String::toNativeString() const noexcept {
#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	return toStdWString();
#else 
	return toStdString();
#endif
}

std::wstring String::toStdWString() const noexcept {
	/*std::wstring result;

	const auto iterations = size() / DEFAULT_BUF_SIZE;
	for (sizetype i = 0; i < iterations; ++i) {
		ConvertUnicodeToWChar(result.data(), 
			_data.data() + iterations * DEFAULT_BUF_SIZE, 
			DEFAULT_BUF_SIZE)
	}*/
	return {};
}

std::string String::toStdString() const noexcept {
	return {};
}

sizetype String::size() const noexcept {
	return _data.size();
}

sizetype String::length() const noexcept {
	return size();
}

sizetype String::capacity() const noexcept {
	return _data.capacity();
}

Char String::pop() noexcept {
	const auto _Size = size();

	if (_Size <= 0)
		return Char();

	const auto character = at(_Size - 1);
	remove(_Size - 1, 1);

	return character;
}

sizetype String::find(const String& string) const noexcept {
	const auto searchStringSize = string.size();
	const auto _Size			= size();

	if (searchStringSize == 1)
		return find(string[0]);

	if (_Size < searchStringSize)
		return -1;

	for (sizetype i = 0; i < _Size; i += searchStringSize) {
		
	}

	return {};
}

sizetype String::find(Char ch) const noexcept {
	const auto _Size = size();

	if (_Size <= 0)
		return -1;

	return {};
}

std::vector<sizetype> String::findAll(const String& string) const noexcept {
	return {};
}

std::vector<sizetype> String::findAll(Char ch) const noexcept {
	return {};
}

sizetype String::findLastOf(const String& string) const noexcept {
	return {};
}

sizetype String::findLastOf(Char ch) const noexcept {
	return {};
}

sizetype String::findFirstOf(const String& string) const noexcept {
	return {};
}

sizetype String::findFirstOf(Char ch) const noexcept {
	return {};
}

sizetype String::findLastNotOf(const String& string) const noexcept {
	return {};
}

sizetype String::findLastNotOf(Char ch) const noexcept {
	return {};
}

sizetype String::findFirstNotOf(const String& string) const noexcept {
	return {};
}

sizetype String::findFirstNotOf(Char ch) const noexcept {
	return {};
}

void String::clear() {
	_data.clear();
}

bool String::isEmpty() const noexcept {
	return (_data.size() == 0);
}

bool String::isNull() const noexcept {
	return isEmpty();
}

Char* String::data() const noexcept {
	return nullptr;
}

const Char* String::constData() const noexcept {
	return nullptr;
}

CONSTEXPR_CXX20 String::Iterator String::begin() noexcept {
	return Iterator(this);
}

CONSTEXPR_CXX20 String::ConstIterator String::begin() const noexcept {
	return ConstIterator(this);
}

CONSTEXPR_CXX20 String::ConstIterator String::cbegin() const noexcept {
	return ConstIterator(this);
}

CONSTEXPR_CXX20 String::ConstIterator String::constBegin() const noexcept {
	return ConstIterator(this);
}

CONSTEXPR_CXX20 String::Iterator String::end() noexcept  {
	return (Iterator(this) + size());
}

CONSTEXPR_CXX20 String::ConstIterator String::end() const noexcept {
	return (ConstIterator(this) + size());
}

CONSTEXPR_CXX20 String::ConstIterator String::cend() const {
	return (ConstIterator(this) + size());
}

CONSTEXPR_CXX20 String::ConstIterator String::constEnd() const noexcept {
	return (ConstIterator(this) + size());
}

CONSTEXPR_CXX20 String::ReverseIterator String::rbegin() noexcept {
	return ReverseIterator(end());
}

CONSTEXPR_CXX20 String::ReverseIterator String::rend() noexcept {
	return ReverseIterator(begin());
}

CONSTEXPR_CXX20 String::ConstReverseIterator String::rbegin() const noexcept {
	return ConstReverseIterator(end());
}

CONSTEXPR_CXX20 String::ConstReverseIterator String::rend() const noexcept {
	return ConstReverseIterator(begin());
}

CONSTEXPR_CXX20 String::ConstReverseIterator String::crbegin() const noexcept {
	return ConstReverseIterator(end());
}

CONSTEXPR_CXX20 String::ConstReverseIterator String::crend() const noexcept {
	return ConstReverseIterator(begin());
}

void String::push_back(Char c) {
	append(c); 
}

void String::push_back(const String& s) { 
	append(s);
}

void String::push_front(Char c) {
	prepend(c);
}

void String::push_front(const String& s) {
	prepend(s); 
}

String::Iterator String::erase(
	ConstIterator first, 
	ConstIterator last)
{
	return Iterator(this);
}

String::Iterator String::erase(ConstIterator it) { 
	return erase(it, it + 1); 
}

Char String::front() const { 
	return at(0); 
}

Char& String::front() {
	return _data.at(0);
}

Char String::back() const {
	return at(size() - 1); 
}

Char& String::back() {
	return _data.at(size() - 1);
}

void String::resize(sizetype size) {
	_data.resize(size);
}

void String::resize(
	sizetype _size,
	Char _fill)
{
	_data.resize(_size);
	fill(_fill, size());
}

sizetype String::indexOf(
	Char ch,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{
	const auto _Size = size();

	if (from < 0 || from > _Size)
		return -1;

	for (sizetype i = from; i < _Size; ++i)
		if (_data[i] == ch)
			return i;

	return -1;
}

sizetype String::indexOf(
	const String& string,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{
	const auto _Size			= size();
	const auto searchStringSize = string.size();

	if (searchStringSize == 1)
		return indexOf(string[0]);

	if (_Size < searchStringSize)
		return -1;

	for (sizetype i = from; i < _Size; ++i) {
		// ...
	}
}

sizetype String::lastIndexOf(
	Char ch,
	CaseSensitivity caseSensitivity) const noexcept
{
	return lastIndexOf(
		ch, size(), caseSensitivity);
}

sizetype String::lastIndexOf(
	Char ch,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{
	const auto _Size = size();

	if (_Size <= 0 || _Size < from || from < 0)
		return -1;

	// for (sizetype i = from; i 
}

sizetype String::lastIndexOf(
	const String& string,
	CaseSensitivity caseSensitivity) const
{
	return lastIndexOf(
		string, size(),
		caseSensitivity);
}

sizetype String::lastIndexOf(
	const String& string,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{
	return {};
}

bool String::contains(
	Char ch, 
	CaseSensitivity caseSensitivity) const 
{
	return {};
}

bool String::contains(
	const String& string,
	CaseSensitivity caseSensitivity) const 
{
	return {};
}

sizetype String::count(
	Char ch,
	CaseSensitivity caseSensitivity) const
{
	return {};
}

sizetype String::count(
	const String& string,
	CaseSensitivity caseSensitivity) const
{
	return {};
}

String& String::fill(
	Char ch,
	sizetype size)
{
	return *this;
}

String& String::insert(
	sizetype index,
	Char ch)
{
	return *this;
}

String& String::insert(
	sizetype index,
	const Char* ch,
	sizetype length)
{
	return *this;
}

String& String::insert(
	sizetype index,
	const String& string) 
{
	return insert(
		index, string.constData(),
		string.size());
}

String& String::append(Char ch) {
	_data.push_back(ch);
	return *this;
}

String& String::append(
	const Char* ch,
	sizetype length)
{
	_data.push_back(*ch);
	return *this;
}

String& String::append(const String& string) {
	for (sizetype i = 0; i < string.size(); ++i)
		_data.push_back(string[i]);

	return *this;
}

String& String::prepend(Char ch) {
	return insert(0, ch); 
}

String& String::prepend(
	const Char* ch,
	sizetype length) 
{
	return insert(0, ch, length);
}

String& String::prepend(const String& string) {
	return insert(0, string); 
}

String& String::remove(
	sizetype index,
	sizetype length)
{
	return *this;
}

String& String::remove(
	Char ch,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String& String::remove(
	const String& string,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String& String::removeAt(sizetype pos) {
	return size_t(pos) < size_t(size()) 
		? remove(pos, 1) 
		: *this;
}

String& String::removeFirst() {
	return !isEmpty() 
		? remove(0, 1) 
		: *this;
}

String& String::removeLast() { 
	return !isEmpty() 
		? remove(size() - 1, 1) 
		: *this; 
}

String& String::replace(
	sizetype index,
	sizetype length,
	Char after)
{
	return *this;
}

String& String::replace(
	sizetype index,
	sizetype length,
	const Char* ch,
	sizetype slen)
{
	return *this;
}

String& String::replace(
	sizetype index,
	sizetype length,
	const String& after)
{
	return *this;
}

String& String::replace(
	Char before,
	Char after,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String& String::replace(
	const Char* before,
	sizetype beforeLength,
	const Char* after,
	sizetype afterLength,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String& String::replace(
	const String& before,
	const String& after,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String& String::replace(
	Char ch,
	const String& after,
	CaseSensitivity caseSensitivity)
{
	return *this;
}

String::StringList String::split(
	const String& sep,
	SplitBehavior behavior,
	CaseSensitivity caseSensibity) const
{
	return {};
}

String::StringList String::split(
	Char sep,
	SplitBehavior behavior,
	CaseSensitivity caseSensibity) const
{
	return {};
}

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	int String::ConvertWCharToUnicode(
        char* buffer,
        size_t bufferlen,
        const wchar_t* input)
    {
        return WideCharToMultiByte(
            CP_UTF8, 0, input, -1,
            buffer, (int)bufferlen,
            nullptr, nullptr);
    }

    int String::ConvertUnicodeToWChar(
        wchar_t* buffer,
        size_t bufferlen,
        const char* input)
    {
		return MultiByteToWideChar(
			CP_UTF8 , 0, input, -1,
            buffer, (int)bufferlen);
    }
#endif

__BASE_STRING_NAMESPACE_END