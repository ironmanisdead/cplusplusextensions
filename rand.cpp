#include <iostream>
#include <cstdlib>

int main() {
	std::srand(1);
	std::cout << std::rand() << std::endl;
	bool al;
	std::cin >> std::boolalpha;
	std::cin >> al;
	std::cout << al << std::endl;
	if (al)
		std::srand(1);
	std::cout << std::rand() << std::endl;
}
