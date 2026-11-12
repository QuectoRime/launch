#pragma once

#include <algorithm>
#include <bit>
#include <limits>
#include <numbers>
#include <type_traits>

namespace launch {
	constexpr double quiet_nan = std::numeric_limits<double>::quiet_NaN();
	constexpr double signaling_nan = std::numeric_limits<double>::signaling_NaN();
	constexpr double nan = quiet_nan;
	constexpr double infinity = std::numeric_limits<double>::infinity();

	constexpr uint64_t sign_bit(double x) {
		return std::bit_cast<uint64_t>(x) >> 63;
	}

	constexpr uint64_t exponent_bits(double x) {
		return std::bit_cast<uint64_t>(x) >> 52 & 0x7FF;
	}

	constexpr uint64_t mantissa_bits(double x) {
		return std::bit_cast<uint64_t>(x) & 0xFFFFFFFFFFFFF;
	}

	constexpr bool issub(double x) {
		return exponent_bits(x) == 0 && mantissa_bits(x) != 0;
	}

	constexpr bool isinf(double x) {
		return x == infinity || x == -infinity;
	}

	constexpr bool isnan(double x) {
		return exponent_bits(x) == 0x7FF && mantissa_bits(x) != 0;
	}

	constexpr double abs(double x) {
		if (isnan(x)) {
			return nan;
		}
		if (sign_bit(x) == 1) {
			return -x;
		}
		return x;
	}

	template <typename T, typename U>
	constexpr bool feq(T x, U y) {
		if (isnan(x) || isnan(y)) {
			return false;
		}
		return abs(x - y) <= std::max(abs(x), abs(y)) * std::numeric_limits<std::common_type_t<T, U>>::epsilon();
	}

	constexpr long long exponent(double x) {
		const long long e = exponent_bits(x);
		return std::max(e - 1023, -1022ll);
	}

	constexpr double mantissa(double x) {
		double m = std::bit_cast<double>(std::bit_cast<uint64_t>(x) & 0x800FFFFFFFFFFFFF | 0x3FF0000000000000);
		if (issub(x)) {
			if (x < 0) {
				m += 1;
			}
			else {
				m -= 1;
			}
		}
		return m;
	}

	constexpr double mod(double x, double y) {
		if (feq(y, 0) || isnan(x) || isnan(y) || isinf(x)) {
			return nan;
		}
		if (feq(x, 0)) {
			return x;
		}
		if (x < 0) {
			return -mod(-x, y);
		}
		y = abs(y);
		if (x < y) {
			return x;
		}
		const double x_half = x / 2;
		double scale = y;
		while (scale <= x_half) {
			scale *= 2;
		}
		while (scale >= y) {
			if (x >= scale) {
				x -= scale;
			}
			scale /= 2;
		}
		return x;
	}

	constexpr double trunc(double x) {
		if (isinf(x)) {
			return x;
		}
		if (isnan(x)) {
			return nan;
		}
		return x - mod(x, 1);
	}

	constexpr double floor(double x) {
		const double trunc_x = trunc(x);
		if (trunc_x > x) {
			return trunc_x - 1;
		}
		return trunc_x;
	}

	constexpr double ceil(double x) {
		const double trunc_x = trunc(x);
		if (trunc_x < x) {
			return trunc_x + 1;
		}
		return trunc_x;
	}

	constexpr double round(double x) {
		if (isinf(x)) {
			return x;
		}
		if (isnan(x)) {
			return nan;
		}
		if (x >= 0) {
			return floor(x + 0.5);
		}
		return ceil(x - 0.5);
	}

	template <typename T>
	constexpr bool is2pos4(double x) {
		return x >= std::numeric_limits<T>::max();
	}

	template <typename T>
	constexpr bool is2neg4(double x) {
		return x <= std::numeric_limits<T>::lowest();
	}

	constexpr long long itrunc(double x) {
		if (is2pos4<long long>(x)) {
			return std::numeric_limits<long long>::max();
		}
		if (is2neg4<long long>(x)) {
			return std::numeric_limits<long long>::lowest();
		}
		if (isnan(x)) {
			return 0;
		}
		return static_cast<long long>(trunc(x));
	}

	constexpr long long ifloor(double x) {
		if (is2pos4<long long>(x)) {
			return std::numeric_limits<long long>::max();
		}
		if (is2neg4<long long>(x)) {
			return std::numeric_limits<long long>::lowest();
		}
		if (isnan(x)) {
			return 0;
		}
		return static_cast<long long>(floor(x));
	}

	constexpr long long iceil(double x) {
		if (is2pos4<long long>(x)) {
			return std::numeric_limits<long long>::max();
		}
		if (is2neg4<long long>(x)) {
			return std::numeric_limits<long long>::lowest();
		}
		if (isnan(x)) {
			return 0;
		}
		return static_cast<long long>(ceil(x));
	}

	constexpr long long iround(double x) {
		if (is2pos4<long long>(x)) {
			return std::numeric_limits<long long>::max();
		}
		if (is2neg4<long long>(x)) {
			return std::numeric_limits<long long>::lowest();
		}
		if (isnan(x)) {
			return 0;
		}
		return static_cast<long long>(round(x));
	}

	constexpr bool isint(double x) {
		return feq(x, trunc(x));
	}

	constexpr double ipow(double x, long long y) {
		if (y == 0) {
			return 1;
		}
		if (feq(x, 0)) {
			if (y < 0) {
				return infinity;
			}
			return 0;
		}
		const bool neg = y < 0;
		unsigned long long abs_y = abs(y);
		double res = 1, base = x;
		while (abs_y) {
			if (abs_y & 1) {
				res *= base;
			}
			base *= base;
			abs_y >>= 1;
		}
		return neg ? 1 / res : res;
	}

