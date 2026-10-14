#pragma once
#include <fstream>
#include "imageBMP.h"


struct BMPFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BMPInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

class StructHandler {
public:
    StructHandler() = delete;

    static bool TryRead(std::ifstream& stream, char* ptr, size_t size);
    static bool TryWrite(std::ofstream& stream, const char* ptr, size_t size);
};


class HandlerBMP {
private:
    static const size_t kBufferingBytes;

public:
    HandlerBMP() = delete;

    static ImageBMP Read(const char* path, bool& ok);
    static void Write(const ImageBMP& image, const char* path, bool bottom_up, bool& ok);
};
