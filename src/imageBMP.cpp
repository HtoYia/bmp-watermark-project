#include <stdexcept>
#include "../include/imageBMP.h"


ImageBMP::ImageBMP(size_t width, size_t height, const std::vector<Color>& colors)
    : width_(width), height_(height), colors_(colors) {
    if (colors.size() != width_ * height_)
        throw std::invalid_argument("Invalid size: width*height != colors.size()");
}

ImageBMP::ImageBMP(size_t width, size_t height)
    : ImageBMP(width, height, std::vector<Color>(width*height)) { }

ImageBMP::ImageBMP()
    : ImageBMP(0, 0) { }


size_t ImageBMP::GetWidth() const { return width_; }
size_t ImageBMP::GetHeight() const { return height_; }


Color& ImageBMP::Pixel(size_t r, size_t c) {
    if (r >= height_ || c >= width_) throw std::out_of_range("Index out of range");
    return colors_[r * width_ + c];
}

const Color& ImageBMP::Pixel(size_t r, size_t c) const {
    if (r >= height_ || c >= width_) throw std::out_of_range("Index out of range");
    return colors_[r * width_ + c];
}