	constexpr double ln(double x) {
		if (x < 0) {
			return nan;
		}
		if (feq(x, 0)) {
			return -infinity;
		}
		const long long e = exponent(x);
		const double
			m = mantissa(x),
			temp = (m - 1) / (m + 1);
		double ln_m = 0;
		for (unsigned long long i = 1; i <= 16; ++i) {
			const unsigned long long temp2 = i * 2 - 1;
			ln_m += ipow(temp, temp2) / temp2;
		}
		ln_m *= 2;
		return e * std::numbers::ln2 + ln_m;
	}

	constexpr double log(double x, double y) {
		return ln(x) / ln(y);
	}

	constexpr double lg(double x) {
		return log(x, 10);
	}

	constexpr double log2(double x) {
		return log(x, 2);
	}

	constexpr double fac(unsigned long long x) {
		double res = 1;
		for (unsigned long long i = 2; i <= x; ++i) {
			res *= i;
		}
		return res;
	}

	constexpr double exp(double x) {
		if (isinf(x)) {
			return x < 0 ? 0 : infinity;
		}
		const long long n = iround(x / std::numbers::ln2);
		const double rem = x - n * std::numbers::ln2;
		double exp_r = 0;
		for (unsigned long long i = 0; i < 16; ++i) {
			exp_r += ipow(rem, i) / fac(i);
		}
		return ipow(2, n) * exp_r;
	}

	constexpr double pow(double x, double y) {
		if (isint(y)) {
			return ipow(x, y);
		}
		if (feq(x, 0)) {
			if (y > 0) {
				return 0;
			}
			return infinity;
		}
		if (x < 0) {
			return nan;
		}
		return exp(y * ln(x));
	}

	constexpr double nrt(double x, double y) {
		if (feq(abs(mod(y, 2)), 1) && x < 0) {
			return -pow(-x, 1 / y);
		}
		return pow(x, 1 / y);
	}

	constexpr double sqrt(double x) {
		return nrt(x, 2);
	}

	constexpr double cbrt(double x) {
		return nrt(x, 3);
	}

	constexpr double hypot(double x, double y) {
		if (isinf(x) || isinf(y)) {
			return infinity;
		}
		const double a = std::max(abs(x), abs(y));
		if (feq(a, 0)) {
			return 0;
		}
		const double b = std::min(abs(x), abs(y));
		return a * sqrt(1 + pow(b / a, 2));
	}

	constexpr double dtor(double deg) {
		return deg * std::numbers::pi / 180;
	}

	constexpr double rtod(double rad) {
		return rad * 180 / std::numbers::pi;
	}

	constexpr double sin(double x) {
		x = mod(x, 2 * std::numbers::pi);
		double res = 0;
		for (unsigned long long i = 1; i <= 16; ++i) {
			const unsigned long long temp = i * 2 - 1;
			const double sign = i % 2 == 0 ? -1 : 1;
			res += ipow(x, temp) / fac(temp) * sign;
		}
		return res;
	}

	constexpr double cos(double x) {
		return sin(std::numbers::pi / 2 - x);
	}

	constexpr double tan(double x) {
		return sin(x) / cos(x);
	}

	constexpr double arcsin(double x) {
		if (x < -1 || x > 1) {
			return nan;
		}
		if (x < 0) {
			return -arcsin(-x);
		}
		if (x > 0.5) {
			return std::numbers::pi / 2 - 2 * arcsin(sqrt((1 - x) / 2));
		}
		double res = 0;
		for (unsigned long long i = 0; i < 16; ++i) {
			const unsigned long long temp = 2 * i + 1;
			res += fac(2 * i) / ipow(fac(i), 2) / ipow(4, i) / temp * ipow(x, temp);
		}
		return res;
	}

	constexpr double arccos(double x) {
		return std::numbers::pi / 2 - arcsin(x);
	}

	constexpr double arctan(double x) {
		const double temp = x * x + 1;
		if (isinf(temp)) {
			return x > 0 ? std::numbers::pi / 2 : -std::numbers::pi / 2;
		}
		return arcsin(x / sqrt(temp));
	}

	constexpr double arctan2(double y, double x) {
		if (feq(x, 0)) {
			if (feq(y, 0)) {
				return 0;
			}
			if (y > 0) {
				return std::numbers::pi / 2;
			}
			if (y < 0) {
				return -std::numbers::pi / 2;
			}
		}
		if (x > 0) {
			return arctan(y / x);
		}
		if (y >= 0) {
			return arctan(y / x) + std::numbers::pi;
		}
		return arctan(y / x) - std::numbers::pi;
	}

	constexpr double sinh(double x) {
		return (exp(x) - exp(-x)) / 2;
	}

	constexpr double cosh(double x) {
		return (exp(x) + exp(-x)) / 2;
	}

	constexpr double tanh(double x) {
		const double exp_2x = exp(2 * x);
		if (isinf(exp_2x)) {
			return 1;
		}
		return (exp_2x - 1) / (exp_2x + 1);
	}

	constexpr double arsinh(double x) {
		return ln(x + sqrt(x * x + 1));
	}

	constexpr double arcosh(double x) {
		if (x < 1) {
			return nan;
		}
		return ln(x + sqrt(x * x - 1));
	}

	constexpr double artanh(double x) {
		if (abs(x) >= 1) {
			return nan;
		}
		return ln((1 + x) / (1 - x)) / 2;
	}
}