#include <base/qt/style/StyleWidgets.h>

#include <QFile>
#include <QString>


namespace base::qt::style {
	namespace {
		QString _sliderStyle = "";
		QString _scrollAreaStyle = "";

		void loadStyles() {
			const auto sliderStyle = ":/src/css/SliderStyle.css";
			const auto scrollAreaStyle = ":/src/css/ScrollAreaStyle.css";

			auto file = QFile(sliderStyle);

			if (file.open(QFile::ReadOnly))
				_sliderStyle = file.readAll();

			file.setFileName(scrollAreaStyle);

			if (file.open(QFile::ReadOnly))
				_scrollAreaStyle = file.readAll();
		}
	} // namespace

	QString SliderStyle() {
		return _sliderStyle;
	}

	QString ScrollAreaStyle() {
		return _scrollAreaStyle;
	}

	void initStyles() {
		loadStyles();

		defaultTextStyle = new TextStyle{
		   ._font = font(13, 0, 0),
		   .lineHeight = 14,

		   .linkUnderLine = true,
		   .blockquote = {} };

		defaultFlatLabelStyle = new FlatLabel{ StyleBase {

			.borderRadius = 10,
			.borderWidth = 1,

			.textStyle = defaultTextStyle,
			.textPalette = &defaultTextPalette,


			.margin = margins{ 10, 10, 10, 10 },
			.colorBg = color(24, 37, 51),

			/*  .maximumWidth = style::maximumMessageWidth,
				.maximumHeight = style::maximumTextHeight,
				*/
			._size = StyleSize{
				.minimumWidth = style::minimumMessageWidth,
				.minimumHeight = style::minimumMessageHeight,
			}
		}};

		defaultPopupMenuStyle = new PopupMenu{
			.margin = { 0, 0, 0, 0 },
			.colorBg = color(24, 37, 51),

			.textStyle = defaultTextStyle,
			.textPalette = &defaultTextPalette,

			.borderRadius = 10,

			.maximumWidth = 150,
			.maximumHeight = 210,

			.minimumWidth = 64,
			.minimumHeight = 30,

			.actionHeight = 30
		};

		defaultFlatButtonStyle = new FlatButton{
			.margin = margins{ 0, 0, 0, 0 },
			.colorBg = color(24, 37, 51),

			.textStyle = defaultTextStyle,
			.textPalette = &defaultTextPalette,

			.borderRadius = 10
		};

		defaultActionStyle = new style::MenuAction{
			.margin = style::margins{ 0, 0, 0, 0 },
			.colorBg = style::color(24, 37, 51),

			.textStyle = style::defaultTextStyle,
			.textPalette = &style::defaultTextPalette,

			.borderRadius = 0
		};

	}
} // namespace base::qt::style