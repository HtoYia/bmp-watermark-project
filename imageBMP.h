#pragma once
#include <stddef.h>
#include <vector>
#include "color.h"

struct ImageBMP {
private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<Color> colors_;

public:
    ImageBMP(size_t width, size_t height, const std::vector<Color>& colors);
    ImageBMP(size_t width, size_t height);
    ImageBMP();

    size_t GetWidth() const;
    size_t GetHeight() const;

    Color& Pixel(size_t r, size_t c);
    const Color& Pixel(size_t r, size_t c) const;
};
