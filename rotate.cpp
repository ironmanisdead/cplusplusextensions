#include "libs/headers/BinMap.hpp"
#include <iostream>
using namespace CustomUtils;

int main() {
	BinMap<int, int> map;
	map.insert(2, 1);
	map.insert(1, 2);
	map.insert(3, 5);
	std::cout << map.search(1)->rotate(false) << std::endl;
	std::cout << map[1] << ' ' << map[2] << ' ' << map[3] << std::endl;
}
