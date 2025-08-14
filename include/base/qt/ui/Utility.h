#pragma once

#include <QPointer>
#include <QWidget>

#include <vector>

#include <base/core/Types.h>
#include <base/core/type_traits/TypeTraits.h>



namespace base::qt::ui {
	//  std::vector<QPointer<QWidget>>
	//	GetChildWidgets(not_null<QWidget*> widget)
	//{
	//	const auto& children = widget->children();
	//	auto result = std::vector<QPointer<QWidget>>();

	//	result.reserve(children.size());

	//	for (const auto child : children)
	//		if (child && child->isWidgetType())
	//			result.push_back(static_cast<QWidget*>(child));

	//	return result;
	//}

	//void ToggleChildrenVisibility(
	//	not_null<QWidget*> widget,
	//	bool visible)
	//{
	//	for (const auto& child : GetChildWidgets(widget))
	//		if (child)
	//			child->setVisible(visible);
	//}

} // namespace base::qt::ui