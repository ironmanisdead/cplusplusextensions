#include <iostream>
#include <cstdlib>

int main() {
	std::srand(1);
	std::cout << std::rand() << std::endl;
	std::srand(1);
	std::cout << std::rand() << std::endl;
}
