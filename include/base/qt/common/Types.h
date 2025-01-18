#pragma once 

#include <QString>

[[nodiscard]] inline QByteArray operator""_q(
	const char* data,
	std::size_t size)
{
	return QByteArray::fromRawData(data, size);
}

[[nodiscard]] inline QString operator""_q(
	const char16_t* data,
	std::size_t size) 
{
	return QString::fromRawData(
		reinterpret_cast<const QChar*>(data),
		size);
}
