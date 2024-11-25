#include <thread>
#include <array>
#include <iostream>
#include <barrier>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution distr{128.0, 15.0};


struct Picture {
    Picture(unsigned width, unsigned height) {
        const unsigned pixels = width * height;
        r.resize(pixels);
        g.resize(pixels);
        b.resize(pixels);
    }

    std::vector<unsigned char> r;
    std::vector<unsigned char> g;
    std::vector<unsigned char> b;
};

struct StretchContrast {
    StretchContrast(Picture & pic): picture{pic} {}
    Picture & picture;
    unsigned char min = 255;
    unsigned char max = 0;

    static std::pair<unsigned char, unsigned char> analyze_channel(const std::vector<unsigned char> & channel) {
        unsigned char min = 255;
        unsigned char max = 0;
        for (auto pixel : channel) {
            if (pixel < min)
            {
                min = pixel;
            }
            if (pixel > max)
            {
                max = pixel;
            }
        }
        return {min, max};
    }
    static void stretch_channel(std::vector<unsigned char> & channel, std::pair<unsigned char, unsigned char> limits) {
        const long span = limits.second - limits.first;
        for (auto & pixel : channel) {
            const long distance = pixel - limits.first;
            const long stretched = (distance * 256) / span;
            pixel = static_cast<unsigned char>(stretched);
        }
    }
};

int main(int, char **) {
    std::vector<std::jthread> threads;

    // Create input picture with random data
    auto pic = std::make_unique<Picture>(640, 480);

    auto random_int = []{ return std::round(distr(gen)); };
    for (auto & pixel : pic->r) {
        pixel = random_int();
    }
    for (auto & pixel : pic->g) {
        pixel = random_int();
    }
    for (auto & pixel : pic->b) {
        pixel = random_int();
    }

    StretchContrast strech_op{*pic};

    auto r_range = strech_op.analyze_channel(strech_op.picture.r);
    auto g_range = strech_op.analyze_channel(strech_op.picture.g);
    auto b_range = strech_op.analyze_channel(strech_op.picture.b);

    auto pix_min = std::min(r_range.first, g_range.first);
    pix_min = std::min(pix_min, b_range.first);

    auto pix_max = std::max(r_range.second, g_range.second);
    pix_max = std::max(pix_max, b_range.second);

    std::cout << "Min: " << static_cast<int>(pix_min) << " Max: " << static_cast<int>(pix_max) << std::endl;
    auto range = std::make_pair(pix_min, pix_max);

    strech_op.stretch_channel(strech_op.picture.r, range);
    strech_op.stretch_channel(strech_op.picture.g, range);
    strech_op.stretch_channel(strech_op.picture.b, range);

    return 0;
}
