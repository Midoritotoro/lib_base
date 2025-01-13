#include <base/qt/ui/effects/BlurBehind.h>
#include <base/qt/ui/effects/OpenGLBlur.h>

#include <base/qt/images/ImagesBlur.h>

#include <QPainter>
#include <QWidget>

#include <QThread>
#include <QDebug>

#include <QPainterPath>


class BlurBehindEffectPrivate
{
public:
    GLBlurFunctions glBlur_;
    qint64 cacheKey_;
    QImage sourceImage_;
    QImage blurredImage_;
    QRegion region_;
    BlurBehindEffect::BlurMethod blurringMethod_;
    Qt::CoordinateSystem coordSystem_;
    QBrush backgroundBrush_;
    double sourceOpacity_;
    double blurOpacity_;
    double downsamplingFactor_;
    int blurRadius_;
    int maxThreadCount_;
    bool sourceUpdated_;

    BlurBehindEffectPrivate()
        : cacheKey_(0)
        , blurringMethod_(BlurBehindEffect::BlurMethod::StackBlur)
        , coordSystem_(Qt::LogicalCoordinates)
        , sourceOpacity_(1.0)
        , blurOpacity_(1.0)
        , downsamplingFactor_(2.0)
        , blurRadius_(2)
        , maxThreadCount_(1)
        , sourceUpdated_(true)
    {
    }

    QImage blurImage(const QImage &_input)
    {
        switch(blurringMethod_)
        {
        case BlurBehindEffect::BlurMethod::BoxBlur:
            return images::boxBlurImage(_input, blurRadius_);
        case BlurBehindEffect::BlurMethod::StackBlur:
            return stackBlurImage(_input, blurRadius_, maxThreadCount_);
        case BlurBehindEffect::BlurMethod::GLBlur:
            return glBlur_.blurImage_DualKawase(_input, 2, std::max(blurRadius_ - 2, 1));
        }
        return _input;
    }

    QPixmap grabSource(QWidget* _widget) const
    {
        if (_widget == nullptr)
            return QPixmap();

        double dpr = 1.;
        if (const auto *paintDevice = _widget)
            dpr = paintDevice->devicePixelRatioF();
        else
            qWarning("QtBlurBehindEffect::grabSource: Painter not active");

        const bool isGlBlur = (blurringMethod_ == BlurBehindEffect::BlurMethod::GLBlur);
        auto image = QPixmap(_widget->size() * dpr);

        const auto fill = isGlBlur
            ? _widget->palette().color(_widget->backgroundRole())
            : Qt::transparent;

        image.setDevicePixelRatio(dpr);
        image.fill(fill);

        _widget->render(&image, {}, {}, QWidget::DrawChildren);

        return image;
    }

    void renderImage(
        QPainter *_painter,
        const QImage &_image, 
        const QBrush& _brush)
    {
        if (region_.isEmpty() || _image.isNull())
            return;

        const auto bounds = region_.boundingRect();

        const auto s = bounds.size();
        const auto r = QRect{{}, s};

        auto transform = QTransform();

        if (coordSystem_ == Qt::LogicalCoordinates)
            transform = _painter->worldTransform();

        _painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        if (coordSystem_ == Qt::LogicalCoordinates)
            _painter->translate(bounds.topLeft());

        if (!_brush.isOpaque() && _brush.style() != Qt::NoBrush)
            _painter->fillRect(r, _brush);

        _painter->setOpacity(blurOpacity_);
        _painter->drawImage(r, _image);

        if (coordSystem_ == Qt::LogicalCoordinates)
            _painter->setWorldTransform(transform);
    }
};

BlurBehindEffect::BlurBehindEffect(QWidget* _parent)
    : QGraphicsEffect(_parent)
    , d(std::make_unique<BlurBehindEffectPrivate>())
{}

BlurBehindEffect::~BlurBehindEffect() = default;

void BlurBehindEffect::setSourceOpacity(double _opacity)
{
    _opacity = std::clamp(_opacity, 0.0, 1.0);

    if (d->sourceOpacity_ == _opacity)
        return;

    d->sourceOpacity_ = std::clamp(_opacity, 0.0, 1.0);

    emit sourceOpacityChanged(_opacity);
    emit repaintRequired();

    update();
}

double BlurBehindEffect::sourceOpacity() const
{
    return d->sourceOpacity_;
}

void BlurBehindEffect::setBlurOpacity(double _opacity)
{
    _opacity = std::clamp(_opacity, 0.0, 1.0);

    if (d->blurOpacity_ == _opacity)
        return;

    d->blurOpacity_ = _opacity;

    emit blurOpacityChanged(_opacity);
    emit repaintRequired();

    update();
}

double BlurBehindEffect::blurOpacity() const
{
    return d->blurOpacity_;
}

void BlurBehindEffect::setBackgroundBrush(const QBrush& _brush)
{
    if (d->backgroundBrush_ == _brush)
        return;

    d->backgroundBrush_ = _brush;

    emit backgroundBrushChanged(_brush);
    emit repaintRequired();

    update();
}

QBrush BlurBehindEffect::backgroundBrush() const
{
    return d->backgroundBrush_;
}

