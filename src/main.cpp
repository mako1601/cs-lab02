#include <iostream>
#include <vector>
#include <string>

#include "histogram.h"
#include "svg.h"

const auto SCREEN_WIDTH = 80;
const auto MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

std::vector<double> input_numbers(size_t count);
void show_histogram(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_scaling(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_svg(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_scaling_svg(std::vector<double>bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count, size_t length_dash, size_t length_gap);
void make_histogram(std::vector<double> numbers, size_t bin_count, size_t length_dash, size_t length_gap);

int main() {
	// Ввод данных
	size_t number_count;
	std::cerr << "Enter number count: ";
	std::cin >> number_count;
	std::cerr << "Enter numbers : ";
	const auto numbers = input_numbers(number_count);

	size_t bin_count;
	size_t length_dash, length_gap;
	std::cerr << "Enter bin count: ";
	std::cin >> bin_count;
	std::cerr << "Enter length of the dash: ";
	std::cin >> length_dash;
	std::cerr << "Enter length of the gap: ";
	std::cin >> length_gap;
	make_histogram(numbers, bin_count, length_dash, length_gap);

	return 0;
}

std::vector<double> input_numbers(size_t count) {
	std::vector<double> result(count);
	for (size_t i = 0; i < count; i++) {
		std::cin >> result[i];
	}
	return result;
}

void show_histogram(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count) {
	for (size_t i = 0; i < bin_count; i++) {
		//std::cerr.width(longest_label.length());
		std::cerr << std::right << labels[i] << '|';
		for (size_t j = 0; j < bins[i]; j++) {
			std::cerr << '*';
		}
		std::cerr << std::endl;
	}
}

void show_histogram_scaling(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count) {
	size_t max_count = bins[0];
	for (size_t i = 1; i < bin_count; i++) {
		if (max_count < bins[i])
			max_count = bins[i];
	}

	for (size_t i = 0; i < bin_count; i++) {
		std::cerr.width(longest_label.length());
		std::cerr << std::right << labels[i] << '|';
		for (size_t j = 0; j < static_cast<size_t>(MAX_ASTERISK * (static_cast<double>(bins[i]) / max_count)); j++) {
			std::cerr << '*';
		}
		std::cerr << std::endl;
	}
}

void show_histogram_svg(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count) {
	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

	double top = 0;
	for (size_t i = 0; i < bin_count; i++) {
		const double bin_width = BLOCK_WIDTH * bins[i];
		svg_text(TEXT_LEFT, top + TEXT_BASELINE, labels[i]);
		svg_rect(TEXT_WIDTH + longest_label.length(), top, bin_width, BIN_HEIGHT, get_random_color(), "black");
		top += BIN_HEIGHT;
	}

	svg_end();
}

void show_histogram_scaling_svg(std::vector<double>bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count, size_t length_dash, size_t length_gap) {
	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

	size_t max_count = bins[0];
	for (size_t i = 1; i < bin_count; i++) {
		if (max_count < bins[i])
			max_count = bins[i];
	}

	double top = 0;
	for (size_t i = 0; i < bin_count; i++) {
		const double bin_width = static_cast<size_t>(IMAGE_WIDTH * 0.5 * (static_cast<double>(bins[i]) / max_count));
		svg_text(TEXT_LEFT, top + TEXT_BASELINE, labels[i]);
		svg_rect(TEXT_WIDTH + longest_label.length(), top, bin_width, BIN_HEIGHT, get_random_color(), "black");
		top += BIN_HEIGHT;

		if (i != bin_count - 1) {
			svg_line(top + 4, "black", length_dash, length_gap);
			top += 8;
		}
	}

	svg_end();
}

void make_histogram(std::vector<double> numbers, size_t bin_count, size_t length_dash, size_t length_gap) {
	double min, max;
	find_minmax(numbers, min, max);
	std::vector<double> bins(bin_count);

	// для варианта 1ЛАБ //
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

	//// для 2 этапа 1ЛАБ //
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

	//show_histogram(bins, labels, longest_label, bin_count);
	//show_histogram_scaling(bins, labels, longest_label, bin_count);
	//show_histogram_svg(bins, labels, longest_label, bin_count);
	show_histogram_scaling_svg(bins, labels, longest_label, bin_count, length_dash, length_gap);
}