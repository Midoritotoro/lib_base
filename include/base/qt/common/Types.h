#pragma once 

#include <QString>

#include <base/core/BaseNamespace.h>
#include <base/core/arch/KeywordSupport.h>

__BASE_QT_NAMESPACE_BEGIN

NODISCARD inline QByteArray operator""_q(
	const char* data,
	std::size_t size)
{
	return QByteArray::fromRawData(data, size);
}

NODISCARD inline QString operator""_q(
	const char16_t* data,
	std::size_t size) 
{
	return QString::fromRawData(
		reinterpret_cast<const QChar*>(data),
		size);
}

__BASE_QT_NAMESPACE_END

