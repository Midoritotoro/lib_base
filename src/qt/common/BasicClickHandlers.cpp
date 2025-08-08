#include <base/qt/common/BasicClickHandlers.h>

#include <QUrl>
#include <QRegularExpression>

#include <QDesktopServices>
#include <QGuiApplication>

#include <QString>

#include <text/TextEntities.h>
#include <base/core/utility/Algorithm.h>


__BASE_QT_COMMON_NAMESPACE_BEGIN

namespace {
	namespace details {
		enum CutResult {
			Null,
			Empty,
			Full,
			Subset
		};

		static constexpr base_nodiscard CutResult mid(
			qsizetype originalLength,
			qsizetype* _position,
			qsizetype* _length)
		{
			qsizetype& position = *_position;
			qsizetype& length = *_length;

			if (position > originalLength) {
				position = 0;
				length = 0;
				return Null;
			}

			if (position < 0) {
				if (length < 0 || length + position >= originalLength) {
					position = 0;
					length = originalLength;
					return Full;
				}
				if (length + position <= 0) {
					position = length = 0;
					return Null;
				}
				length += position;
				position = 0;
			}
			else if (size_t(length) > size_t(originalLength - position)) {
				length = originalLength - position;
			}

			if (position == 0 && length == originalLength)
				return Full;

			return length > 0 ? Subset : Empty;
		}
	} // namespace details

	base_nodiscard inline QStringView StringViewMid(
		QStringView view,
		qsizetype pos,
		qsizetype n = -1) {
		const auto result = details::mid(
			view.size(),
			&pos,
			&n);
		return (result == details::Null)
			? QStringView()
			: view.mid(pos, n);
	}
} // namespace 



QString TextClickHandler::readable() const {
	const auto result = url();
	return !result.startsWith(QLatin1String("internal:"))
		? result
		: result.startsWith(QLatin1String("internal:url:"))
		? result.mid(QLatin1String("internal:url:").size())
		: QString();
}

UrlClickHandler::UrlClickHandler(const QString& url, bool fullDisplayed)
	: TextClickHandler(fullDisplayed)
	, _originalUrl(url) {
	if (isEmail()) {
		_readable = _originalUrl;
	}
	else if (!_originalUrl.startsWith(QLatin1String("internal:"))) {
		const auto original = QUrl(_originalUrl);
		const auto good = QUrl(original.isValid()
			? original.toEncoded()
			: QString());
		_readable = good.isValid() ? good.toDisplayString() : _originalUrl;
	}
	else if (_originalUrl.startsWith(QLatin1String("internal:url:"))) {
		const auto external = _originalUrl.mid(QLatin1String("internal:url:").size());
		const auto original = QUrl(external);
		const auto good = QUrl(original.isValid()
			? original.toEncoded()
			: QString());
		_readable = good.isValid() ? good.toDisplayString() : external;
	}
}

QString UrlClickHandler::copyToClipboardContextItemText() const {
	return isEmail()
		? "Copy Email"
		: "Copy Link";
}

QString UrlClickHandler::EncodeForOpening(const QString& originalUrl) {
	if (IsEmail(originalUrl))
		return originalUrl;

	static const auto TonExp = QRegularExpression(u"^[^/@:]+\\.ton($|/)"_q);
	if (TonExp.match(originalUrl.toLower()).hasMatch())
		return u"tonsite://"_q + originalUrl;


	const auto u = QUrl(originalUrl);
	const auto good = QUrl(u.isValid() ? u.toEncoded() : QString());
	const auto result = good.isValid()
		? QString::fromUtf8(good.toEncoded())
		: originalUrl;

	static const auto RegExp = QRegularExpression(u"^[a-zA-Z]+:"_q);

	if (!result.isEmpty()
		&& !RegExp.match(result).hasMatch()) {
		// No protocol.
		return u"https://"_q + result;
	}
	return result;
}

void UrlClickHandler::Open(QString url, QVariant context) {
	if (url.isEmpty())
		return;

	if (IsEmail(url))
		url = "mailto: " + url;

	QDesktopServices::openUrl(url);
}

bool UrlClickHandler::IsSuspicious(const QString& url) {
	static const auto Check1 = QRegularExpression(
		"^((https?|s?ftp)://)?([^/#\\:]+)([/#\\:]|$)",
		QRegularExpression::CaseInsensitiveOption);

	const auto match1 = Check1.match(url);
	if (!match1.hasMatch())
		return false;

	const auto domain = match1.capturedView(3);
	static const auto Check2 = QRegularExpression("^(.*)\\.[a-zA-Z]+$");

	const auto match2 = Check2.match(domain);
	if (!match2.hasMatch())
		return false;

	const auto part = match2.capturedView(1);

	static const auto Check3 = QRegularExpression("[^a-zA-Z0-9\\.\\-]");
	return Check3.match(part).hasMatch();
}


QString UrlClickHandler::ShowEncoded(const QString& url) {
	if (const auto _url = QUrl(url); _url.isValid())
		return QString::fromUtf8(_url.toEncoded());

	static const auto Check1 = QRegularExpression(
		"^(https?://)?([^/#\\:]+)([/#\\:]|$)",
		QRegularExpression::CaseInsensitiveOption);

	if (const auto match1 = Check1.match(url); match1.hasMatch()) {
		const auto domain = match1.captured(1).append(match1.capturedView(2));
		if (const auto u = QUrl(domain); u.isValid()) {
			return QString(
			).append(QString::fromUtf8(u.toEncoded())
			).append(StringViewMid(url, match1.capturedEnd(2)));
		}
	}
	return url;
}

auto UrlClickHandler::getTextEntity() const -> TextEntity {
	const auto type = isEmail() ? text::EntityType::Email : text::EntityType::Url;
	return { type, _originalUrl };
}

QString HiddenUrlClickHandler::copyToClipboardText() const {
	return url().startsWith(u"internal:url:"_q)
		? url().mid(u"internal:url:"_q.size())
		: url();
}

QString HiddenUrlClickHandler::copyToClipboardContextItemText() const {
	return url().isEmpty()
		? QString()
		: !url().startsWith(u"internal:"_q)
		? UrlClickHandler::copyToClipboardContextItemText()
		: url().startsWith(u"internal:url:"_q)
		? UrlClickHandler::copyToClipboardContextItemText()
		: QString();
}

QString HiddenUrlClickHandler::dragText() const {
	const auto result = HiddenUrlClickHandler::copyToClipboardText();
	return result.startsWith(u"internal:"_q) ? QString() : result;
}

void HiddenUrlClickHandler::Open(QString url, QVariant context) {
	UrlClickHandler::Open(url, context);
}

auto HiddenUrlClickHandler::getTextEntity() const -> TextEntity {
	return { text::EntityType::CustomUrl, url() };
}

__BASE_QT_COMMON_NAMESPACE_END
