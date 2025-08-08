#pragma once

#include <base/qt/common/ClickHandler.h>
#include <base/core/BaseNamespace.h>

#include <QString>


__BASE_QT_COMMON_NAMESPACE_BEGIN

class TextClickHandler : public ClickHandler {
public:
	TextClickHandler(bool fullDisplayed = true)
		: _fullDisplayed(fullDisplayed) {
	}

	QString copyToClipboardText() const override {
		return url();
	}

	QString tooltip() const override {
		return _fullDisplayed ? QString() : readable();
	}

	void setFullDisplayed(bool full) {
		_fullDisplayed = full;
	}

protected:
	virtual QString readable() const;

	bool _fullDisplayed;

};

class UrlClickHandler : public TextClickHandler {
public:
	UrlClickHandler(const QString& url, bool fullDisplayed = true);

	base_nodiscard QString originalUrl() const {
		return _originalUrl;
	}
		 

	QString copyToClipboardContextItemText() const override;

	QString dragText() const override {
		return url();
	}

	TextEntity getTextEntity() const override;

	static void Open(QString url, QVariant context = {});
	void onClick(ClickContext context) const override {
		const auto button = context.button;
		if (button == Qt::LeftButton || button == Qt::MiddleButton) {
			Open(url(), context.other);
		}
	}

	base_nodiscard static bool IsEmail(const QString& url) {
		const auto at = url.indexOf('@'), slash = url.indexOf('/');
		return ((at > 0) && (slash < 0 || slash > at));
	}
	base_nodiscard static QString EncodeForOpening(const QString& originalUrl);
	base_nodiscard static bool IsSuspicious(const QString& url);
	base_nodiscard static QString ShowEncoded(const QString& url);

protected:
	QString url() const override {
		return EncodeForOpening(_originalUrl);
	}
	QString readable() const override {
		return _readable;
	}

private:
	base_nodiscard bool isEmail() const {
		return IsEmail(_originalUrl);
	}

	QString _originalUrl, _readable;

};

class HiddenUrlClickHandler : public UrlClickHandler {
public:
	HiddenUrlClickHandler(QString url) : UrlClickHandler(url, false) {
	}
	QString copyToClipboardText() const override;
	QString copyToClipboardContextItemText() const override;
	QString dragText() const override;

	static void Open(QString url, QVariant context = {});
	void onClick(ClickContext context) const override {
		const auto button = context.button;
		if (button == Qt::LeftButton || button == Qt::MiddleButton) {
			Open(url(), context.other);
		}
	}

	TextEntity getTextEntity() const override;
};

__BASE_QT_COMMON_NAMESPACE_END