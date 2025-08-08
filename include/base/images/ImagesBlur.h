#pragma once

#include <base/images/Images.h>

__BASE_IMAGES_NAMESPACE_BEGIN

inline constexpr auto kDefaultBoxSize = 3;

#ifdef LIB_BASE_ENABLE_QT
base_nodiscard QImage boxBlurImage(
	const QImage& image,
	const QRect& rect,
	int radius);

base_nodiscard QImage boxBlurImage(
	const QImage& image,
	int radius);
#endif

//!
//! \brief Эта функция выполняет быстрое размытие по Гауссу. При многократном применении
//! Размытие в виде прямоугольника приводит к истинному размытию по Гауссу. Для получения хороших результатов достаточно трех проходов
//! Задается типом данных буфера T. Также используется входной буфер
//! как временный и изменяемый в ходе процесса, следовательно, он не может быть постоянным. 
//!
//! Обычно процесс должен чередоваться между горизонтальными и вертикальными проходами
//! столько раз, сколько мы хотим, чтобы проходило размытие прямоугольника. Однако благодаря свойствам размытия прямоугольника
//! разделяемые проходы могут выполняться в любом порядке без изменения результата.
//! Следовательно, в целях повышения производительности алгоритм таков: 
//! - применить N-кратное горизонтальное размытие (-> горизонтальные проходы)
//! - перевернуть буфер изображения (транспонирование)
//! - применить N-кратное размытие по горизонтали (-> вертикальные переходы)
//! - перевернуть буфер изображения (транспонирование)
//!
//! Мы предоставляем две версии функции:
//! - всего 3 прохода 
//! - N проходов (в которых используется больше стандартных обменов
//!
//! \param[in,out] in  -  Исходный буфер
//! \param[in,out] out -  Целевой буфер
//! \param[in] w       -  Ширина изображения
//! \param[in] h       -  Высота изображения
//! \param[in] c       -  Каналы изображения
//! \param[in] sigma   -  Стандартное отклонение по Гауссу
//!
template<typename T>
void FastGaussianBlur(
    T*& in, T*& out, const int w,
    const int h, const int c,
    const float sigma, const int n = kDefaultBoxSize);

enum Policy {
    EXTEND, 
    KERNEL_CROP 
};

template<typename T, int C, Policy P = KERNEL_CROP>
void HorizontalBlur(
    const T* in, T* out, const int w,
    const int h, const int r);

//!
//! \brief Utility template dispatcher function for horizontal_blur. Templated by buffer data type T.
//!
//! \param[in,out] in  -  Исходный буфер
//! \param[in,out] out -  Целевой буфер
//! \param[in] w       -  Ширина изображения
//! \param[in] h       -  Высота изображения
//! \param[in] c       -  Каналы изображения
//! \param[in] r       -  Радиус размытия 
//! 
template<typename T>
void HorizontalBlur(
    const T* in, T* out, const int w,
    const int h, const int c, const int r);

#ifdef LIB_BASE_ENABLE_QT
base_nodiscard QImage RgbBlur(
	QImage&& image,
	bool ignoreAlpha = true);

base_nodiscard QImage stackBlurImage(
	const QImage& image,
	int radius,
	int threadCount = 1);
#endif

__BASE_IMAGES_NAMESPACE_END
