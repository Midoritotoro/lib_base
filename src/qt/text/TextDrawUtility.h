#pragma once 

#include <src/qt/text/Types.h>


__BASE_QT_TEXT_NAMESPACE_BEGIN

[[nodiscard]] GeometryDescriptor SimpleGeometry(
	int availableWidth,
	int elisionLines,
	int elisionRemoveFromEnd,
	bool elisionBreakEverywhere);

void ValidateQuotePaintCache(
	QuotePaintCache& cache,
	const style::QuoteStyle& st);
void FillQuotePaint(
	QPainter& p,
	QRect rect,
	const QuotePaintCache& cache,
	const style::QuoteStyle& st,
	SkipBlockPaintParts parts);

__BASE_QT_TEXT_NAMESPACE_END