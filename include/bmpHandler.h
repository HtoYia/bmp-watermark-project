#pragma once
#include <fstream>
#include "imageBMP.h"


#pragma pack(push, 1)
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
#pragma pack(pop)


static_assert(sizeof(BMPFileHeader) == 14, "BMPFileHeader must be 14 bytes");

static_assert(sizeof(BMPInfoHeader) == 40, "BMPInfoHeader must be 40 bytes");


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

    static bool Read(const char* path, ImageBMP& out);
    static bool Write(const ImageBMP& image, const char* path, bool top_down);
};
