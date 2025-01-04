#pragma once 

#include <base/qt/StyleFont.h>


namespace style {
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

	[[nodiscard]] QString SliderStyle();
	[[nodiscard]] QString ScrollAreaStyle();

	struct FlatLabel {
		margins margin = { 0, 0, 0, 0 };
		color colorBg;

		const TextStyle* textStyle = nullptr;
		const TextPalette* textPalette = nullptr;

		int borderRadius = 0;

		int maximumWidth = 0;
		int maximumHeight = 0;

		int minimumWidth = 0;
		int minimumHeight = 0;
	};

	struct FlatButton {
		margins margin = { 0, 0, 0, 0 };
		color colorBg;

		const TextStyle* textStyle = nullptr;
		const TextPalette* textPalette = nullptr;

		int borderRadius = 0;
	};

	struct Separator {
		int width = 0;

		margins padding = { 0, 0, 0, 0 };
		color colorFg;
	};

	struct Slider {

	};

	struct PopupMenu {
		margins margin = { 0, 0, 0, 0 };
		color colorBg;

		const TextStyle* textStyle = nullptr;
		const TextPalette* textPalette = nullptr;

		int borderRadius = 0;

		int maximumWidth = 0;
		int maximumHeight = 0;

		int minimumWidth = 0;
		int minimumHeight = 0;
	};


	inline const FlatLabel* defaultFlatLabelStyle = nullptr;
	inline const PopupMenu* defaultPopupMenuStyle = nullptr;

	inline const FlatButton* defaultFlatButtonStyle = nullptr;
} // namespace style