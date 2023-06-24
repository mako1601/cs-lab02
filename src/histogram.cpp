#include "histogram.h"

void find_minmax(std::vector<double> numbers, double& min, double& max) {
	if (!numbers.size()) {
		return;
	}

	min = numbers[0];
	max = numbers[0];
	for (double x : numbers) {
		if (x < min) {
			min = x;
		}
		else if (x > max) {
			max = x;
		}
	}
}