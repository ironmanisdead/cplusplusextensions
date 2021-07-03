#include "libs/headers/BinMap.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
using namespace CustomUtils;
int main() {
	using clock = std::chrono::system_clock;
	auto moment =
		std::chrono::duration_cast<std::chrono::milliseconds>(
				clock::now().time_since_epoch());
	std::srand(moment.count());
	BinMap<int, int> map;
	for (int i = 0; i < 20; i++) {
		int key;
		do {
			key = std::rand() % 30;
		} while (map.find(key));
		int val = std::rand() % 50;
		map.insert(key, val);
		std::cout << key << ' ' << val << std::endl;
	}
	std::cout << std::endl;
	for (auto& node : map)
		std::cout << node.getKey() << ' ' << node.getValue() << std::endl;
}
