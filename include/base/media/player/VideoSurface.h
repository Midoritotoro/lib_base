#pragma once

#include <base/core/BaseNamespace.h>

#include <qopengl.h>
#include <QWidget>

#include <base/media/ffmpeg/video/Picture.h> 
#include <qopenglwidget.h>

__BASE_MEDIA_NAMESPACE_BEGIN

class VideoSurface: public QOpenGLWidget {
public: 
	//VideoSurface();
	//~VideoSurface();

	//void setFrame(not_null<ffmpeg::video::Frame*> frame);
	//

	//void paintGL() override;

};

__BASE_MEDIA_NAMESPACE_END