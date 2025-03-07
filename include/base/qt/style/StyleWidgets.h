#pragma once 

#include <base/qt/style/StyleFont.h>


__BASE_QT_STYLE_NAMESPACE_BEGIN

inline constexpr QMargins mediaPlayerPanelMargins = {
	10, 5, 10, 5
};

inline constexpr auto mediaPlayerPanelBorderRadius = 10;

inline constexpr auto maximumMessageWidth = 430;
inline constexpr auto minimumMessageWidth = 100;

inline constexpr auto minimumMessageHeight = 100;

inline constexpr auto maximumTextHeight = 1080 * 5;
inline constexpr auto maximumTextWidth = 430;

inline constexpr auto sliderHeight = 20;

inline constexpr QMargins messageTextViewMargins = { 8, 5, 8, 8 };
inline constexpr auto messageTextViewBorderRadius = 10;

inline constexpr auto panelWidth = 460;
inline constexpr auto lineWidth = 1;


void initStyles();

NODISCARD QString SliderStyle();
NODISCARD QString ScrollAreaStyle();

struct StyleSize {
	int maximumWidth = 0;
	int maximumHeight = 0;

	int minimumWidth = 0;
	int minimumHeight = 0;
};

struct StyleBase {
	int borderRadius = 0;
	int borderWidth = 0;

	const TextStyle* textStyle = nullptr;
	const TextPalette* textPalette = nullptr;

	margins margin = { 0, 0, 0, 0 };
	color colorBg;

	int prefferedWidth = 0;
	int prefferedHeight = 0;

	std::optional<StyleSize> _size;
};


struct FlatLabel: public StyleBase {
};

struct FlatButton: public StyleBase {
};

struct Separator {
	int width = 0;

	margins padding = { 0, 0, 0, 0 };
	color colorFg;
};

struct Slider: public StyleBase {
};

struct PopupMenu: public StyleBase {
	int actionHeight = 30;
};

using MenuAction = FlatButton;

inline const FlatLabel* defaultFlatLabelStyle = nullptr;
inline const PopupMenu* defaultPopupMenuStyle = nullptr;

inline const FlatButton* defaultFlatButtonStyle = nullptr;
inline const MenuAction* defaultActionStyle = nullptr;

__BASE_QT_STYLE_NAMESPACE_END
