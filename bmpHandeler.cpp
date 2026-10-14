#include "bmpHandler.h"


bool StructHandler::TryRead(std::ifstream& stream, char* ptr, size_t size) {
    char* buf = new char[size];
    if (!stream.read(buf, size))
        return false;

    for (size_t i = 0; i < size; i++)
        ptr[i] = buf[i];
    
    delete [] buf;
    return true;
}

bool StructHandler::TryWrite(std::ofstream& stream, const char* ptr, size_t size) {
    if (!stream.write(ptr, size))
        return false;
    
    return true;
}


const size_t HandlerBMP::kBufferingBytes = 4096;

bool HandlerBMP::Read(const char* path, ImageBMP& out) {
    std::ifstream stream(path, std::ios_base::binary);

    // Reading headers
    BMPFileHeader bmp_fh;
    BMPInfoHeader bmp_ih;

    if (!stream.is_open()
        || !StructHandler::TryRead(stream, (char*)(&bmp_fh), sizeof(BMPFileHeader))
        || !StructHandler::TryRead(stream, (char*)(&bmp_ih), sizeof(BMPInfoHeader))
    ) {
        return false;
    }
    if (!(bmp_fh.bfType == 0x4D42       // 'BM' - signature
        && bmp_fh.bfOffBits == 54       // sum headers size
        && bmp_ih.biSize == 40          // size of BEATMAPINFOHEAD
        && bmp_ih.biWidth > 0           // just positive width
        && bmp_ih.biHeight != 0         // just non-zero height
        && bmp_ih.biBitCount == 24      // just 24-bit color
        && bmp_ih.biCompression == 0    // not compression
        && bmp_ih.biClrUsed == 0        // no for 24 bit
        && bmp_ih.biClrImportant == 0   // no for 24 bit
    )) {
        return false;
    }

    // Reading pixels
    bool top_down = bmp_ih.biHeight < 0;
    size_t width = bmp_ih.biWidth;
    size_t height = top_down ? -bmp_ih.biHeight : bmp_ih.biHeight;

    ImageBMP res(width, height);

    char buf[kBufferingBytes];
    size_t buf_i = 0;
    size_t buf_len = 0;

    size_t padding = (4 - ((width * 3) & 3)) & 3;
    size_t byte_width = 3 * width;
    size_t byte_width_sum = 3 * width + padding;
    size_t data_size = byte_width_sum * height;
    
    for (size_t i = 0; i < height; i++) {
        size_t row = top_down ? i : height - 1 - i;
        for (size_t j = 0; j < byte_width_sum; j++) {
            if (buf_i >= buf_len) {
                size_t cur_s = data_size > kBufferingBytes ? kBufferingBytes : data_size;
                data_size -= cur_s;
                buf_len = cur_s;
                buf_i = 0;
                if (!stream.read(buf, cur_s)) return false;
            }
            char cur_byte = buf[buf_i++];
            if (j >= byte_width) continue;
            size_t col = j / 3;
            if (j % 3 == 0) res.Pixel(row, col).B = cur_byte;
            else if (j % 3 == 1) res.Pixel(row, col).G = cur_byte;
            else res.Pixel(row, col).R = cur_byte;
        }
    }

    return true;
}


bool HandlerBMP::Write(const ImageBMP& image, const char* path, bool top_down) {
    std::ofstream stream(path, std::ios_base::binary);
    uint32_t width  = image.GetWidth();
    uint32_t height = image.GetHeight();

    uint32_t padding   = (4 - ((width * 3) & 3)) & 3;
    size_t byte_width = 3 * width;
    size_t byte_width_sum = 3 * width + padding;
    uint32_t data_size = byte_width_sum * height;

    // Headers
    BMPFileHeader bmp_fh = {
        0x4D42,         // bfType
        54 + data_size, // bfSize
        0,              // bfReserved1
        0,              // bfReserved2
        54              // bfOffBits
    };
    BMPInfoHeader bmp_ih = {
        40,             // biSize
        (int32_t)width, // biWidth
        (int32_t)height * (top_down ? -1 : 1), // biHeight
        1,              // biPlanes
        24,             // biBitCount
        0,              // biCompression
        data_size,      // biSizeImage
        0,              // biXPelsPerMeter
        0,              // biYPelsPerMeter
        0,              // biClrUsed
        0               // biClrImportant
    };

    // Writing headers
    if (!stream.is_open()
        || !StructHandler::TryWrite(stream, (char*)(&bmp_fh), sizeof(BMPFileHeader))
        || !StructHandler::TryWrite(stream, (char*)(&bmp_ih), sizeof(BMPInfoHeader))
    ) {
        return false;
    }

    // Writing pixels
    char buf[kBufferingBytes];
    size_t buf_i = 0;
    
    for (size_t i = 0; i < height; i++) {
        size_t row = top_down ? i : height - 1 - i;
        for (size_t j = 0; j < byte_width_sum; j++) {
            if (buf_i >= kBufferingBytes) {
                buf_i = 0;
                if (!stream.write(buf, kBufferingBytes)) return false;
            }
            if (j >= byte_width) {
                buf[buf_i++] = 0;
                continue;
            }
            size_t col = j / 3;
            if (j % 3 == 0) buf[buf_i++] = image.Pixel(row, col).B;
            else if (j % 3 == 1) buf[buf_i++] = image.Pixel(row, col).G;
            else buf[buf_i++] = image.Pixel(row, col).R;
        }
    }
    if (buf_i != 0)
        if (!stream.write(buf, buf_i))
            return false;

    return true;
}
