#include <iostream>
#include <string>
#include <random>

const auto IMAGE_WIDTH = 400;
const auto IMAGE_HEIGHT = 300;
const auto TEXT_LEFT = 20;
const auto TEXT_BASELINE = 20;
const auto TEXT_WIDTH = 60;
const auto BIN_HEIGHT = 30;
const auto BLOCK_WIDTH = 10;

std::vector<std::string> COLORS = {"#DC143C", "#98FB98", "#20B2AA", "#00FFFF", "#FFFF00", "#FF00FF",
                                   "#FFDEAD", "#D2691E", "#8000FF", "#987654", "#7B68EE", "#FFEC8B",
                                   "#9FE2BF", "#D2B48C", "#464531", "#C1876B", "#CDB891", "#734A12"};

std::string get_random_color() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, COLORS.size() - 1);
    return COLORS[dist(rng)];
}

void svg_begin(double width, double height) {
    std::cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    std::cout << "<svg ";
    std::cout << "width='" << width << "' ";
    std::cout << "height='" << height << "' ";
    std::cout << "viewBox='0 0 " << width << " " << height << "' ";
    std::cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end() {
    std::cout << "</svg>\n";
}

void svg_text(double left, double baseline, std::string text) {
    std::cout << "<text x='"<< left <<"' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, std::string color, std::string color_stroke) {
    std::cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' fill='" << color;
    std::cout << "' stroke='" << color_stroke << "' stroke-width='" << 1;
    std::cout <<"'/>";
}

void svg_line(double y, std::string color_stroke, double length_dash, double length_gap) {
    std::cout << "<line x1='" << 0 << "' y1='" << y << "' x2 ='" << IMAGE_WIDTH << "' y2 ='" << y;
    std::cout << "' stroke='" << color_stroke << "' stroke-dasharray='" << length_dash << " " << length_gap <<"'/>";
}
