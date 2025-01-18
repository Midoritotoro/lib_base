#pragma once 

#include <QPainter>


namespace base::qt::ui {
	class PainterHighQualityEnabler {
	public:
		PainterHighQualityEnabler(QPainter& p) : _painter(p) {
			static constexpr QPainter::RenderHint Hints[] = {
				QPainter::Antialiasing,
				QPainter::SmoothPixmapTransform,
				QPainter::TextAntialiasing
			};

			const auto hints = _painter.renderHints();
			for (const auto hint : Hints) {
				if (!(hints & hint)) {
					_hints |= hint;
				}
			}
			if (_hints) {
				_painter.setRenderHints(_hints);
			}
		}

		PainterHighQualityEnabler(
			const PainterHighQualityEnabler& other) = delete;
		PainterHighQualityEnabler& operator=(
			const PainterHighQualityEnabler& other) = delete;

		~PainterHighQualityEnabler() {
			if (_hints && _painter.isActive()) {
				_painter.setRenderHints(_hints, false);
			}
		}

	private:
		QPainter& _painter;
		QPainter::RenderHints _hints;

	};
} // namespace base::qt::ui
