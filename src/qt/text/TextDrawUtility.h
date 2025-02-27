#pragma once 

#include <src/qt/text/Types.h>


namespace base::qt::text {
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
} // namespace base::qt::text