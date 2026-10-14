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

ImageBMP HandlerBMP::Read(const char* path, bool& ok) {
    std::ifstream stream(path, std::ios_base::binary);

    // Reading headers
    BMPFileHeader bmp_fh;
    BMPInfoHeader bmp_ih;

    if (!stream.is_open()
        || !StructHandler::TryRead(stream, (char*)(&bmp_fh), sizeof(BMPFileHeader))
        || !StructHandler::TryRead(stream, (char*)(&bmp_ih), sizeof(BMPInfoHeader)))
    {
        ok = false;
        return ImageBMP();
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
        ok = false;
        return ImageBMP();
    }

    // Reading colors
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
    
    for (size_t i = 0; i < height; i++) {
        size_t row = top_down ? i : height - 1 - i;
        for (size_t j = 0; j < byte_width_sum; j++) {
            if (buf_i >= buf_len) {
                stream.read(buf, kBufferingBytes);
                buf_len = stream.gcount();
                buf_i = 0;
                if (buf_len == 0) {
                    ok = false;
                    return ImageBMP();
                }
            }
            char cur_byte = buf[buf_i++];
            if (j >= byte_width) continue;
            size_t col = j / 3;
            if (j % 3 == 0) res.Pixel(row, col).B = cur_byte;
            else if (j % 3 == 1) res.Pixel(row, col).G = cur_byte;
            else res.Pixel(row, col).R = cur_byte;
        }
    }

    ok = true;
    return res;
}


void HandlerBMP::Write(const ImageBMP& image, const char* path, bool& ok) {

}
