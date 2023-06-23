#include <iostream>

int main() {
	std::cout << "Enter A and B: ";
	int a, b;
	std::cin >> a >> b;
	std::cout << "A + B = " << a + b << '\n'
			  << "A - B = " << a - b << '\n';
}