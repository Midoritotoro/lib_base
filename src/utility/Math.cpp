#include "Math.h"

#include "base/Utility.h"

#include <cfenv>
#include <cmath>

namespace base {
    int64_t GCD(
        int64_t a,
        int64_t b)
    {
        while (b)
        {
            int64_t c = a % b;
            a = b;
            b = c;
        }
        return a;
    }

    int64_t LCM(int64_t a, int64_t b) {
        return a * b / GCD(a, b);
    }

    bool UnsignedReduce(
        unsigned* pi_dst_nom, unsigned* pi_dst_den,
        uint64_t i_nom, uint64_t i_den, uint64_t i_max)
    {
        bool b_exact = 1;
        uint64_t i_gcd;

        if (i_den == 0)
        {
            *pi_dst_nom = 0;
            *pi_dst_den = 1;
            return 1;
        }

        i_gcd = GCD(i_nom, i_den);
        i_nom /= i_gcd;
        i_den /= i_gcd;

        if (i_max == 0) i_max = INT64_C(0xFFFFFFFF);

        if (i_nom > i_max || i_den > i_max)
        {
            uint64_t i_a0_num = 0, i_a0_den = 1, i_a1_num = 1, i_a1_den = 0;
            b_exact = 0;

            for (; ; )
            {
                uint64_t i_x = i_nom / i_den;
                uint64_t i_a2n = i_x * i_a1_num + i_a0_num;
                uint64_t i_a2d = i_x * i_a1_den + i_a0_den;

                if (i_a2n > i_max || i_a2d > i_max) break;

                i_nom %= i_den;

                i_a0_num = i_a1_num; i_a0_den = i_a1_den;
                i_a1_num = i_a2n; i_a1_den = i_a2d;
                if (i_nom == 0) break;
                i_x = i_nom; i_nom = i_den; i_den = i_x;
            }
            i_nom = i_a1_num;
            i_den = i_a1_den;
        }

        *pi_dst_nom = i_nom;
        *pi_dst_den = i_den;

        return b_exact;
    }

    double SafeRound(double value) {
        if (std::isnan(value))
            return -1;

        if (const auto result = std::round(value); !std::isnan(result)) {
            return result;
        }
        const auto errors = std::fetestexcept(FE_ALL_EXCEPT);
        if (const auto result = std::round(value); !std::isnan(result)) {
            return result;
        }

        std::feclearexcept(FE_ALL_EXCEPT);
        if (const auto result = std::round(value); !std::isnan(result)) {
            return result;
        }

        return value;
    }
} // namespace base