#include "libs/headers/BinTree.hpp"
#include <chrono>
#include <cstdlib>

int main() {
	using clock = std::chrono::system_clock;
	auto moment =
		std::chrono::duration_cast<std::chrono::milliseconds>(
				clock::now().time_since_epoch());
	unsigned long long ms = moment.count();
	std::srand(ms);
}
