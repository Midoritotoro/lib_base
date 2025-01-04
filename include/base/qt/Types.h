#pragma once 

#include <QString>
#include <cstdint>


#if __has_include(<gsl/gsl>)
	#include <gsl/gsl>
	using gsl::not_null;
#endif() 

template <typename Signature>
using Fn = std::function<Signature>;

using int8 = qint8;
using uint8 = quint8;

using int16 = qint16;
using uint16 = quint16;

using int32 = qint32;
using uint32 = quint32;

using int64 = qint64;
using uint64 = quint64;

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
