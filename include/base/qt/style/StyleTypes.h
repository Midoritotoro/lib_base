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

static constexpr cursor cursorDefault = Qt::ArrowCursor;
static constexpr cursor cursorPointer = Qt::PointingHandCursor;
static constexpr cursor cursorText = Qt::IBeamCursor;
static constexpr cursor cursorCross = Qt::CrossCursor;

static constexpr align alignTopLeft = (Qt::AlignTop | Qt::AlignLeft);
static constexpr align alignTop = (Qt::AlignTop | Qt::AlignHCenter);
static constexpr align alignTopRight = (Qt::AlignTop | Qt::AlignRight);
static constexpr align alignRight = (Qt::AlignVCenter | Qt::AlignRight);
static constexpr align alignBottomRight = (Qt::AlignBottom | Qt::AlignRight);
static constexpr align alignBottom = (Qt::AlignBottom | Qt::AlignHCenter);
static constexpr align alignBottomLeft = (Qt::AlignBottom | Qt::AlignLeft);
static constexpr align alignLeft = (Qt::AlignVCenter | Qt::AlignLeft);
static constexpr align alignCenter = (Qt::AlignVCenter | Qt::AlignHCenter);

__BASE_QT_STYLE_NAMESPACE_END
