#include <CPPExtensions/BinMap.hpp>
#include <iostream>
using namespace CPPExtensions;
int main() {
	BinMap<int, int> map;
	std::cout << "unsorted list:" << std::endl;
	for (int i = 0; i < 20; i++) {
		int key;
		do
			key = Utils::rand() % 30;
		while (map.find(key));
		int val = Utils::rand() % 50;
		map.insert(key, val);
		std::cout << key << ' ' << val << std::endl;
	}
	std::cout << "\nsorted list:" << std::endl;
	for (auto& node : map)
		std::cout << node.getKey() << ' ' << node.getValue() << std::endl;
}
