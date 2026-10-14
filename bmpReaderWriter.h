#pragma once
#include "imageBMP.h"

class ReaderWriterBMP {
public:
    ReaderWriterBMP() = delete;

    static ImageBMP Read(const char* path, bool& ok);
    static void Write(const ImageBMP& image, const char* path, bool& ok);
};
