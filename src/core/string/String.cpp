#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

//
//#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
//    inline [[nodiscard]] int ConvertWCharToUnicode(
//        char* buffer,
//        size_t bufferlen,
//        const wchar_t* input)
//    {
//        return WideCharToMultiByte(
//            CP_UTF8, 0, input, -1,
//            buffer, (int)bufferlen,
//            nullptr, nullptr);
//    }
//
//    inline [[nodiscard]] int ConvertUnicodeToWChar(
//        wchar_t* buffer,
//        size_t bufferlen,
//        const char* input)
//    {
//        return MultiByteToWideChar(
//            CP_UTF8, 0, input, -1,
//            buffer, (int)bufferlen);
//    }
//#endif

String::String() {

}

String::~String() {

}

String::String(const String& string) {

}

String::String(const Char* chs) {

}

Char& String::operator[](const sizetype index) noexcept {

}

Char String::at(const sizetype index) const noexcept {

}

NativeString String::toNativeString() const noexcept {

}

std::wstring String::toStdWString() const noexcept {

}

std::string String::toStdString() const noexcept {

}

sizetype String::size() const noexcept {

}

sizetype String::length() const noexcept {

}

void String::append(Char ch) noexcept {

}

void String::insert(
	Char ch,
	sizetype index) noexcept 
{

}

Char String::pop() noexcept {

}

sizetype String::find(const Char* chs) const noexcept {

}

sizetype String::findLastOf(const Char* chs) const noexcept {

}

sizetype String::findLastOf(const Char* chs) const noexcept {

}

sizetype String::findFirstOf(const Char* chs) const noexcept {

}

sizetype String::findLastNotOf(const Char* chs) const noexcept {

}

sizetype String::findFirstNotOf(const Char* chs) const noexcept {

}

void String::clear() {

}

bool String::isEmpty() const noexcept {

}

bool String::isNull() const noexcept {

}

Char* String::data() const noexcept {

}

const Char* String::constData() const noexcept {

}

String::iterator String::begin() noexcept {

}

String::const_iterator String::begin() const noexcept {

}

String::const_iterator String::cbegin() const noexcept {

}

String::const_iterator String::constBegin() const noexcept {

}

String::iterator String::end() noexcept  {

}

String::const_iterator String::end() const noexcept {

}

String::const_iterator String::cend() const {

}

String::const_iterator String::constEnd() const noexcept {

}

String::reverse_iterator String::rbegin() noexcept {
	return reverse_iterator(end()); 
}

String::reverse_iterator String::rend() noexcept {
	return reverse_iterator(begin());
}

String::const_reverse_iterator String::rbegin() const noexcept {
	return const_reverse_iterator(end()); 
}

String::const_reverse_iterator String::rend() const noexcept {
	return const_reverse_iterator(begin()); 
}

String::const_reverse_iterator String::crbegin() const noexcept {
	return const_reverse_iterator(end());
}

String::const_reverse_iterator String::crend() const noexcept {
	return const_reverse_iterator(begin());
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

}

String::Iterator String::erase(ConstIterator it) { 
	return erase(it, it + 1); 
}


Char String::front() const { 
	return at(0); 
}

Char& String::front() {
	return Char();
}

Char String::back() const { 
	return at(size() - 1); 
}

Char& String::back() {

}

void String::resize(sizetype size) {

}

void String::resize(
	sizetype size,
	Char fill)
{

}

sizetype String::indexOf(
	Char ch,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{

}

sizetype String::indexOf(
	const String& string,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{
}


sizetype String::lastIndexOf(
	Char ch,
	CaseSensitivity caseSensitivity) const noexcept
{
	return lastIndexOf(ch, -1, caseSensitivity);
}

sizetype String::lastIndexOf(
	Char ch,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{

}

sizetype String::lastIndexOf(
	const String& string,
	CaseSensitivity caseSensitivity) const
{
	return lastIndexOf(s, size(), cs);
}

sizetype String::lastIndexOf(
	const String& string,
	sizetype from,
	CaseSensitivity caseSensitivity) const
{

}

bool String::contains(Char ch, CaseSensitivity caseSensitivity) const {

}

bool String::contains(
	const String& string,
	CaseSensitivity caseSensitivity) const 
{

}

sizetype String::count(
	Char c,
	CaseSensitivity caseSensitivity) const
{

}

sizetype String::count(
	const String& string,
	CaseSensitivity caseSensitivity) const
{

}

String& String::fill(
	Char c,
	sizetype size)
{

}


__BASE_STRING_NAMESPACE_END