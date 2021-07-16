#pragma once
namespace CPPExtensions {
	class Fraction {
		public:
			static constexpr long gcd(long a, long b) noexcept {
				while (true) {
					if (a == b)
						return a;
					if (a == 0)
						return b;
					if (b == 0)
						return a;
					if (a > b)
						a -= b;
					else
						b -= a;
				}
			}
		private:
			long tmp;
			long num;
			long den;
		public:
			constexpr Fraction(long n, long d) noexcept : 
				tmp(gcd(n, d)), num(n / tmp), den(d / tmp) {}
	};
}
