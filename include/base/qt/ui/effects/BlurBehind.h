#pragma once
#include <memory>
#include <QGraphicsEffect>


namespace base::qt::ui::effects {
    class BlurBehindEffect :
        public QGraphicsEffect
    {
        Q_OBJECT
    public:
        enum class BlurMethod
        {
            BoxBlur,
            StackBlur,
            GLBlur
        };

        Q_ENUM(BlurMethod)

         BlurBehindEffect(QWidget* _parent = nullptr);
        ~BlurBehindEffect();

        void render(QPainter* _painter);
        void render(
            QPainter* _painter, 
            const QPainterPath& _clipPath);

        void setSourceOpacity(double _opacity);
          double sourceOpacity() const;

        void setBlurOpacity(double _opacity);
          double blurOpacity() const;

        void setBackgroundBrush(const QBrush& _brush);
          QBrush backgroundBrush() const;

        void setBlurMethod(BlurMethod _method);
          BlurMethod blurMethod() const;

        void setRegion(const QRegion& _sourceRegion);
          const QRegion& region() const;

        void setBlurRadius(int _radius);
          int blurRadius() const;

        void setDownsampleFactor(double _factor);
          double downsampleFactor() const;

        void setMaxThreadCount(int _nthreads);
          int maxThreadCount() const;

        void setCoordinateSystem(Qt::CoordinateSystem _system);
          Qt::CoordinateSystem coordinateSystem() const;
    protected:
        void draw(QPainter* _painter) override;

    Q_SIGNALS:
        void blurRadiusChanged(int radius);
        void blurOpacityChanged(double opacity);

        void sourceOpacityChanged(double opacity);
        void downsampleFactorChanged(double factor);

        void backgroundBrushChanged(const QBrush& brush);
        void repaintRequired();
    private:
        std::unique_ptr<class BlurBehindEffectPrivate> d;
    };
}