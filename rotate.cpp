#include <CustomUtils/BinMap.hpp>
#include <iostream>
using namespace CustomUtils;

int main() {
	BinMap<int, int> map;
	map.insert(2, 1);
	map.insert(1, 2);
	map.insert(3, 5);
	auto* root = map.getRoot();
	std::cout << root->getKey() << ' ' << root->getValue() << std::endl;
	map.search(2)->rotate(true);
	root = map.getRoot();
	std::cout << root->getKey() << ' ' << root->getValue() << std::endl;
	std::cout << map[1] << ' ' << map[2] << ' ' << map[3] << std::endl;
}
