#pragma once
#include "predefs.hpp"
#include <iostream>
namespace CPPExtensions {
	class Fraction {
		public:
			using schar = Utils::schar;
			using ulong = Utils::ulong;
		private:
			static constexpr ulong _abs(long num, schar* sign = nullptr) noexcept {
				if (num < 0) {
					if (sign)
						*sign = -(*sign);
					return -num;
				}
				return num;
			}
			static constexpr double _abs(double num, schar* sign = nullptr) noexcept {
				if (num < 0) {
					if (sign)
						*sign = -(*sign);
					return -num;
				}
				return num;
			}
		public:
			static constexpr ulong gcd(ulong a, ulong b) noexcept {
				if (a == 0)
					return 0;
				if (b == 0)
					return 0;
				while (a != b) {
					if ((a == 1) || (b == 1))
						return 1;
					if (a > b)
						a -= b;
					else
						b -= a;
				}
				return a;
			}
		private:
			static constexpr double _frac(double init) noexcept {
				return init - (long)init;
			}
			static constexpr long round(double num) noexcept {
				return (long)(num + 0.5);
			}
			static constexpr long precision = 1000000000;
			long _num;
			ulong _den;
		public:
			constexpr Fraction(long n) noexcept : _num(n), _den(1) {}
			constexpr Fraction(int n) noexcept : _num(n), _den(1) {}
			constexpr Fraction(short n) noexcept : _num(n), _den(1) {}
			constexpr Fraction(long n, long d) noexcept : _num(0), _den(0) {
				schar flag = 1;
				ulong num = _abs(n, &flag);
				ulong den = _abs(d, &flag);
				ulong cd = gcd(num, den);
				_num = flag * long(num / cd);
				_den = den / cd;
			}
			constexpr Fraction(double d) noexcept : _num(0), _den(0) {
				schar flag = 1;
				double v = _abs(d, &flag);
				const long approx = round(_frac(v) * precision);
				const long total = round(v * precision);
				ulong cd = gcd(approx, precision);
				_num = flag * (total / cd);
				_den = precision / cd;
			}
			constexpr long num() const noexcept {
				return _num;
			}
			constexpr long den() const noexcept {
				return _den;
			}
	};
}
