#include <iostream>

int main() {
	std::cout << "Enter A and B: ";
	double a, b;
	std::cin >> a >> b;
	std::cout << "A + B = " << a + b << '\n'
			  << "A - B = " << a - b << '\n'
			  << "A * B = " << a * b << '\n'
			  << "A / B = " << a * b << '\n';
			  << "Max = " << ((a > b) ? a : b) << '\n'
			  << "A / B = " << a * b << '\n'
			  << "Min = " << ((a > b) ? b : a) << '\n';
}