#include <iostream>
#include <vector>
#include <string>

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

std::vector<double> input_numbers(size_t count);
void output_numbers(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void output_numbers_scaling(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void find_minmax(std::vector<double> numbers, double& min, double& max);
void make_histogram(std::vector<double> numbers, size_t bin_count);

int main() {
	// ���� ������
	size_t number_count;
	std::cerr << "Enter number count: ";
	std::cin >> number_count;
	std::cerr << "Enter numbers : ";
	const auto numbers = input_numbers(number_count);

	size_t bin_count;
	std::cerr << "Enter bin count: ";
	std::cin >> bin_count;
	make_histogram(numbers, bin_count);

	return 0;
}

std::vector<double> input_numbers(size_t count) {
	std::vector<double> result(count);
	for (size_t i = 0; i < count; i++) {
		std::cin >> result[i];
	}
	return result;
}

void output_numbers(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count) {
	for (size_t i = 0; i < bin_count; i++) {
		std::cout.width(longest_label.length());
		std::cout << std::right << labels[i] << '|';
		for (size_t j = 0; j < bins[i]; j++) {
			std::cout << '*';
		}
		std::cout << std::endl;
	}
}

void output_numbers_scaling(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count) {
	size_t max_count = bins[0];
	for (size_t i = 1; i < bin_count; i++) {
		if (max_count < bins[i]) max_count = bins[i];
	}
	for (size_t i = 0; i < bin_count; i++) {
		std::cout.width(longest_label.length());
		std::cout << std::right << labels[i] << '|';
		for (size_t j = 0; j < static_cast<size_t>(MAX_ASTERISK * (static_cast<double>(bins[i]) / max_count)); j++) {
			std::cout << '*';
		}
		std::cout << std::endl;
	}
}

void find_minmax(std::vector<double> numbers, double& min, double& max) {
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

void make_histogram(std::vector<double> numbers, size_t bin_count) {
	double min, max;
	find_minmax(numbers, min, max);
	std::vector<double> bins(bin_count);

	// ��� �������� 1��� //
	std::string longest_label;
	std::string line_label;
	std::vector<std::string> labels(bin_count);
	for (size_t i = 0; i < bin_count; i++) {
		std::cerr << "Enter label " << i + 1 << " column: ";
		std::getline(std::cin >> std::ws, line_label);
		labels[i] = line_label;
		std::cin.clear();
		if (labels[i].length() > longest_label.length()) {
			longest_label = labels[i];
		}
	}

	//// ��� 2 ����� 1��� //
	// labels[0] = std::to_string(bins[0]);
	// longest_label = labels[0];
	// for (size_t i = 1; i < bin_count; i++) {
	// 	labels[i] = std::to_string(bins[i]);
	// 	if (std::to_string(bins[i]).length() > longest_label.length()) {
	// 		longest_label = std::to_string(bins[i]);
	// 	}
	// }


	double bin_size = (max - min) / bin_count;
	for (size_t i = 0; i < numbers.size(); i++) {
		bool found = false;
		for (size_t j = 0; (j < bin_count - 1) && !found; j++) {
			auto low = min + j * bin_size;
			auto high = min + (j + 1) * bin_size;
			if ((low <= numbers[i]) && (numbers[i] < high)) {
				bins[j]++;
				found = true;
			}
		}
		if (!found) {
			bins[bin_count - 1]++;
		}
	}

	output_numbers(bins, labels, longest_label, bin_count);
	output_numbers_scaling(bins, labels, longest_label, bin_count);
}