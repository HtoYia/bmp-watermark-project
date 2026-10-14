#pragma once
#include <fstream>
#include "imageBMP.h"


class HandlerBMP {
private:
    static const size_t kBufferingBytes;

public:
    HandlerBMP() = delete;

    static bool Read(const char* path, ImageBMP& out);
    static bool Write(const ImageBMP& image, const char* path, bool top_down);
};