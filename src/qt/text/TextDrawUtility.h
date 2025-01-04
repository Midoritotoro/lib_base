#pragma once 

#include "Types.h"


namespace text {
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
} // namespace text