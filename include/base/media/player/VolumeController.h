#pragma once

#include <QPushButton>

#include <base/core/BaseNamespace.h>
#include <base/core/arch/KeywordSupport.h>

class EnhancedSlider;

__BASE_QT_UI_NAMESPACE_BEGIN

class VolumeController : public QPushButton
{
	Q_OBJECT
public:
	VolumeController(QWidget* parent = nullptr);

	base_nodiscard bool isSpeakerOn() const noexcept;

	void setSpeakerEnabled(bool enabled);
	void setVolume(int volume);
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	void paintSpeakerOff(QPainter& painter);
	void paintSpeakerOn(QPainter& painter);

	EnhancedSlider* _volumeSlider = nullptr;
	QImage _speakerOn, _speakerSmallOn, _speakerOff;

	bool _isSpeakerOn, _isVolumeValueSmall;
	int _previousVolume;
};

__BASE_QT_UI_NAMESPACE_END
