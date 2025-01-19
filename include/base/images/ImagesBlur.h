#pragma once

#include <base/images/Images.h>

namespace base::images {
    inline constexpr auto kDefaultBoxSize = 3;

	Image boxBlurImage(
		const Image& image,
		const Rect<int32>& rect,
		int radius);

	Image boxBlurImage(
		const Image& image,
		int radius);

    //!
   //! \brief ��� ������� ��������� ������� �������� �� ������. ��� ������������ ����������
   //! �������� � ���� �������������� �������� � ��������� �������� �� ������. ��� ��������� ������� ����������� ���������� ���� ��������
   //! �������� ����� ������ ������ T. ����� ������������ ������� �����
   //! ��� ��������� � ���������� � ���� ��������, �������������, �� �� ����� ���� ����������. 
   //!
   //! ������ ������� ������ ������������ ����� ��������������� � ������������� ���������
   //! ������� ���, ������� �� �����, ����� ��������� �������� ��������������. ������ ��������� ��������� �������� ��������������
   //! ����������� ������� ����� ����������� � ����� ������� ��� ��������� ����������.
   //! �������������, � ����� ��������� ������������������ �������� �����: 
   //! - ��������� N-������� �������������� �������� (-> �������������� �������)
   //! - ����������� ����� ����������� (����������������)
   //! - ��������� N-������� �������� �� ����������� (-> ������������ ��������)
   //! - ����������� ����� ����������� (����������������)
   //!
   //! �� ������������� ��� ������ �������:
   //! - ����� 3 ������� 
   //! - N �������� (� ������� ������������ ������ ����������� �������
   //!
   //! \param[in,out] in  -  �������� �����
   //! \param[in,out] out -  ������� �����
   //! \param[in] w       -  ������ �����������
   //! \param[in] h       -  ������ �����������
   //! \param[in] c       -  ������ �����������
   //! \param[in] sigma   -  ����������� ���������� �� ������
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
    //! \param[in,out] in  -  �������� �����
    //! \param[in,out] out -  ������� �����
    //! \param[in] w       -  ������ �����������
    //! \param[in] h       -  ������ �����������
    //! \param[in] c       -  ������ �����������
    //! \param[in] r       -  ������ �������� 
    //! 
    template<typename T>
    void HorizontalBlur(
        const T* in, T* out, const int w,
        const int h, const int c, const int r);

	[[nodiscard]] Image RgbBlur(
		QImage&& image,
		bool ignoreAlpha = true);

	[[nodiscard]] Image stackBlurImage(
		const Image& image,
		int radius,
		int threadCount = 1);
} // namespace base::images