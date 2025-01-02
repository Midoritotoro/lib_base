#pragma once

#include <vector>

#include <cfenv>
#include <algorithm>

#include "utility/Accumulate.h"

#include "utility/Round.h"
#include <iostream>

#if _has_include(gsl/gsl)
/**
 *����� ������ �� ������� ������� ��������� ���������� ����������� �� ���������� ����� ����� ���� �����
 *\param name - "���" ������� ���������
*/
#define measureExecutionTime(name) \
		 const auto ms = Time::now(); \
		 const auto timer = gsl::finally([] { \
			std::cout << name << " completed for: " \
			<< Time::now() - ms << " ms" << '\n'; });
#endif


namespace core::utility {
	
} // namespace core::utility