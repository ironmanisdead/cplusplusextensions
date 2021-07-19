#pragma once
namespace CPPExtensions {
	class Fraction {
		public:
			static constexpr double frac(double init) noexcept {
				return init - (int)init;
			}
			static constexpr long round(double num) noexcept {
				return (long)(num + 0.5);
			}
			static constexpr long gcd(long a, long b) noexcept {
				while (a != b) {
					if (a == 0)
						return b;
					if (b == 0)
						return a;
					if (a > b)
						a -= b;
					else
						b -= a;
				}
				return a;
			}
		private:
			static constexpr long precision = 10000;
			long _cd;
			long _num;
			long _den;
		public:
			constexpr Fraction(long n) noexcept : _cd(1), _num(n), _den(1) {}
			constexpr Fraction(long n, long d) noexcept : 
				_cd(gcd(n, d)), _num(n / _cd), _den(d / _cd) {}
			constexpr Fraction(double d) noexcept : _cd(0), _num(0), _den(0) {
				const long approx = round(frac(d) * precision);
				const long total = round(d * precision);
				_cd = gcd(approx, precision);
				_den = precision / _cd;
				_num = total / _cd;
			}
			constexpr long num() noexcept {
				return _num;
			}
			constexpr long den() noexcept {
				return _den;
			}
	};
}
