#pragma once

#include <QPushButton>

class EnhancedSlider;

namespace base::qt::ui {
	class VolumeController : public QPushButton
	{
		Q_OBJECT
	public:
		VolumeController(QWidget* parent = nullptr);

		[[nodiscard]] bool isSpeakerOn() const noexcept;

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
} // namespace base::qt::ui