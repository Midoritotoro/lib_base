#pragma once 

#include <QCursor>
#include <QIcon>

#include <base/core/Types.h>

__BASE_QT_STYLE_NAMESPACE_BEGIN

namespace internal {
	class Font;
	class OwnedFont;
}

using rect = QRect;
using color = QColor;
using align = Qt::Alignment;
using cursor = Qt::CursorShape;
using margins = QMargins;
using font = internal::Font;
using owned_font = internal::OwnedFont;
using icon = QIcon;

inline static constexpr cursor cursorDefault = Qt::ArrowCursor;
inline static constexpr cursor cursorPointer = Qt::PointingHandCursor;
inline static constexpr cursor cursorText = Qt::IBeamCursor;
inline static constexpr cursor cursorCross = Qt::CrossCursor;

inline static constexpr align alignTopLeft = (Qt::AlignTop | Qt::AlignLeft);
inline static constexpr align alignTop = (Qt::AlignTop | Qt::AlignHCenter);
inline static constexpr align alignTopRight = (Qt::AlignTop | Qt::AlignRight);
inline static constexpr align alignRight = (Qt::AlignVCenter | Qt::AlignRight);
inline static constexpr align alignBottomRight = (Qt::AlignBottom | Qt::AlignRight);
inline static constexpr align alignBottom = (Qt::AlignBottom | Qt::AlignHCenter);
inline static constexpr align alignBottomLeft = (Qt::AlignBottom | Qt::AlignLeft);
inline static constexpr align alignLeft = (Qt::AlignVCenter | Qt::AlignLeft);
inline static constexpr align alignCenter = (Qt::AlignVCenter | Qt::AlignHCenter);

__BASE_QT_STYLE_NAMESPACE_END