void BlurBehindEffect::setBlurMethod(BlurBehindEffect::BlurMethod _method)
{
    if (d->blurringMethod_ == _method)
        return;

    d->blurringMethod_ = _method;
    emit repaintRequired();

    update();
}

BlurBehindEffect::BlurMethod BlurBehindEffect::blurMethod() const
{
    return d->blurringMethod_;
}

void BlurBehindEffect::setRegion(const QRegion& _sourceRegion)
{
    d->region_ = _sourceRegion;
    updateBoundingRect();
}

const QRegion &BlurBehindEffect::region() const
{
    return d->region_;
}

void BlurBehindEffect::setBlurRadius(int _radius)
{
    if (d->blurRadius_ == _radius)
        return;

    d->blurRadius_ = _radius;

    emit blurRadiusChanged(_radius);
    emit repaintRequired();

    updateBoundingRect();
}

int BlurBehindEffect::blurRadius() const
{
    return d->blurRadius_;
}

void BlurBehindEffect::setDownsampleFactor(double _factor)
{
    _factor = std::max(_factor, 1.0);

    if (d->downsamplingFactor_ == _factor)
        return;

    d->downsamplingFactor_ = _factor;

    emit downsampleFactorChanged(_factor);
    emit repaintRequired();

    update();
}

double BlurBehindEffect::downsampleFactor() const
{
    return d->downsamplingFactor_;
}

void BlurBehindEffect::setMaxThreadCount(int _nthreads)
{
    d->maxThreadCount_ = std::clamp(_nthreads, 1, std::max(QThread::idealThreadCount(), 1));
}

int BlurBehindEffect::maxThreadCount() const
{
    return d->maxThreadCount_;
}

void BlurBehindEffect::setCoordinateSystem(Qt::CoordinateSystem _system)
{
    if (d->coordSystem_ == _system)
        return;

    d->coordSystem_ = _system;

    emit repaintRequired();
    update();
}

Qt::CoordinateSystem BlurBehindEffect::coordinateSystem() const
{
    return d->coordSystem_;
}

void BlurBehindEffect::draw(QPainter* _painter)
{
    auto w = qobject_cast<QWidget*>(parent());
    if (!w)
        return;

    const auto bounds = d->region_.boundingRect();

    // grap widget source pixmap
    auto pixmap = d->grabSource(w);
    // render source
    _painter->drawPixmap(0, 0, pixmap);

    if (d->sourceOpacity_ > 0.0 && d->sourceOpacity_ < 1.0)
    {
        if (!d->region_.isEmpty() && bounds != w->rect())
            _painter->setClipRegion(QRegion(w->rect()) -= d->region_);
        else if (!d->region_.isEmpty())
            _painter->setClipRegion(d->region_);

        const double opacity = _painter->opacity();
        _painter->setOpacity(d->sourceOpacity_);
        _painter->drawPixmap(0, 0, pixmap);
        _painter->setOpacity(opacity);
    }

    if (!d->region_.isEmpty() && d->blurRadius_ > 1) {
        const auto dpr = pixmap.devicePixelRatioF();

        const auto s = (QSizeF(bounds.size()) * dpr / d->downsamplingFactor_).toSize();
        const auto r = QRect{ bounds.topLeft() * dpr, bounds.size() * dpr };

        auto pixmapPart = pixmap.copy(r).scaled(
            s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).toImage();

        if (d->sourceImage_ != pixmapPart) {
            d->sourceImage_ = pixmapPart;
            d->cacheKey_ = pixmapPart.cacheKey();
            d->sourceUpdated_ = true;
        }
    }
}

void BlurBehindEffect::render(QPainter* _painter)
{
    if (blurRadius() <= 1 || d->sourceImage_.isNull())
        return;

    if (!d->sourceUpdated_)
        return d->renderImage(_painter, d->blurredImage_, d->backgroundBrush_);

    d->blurredImage_ = d->blurImage(d->sourceImage_);
    d->renderImage(_painter, d->blurredImage_, d->backgroundBrush_);

    d->sourceUpdated_ = false;
}

void BlurBehindEffect::render(
    QPainter* _painter,
    const QPainterPath& _clipPath)
{
    if (blurRadius() <= 1 || d->sourceImage_.isNull())
        return;

    if (_clipPath.isEmpty())
        return render(_painter);

    const auto regionRect = d->region_.boundingRect();
    const auto targetBounds = _clipPath.boundingRect();

    if (!regionRect.contains(targetBounds.toRect()))
    {
        qWarning() << "target path is outside of source region";
        return;
    }

    if (d->sourceUpdated_)
    {
        d->blurredImage_ = d->blurImage(d->sourceImage_);
        d->sourceUpdated_ = false;
    }

    const auto dpr = d->blurredImage_.devicePixelRatioF();
    const auto targetRect = QRectF{ targetBounds.topLeft() * dpr, targetBounds.size() * dpr };

    auto image = d->blurredImage_.scaled(
        regionRect.size() * dpr, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    _painter->setOpacity(d->blurOpacity_);
    _painter->drawImage(QPointF{}, image, targetRect);
}
