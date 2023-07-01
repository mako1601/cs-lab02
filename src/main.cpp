#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <curl/curl.h>

#include "histogram.h"
#include "svg.h"

const auto SCREEN_WIDTH = 80;
const auto MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

struct Input {
	std::vector<double> numbers;
	size_t bin_count{};
	size_t length_dash{};
	size_t length_gap{};
	std::vector<std::string> labels;
	std::string longest_label;
};

Input read_input(std::istream& in, bool prompt);
Input download(const std::string& adress);
size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx);
std::vector<double> input_numbers(std::istream& in, size_t count);
void show_histogram(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_scaling(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_svg(std::vector<double> bins, std::vector<std::string> labels, std::string longest_label, size_t bin_count);
void show_histogram_scaling_svg(std::vector<double>bins, Input data);
void make_histogram(Input data);

int main(int argc, char* argv[]) {
	Input input;

	// Ввод данных
	if (argc > 1) {
		input = download(argv[1]);
	}
	else {
		input = read_input(std::cin, true);
	}

	// Создание гистограммы
	make_histogram(input);

	return 0;
}

Input read_input(std::istream& in, bool prompt) {
	Input data;
	size_t number_count;
	std::string line_label;

	if (prompt) {
		std::cerr << "Enter number count: ";
		in >> number_count;

		std::cerr << "Enter numbers : ";
		data.numbers = input_numbers(std::cin, number_count);
		std::cerr << "Enter bin count: ";
		in >> data.bin_count;

		std::cerr << "Enter length of the dash: ";
		in >> data.length_dash;
		std::cerr << "Enter length of the gap: ";
		in >> data.length_gap;

		data.labels.resize(data.bin_count);
		for (size_t i = 0; i < data.bin_count; i++) {
			std::cerr << "Enter label " << i + 1 << " column: ";
			std::getline(in >> std::ws, line_label);
			data.labels[i] = line_label;
			in.clear();
			if (data.labels[i].length() > data.longest_label.length()) {
				data.longest_label = data.labels[i];
			}
		}
	}
	else {
		in >> number_count;
		data.numbers = input_numbers(std::cin, number_count);
		in >> data.bin_count >> data.length_dash >> data.length_gap;

		data.labels.resize(data.bin_count);
		for (size_t i = 0; i < data.bin_count; i++) {
			std::getline(in >> std::ws, line_label);
			data.labels[i] = line_label;
			in.clear();
			if (data.labels[i].length() > data.longest_label.length()) {
				data.longest_label = data.labels[i];
			}
		}
	}

	return data;
}

Input download(const std::string& address) {
	std::stringstream buffer;
	CURL* curl = curl_easy_init();
	if (curl) {
		CURLcode res;
		curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

		res = curl_easy_perform(curl);
		if (res == CURLE_HTTP_RETURNED_ERROR) {
			std::cerr << "FAILED TO LOAD FILE" << std::endl;
		}
		if (res != CURLE_OK) {
			std::cerr << "ERROR " << res << ": " << curl_easy_strerror(res) << std::endl;
			exit(1);
		}
		curl_easy_cleanup(curl);
	}

	return read_input(buffer, false);
}

size_t write_data(void* items, size_t item_size, size_t item_count, void* ctx) {
	std::stringstream* buffer = reinterpret_cast<std::stringstream*>(ctx);
	const char* data = reinterpret_cast<char*>(items);
	size_t data_size = item_count * item_size;
	buffer->write(data, data_size);
	return data_size;
}

std::vector<double> input_numbers(std::istream& in, size_t count) {
	std::vector<double> result(count);
	for (size_t i = 0; i < count; i++) {
		in >> result[i];
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

void show_histogram_scaling_svg(std::vector<double>bins, Input data) {
	svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

	size_t max_count = bins[0];
	for (size_t i = 1; i < data.bin_count; i++) {
		if (max_count < bins[i])
			max_count = bins[i];
	}

	double top = 0;
	for (size_t i = 0; i < data.bin_count; i++) {
		const double bin_width = static_cast<size_t>(IMAGE_WIDTH * 0.5 * (static_cast<double>(bins[i]) / max_count));
		svg_text(TEXT_LEFT, top + TEXT_BASELINE, data.labels[i]);
		svg_rect(TEXT_WIDTH + data.longest_label.length(), top, bin_width, BIN_HEIGHT, get_random_color(), "black");
		top += BIN_HEIGHT;

		if (i != data.bin_count - 1) {
			svg_line(top + 4, "black", data.length_dash, data.length_gap);
			top += 8;
		}
	}

	svg_end();
}

void make_histogram(Input data) {
	double min, max;
	find_minmax(data.numbers, min, max);
	std::vector<double> bins(data.bin_count);

	// для варианта 1ЛАБ //
	//std::string longest_label;
	//std::string line_label;
	//std::vector<std::string> labels(data.bin_count);
	//for (size_t i = 0; i < data.bin_count; i++) {
	//	std::cerr << "Enter label " << i + 1 << " column: ";
	//	std::getline(std::cin >> std::ws, line_label);
	//	labels[i] = line_label;
	//	std::cin.clear();
	//	if (labels[i].length() > longest_label.length()) {
	//		longest_label = labels[i];
	//	}
	//}

	//// для 2 этапа 1ЛАБ //
	// labels[0] = std::to_string(bins[0]);
	// longest_label = labels[0];
	// for (size_t i = 1; i < data.bin_count; i++) {
	// 	labels[i] = std::to_string(bins[i]);
	// 	if (std::to_string(bins[i]).length() > longest_label.length()) {
	// 		longest_label = std::to_string(bins[i]);
	// 	}
	// }


	double bin_size = (max - min) / data.bin_count;
	for (size_t i = 0; i < data.numbers.size(); i++) {
		bool found = false;
		for (size_t j = 0; (j < data.bin_count - 1) && !found; j++) {
			auto low = min + j * bin_size;
			auto high = min + (j + 1) * bin_size;
			if ((low <= data.numbers[i]) && (data.numbers[i] < high)) {
				bins[j]++;
				found = true;
			}
		}
		if (!found) {
			bins[data.bin_count - 1]++;
		}
	}

	//show_histogram(bins, labels, longest_label, bin_count);
	//show_histogram_scaling(bins, labels, longest_label, bin_count);
	//show_histogram_svg(bins, labels, longest_label, bin_count);
	show_histogram_scaling_svg(bins, data);
}