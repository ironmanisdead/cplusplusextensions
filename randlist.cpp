#include <CPPExtensions/BinMap.hpp>
#include <iostream>
using namespace CPPExtensions;
int main() {
	String printer;
	printer.allocate(100);
	BinMap<int, int> map;
	printer.set("unsorted list");
	printer.write(Utils::std_out);
	for (int i = 0; i < 20; i++) {
		int key;
		do
			key = Utils::rand() % 30;
		while (map.find(key));
		int val = Utils::rand() % 50;
		map.insert(key, val);
		printer.set(key, ' ', val, '\n');
		printer.write(Utils::std_out);
	}
	Utils::puts(Utils::std_out, "sorted list:");
	for (auto& node : map) {
		printer.set(node.getKey(), ' ', node.getValue(), '\n');
		printer.write(Utils::std_out);
	}
}
