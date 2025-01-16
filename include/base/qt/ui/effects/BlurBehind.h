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
        [[nodiscard]] double sourceOpacity() const;

        void setBlurOpacity(double _opacity);
        [[nodiscard]] double blurOpacity() const;

        void setBackgroundBrush(const QBrush& _brush);
        [[nodiscard]] QBrush backgroundBrush() const;

        void setBlurMethod(BlurMethod _method);
        [[nodiscard]] BlurMethod blurMethod() const;

        void setRegion(const QRegion& _sourceRegion);
        [[nodiscard]] const QRegion& region() const;

        void setBlurRadius(int _radius);
        [[nodiscard]] int blurRadius() const;

        void setDownsampleFactor(double _factor);
        [[nodiscard]] double downsampleFactor() const;

        void setMaxThreadCount(int _nthreads);
        [[nodiscard]] int maxThreadCount() const;

        void setCoordinateSystem(Qt::CoordinateSystem _system);
        [[nodiscard]] Qt::CoordinateSystem coordinateSystem() const;
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