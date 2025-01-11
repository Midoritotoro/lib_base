#pragma once

#include <base/qt/style/StyleWidgets.h>
#include <base/qt/ui/BaseWidget.h>


namespace base::qt::ui {
	class Separator : 
		public CoreWidget<style::Separator>
	{
	public:
		Separator(
			QWidget* parent,
			const SelfStyle* style,
			const style::PopupMenu* menuStyle);
	private:
		const int _lineWidth;

		const style::margins& _padding;

		const style::color& _fg;
		const style::color& _bg;

		const int _height;
	};
} // namespace base::qt::ui