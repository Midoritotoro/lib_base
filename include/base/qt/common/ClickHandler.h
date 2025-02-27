#pragma once

#include <src/qt/text/Types.h>
#include <QVariant>


namespace base::qt::text {
	enum class EntityType : uchar;
}

namespace base::qt::common {
	struct ClickContext {
		Qt::MouseButton button = Qt::LeftButton;
		QVariant other;
	};

	class ClickHandler;
	using ClickHandlerPtr = std::shared_ptr<ClickHandler>;

	class ClickHandlerHost {
	protected:
		virtual void clickHandlerActiveChanged(const ClickHandlerPtr& action, bool active) {
		}
		virtual void clickHandlerPressedChanged(const ClickHandlerPtr& action, bool pressed) {
		}

		virtual ~ClickHandlerHost() = 0;
		friend class ClickHandler;
	};

	class ClickHandler {
	public:
		virtual ~ClickHandler() {
		}

		virtual void onClick(ClickContext context) const = 0;

		// Some sort of `id`, for text links contains urls.
		virtual QString url() const {
			return QString();
		}

		// What text to show in a tooltip when mouse is over that click handler as a link in Text.
		virtual QString tooltip() const {
			return QString();
		}

		// What to drop in the input fields when dragging that click handler as a link from Text.
		virtual QString dragText() const {
			return QString();
		}

		// Copy to clipboard support.
		virtual QString copyToClipboardText() const {
			return QString();
		}
		virtual QString copyToClipboardContextItemText() const {
			return QString();
		}

		// Entities in text support.
		struct TextEntity {
			base::qt::text::EntityType type = base::qt::text::EntityType();
			QString data;
		};
		virtual TextEntity getTextEntity() const;

		// This method should be called on mouse over a click handler.
		// It returns true if the active handler was changed or false otherwise.
		static bool setActive(const ClickHandlerPtr& p, ClickHandlerHost* host = nullptr);

		// This method should be called when mouse leaves the host.
		// It returns true if the active handler was changed or false otherwise.
		static bool clearActive(ClickHandlerHost* host = nullptr);

		// This method should be called on mouse press event.
		static void pressed();

		// This method should be called on mouse release event.
		// The activated click handler (if any) is returned.
		static ClickHandlerPtr unpressed();

		static ClickHandlerPtr getActive();
		static ClickHandlerPtr getPressed();

		static bool showAsActive(const ClickHandlerPtr& p);
		static bool showAsPressed(const ClickHandlerPtr& p);
		static void hostDestroyed(ClickHandlerHost* host);

	private:
		static ClickHandlerHost* _activeHost;
		static ClickHandlerHost* _pressedHost;
	};

	void ActivateClickHandler(
		not_null<QWidget*> guard,
		ClickHandlerPtr handler,
		ClickContext context);

	void ActivateClickHandler(
		not_null<QWidget*> guard,
		ClickHandlerPtr handler,
		Qt::MouseButton button);
} // namespace base::qt::common