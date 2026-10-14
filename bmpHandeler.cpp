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

    BMPFileHeader bmp_fh;
    BMPInfoHeader bmp_ih;

    if (!stream.is_open()
        || !StructHandler::TryRead(stream, (char*)(&bmp_fh), sizeof(BMPFileHeader))
        || !StructHandler::TryRead(stream, (char*)(&bmp_ih), sizeof(BMPInfoHeader)))
    {
        ok = false;
        return ImageBMP();
    }

    ImageBMP res(bmp_ih.biWidth, bmp_ih.biHeight);

    size_t padding = (4 - ((bmp_ih.biWidth * 3) & 3)) & 3;

    char buf[kBufferingBytes];
    size_t buf_i = 0;
    size_t buf_len = 0;

    for (size_t i = 0; i < bmp_ih.biHeight; i++) {
        for (size_t j = 0; j < bmp_ih.biWidth; j++) {
            unsigned char bgr[3];
            for (size_t k = 0; k < 3; k++) {
                if (buf_i == buf_len) {
                    stream.read(buf, kBufferingBytes);
                    buf_len = (size_t)stream.gcount();
                    buf_i = 0;
                    if (buf_len == 0) {
                        ok = false;
                        return ImageBMP();
                    }
                }
                bgr[k] = (unsigned char)buf[buf_i++];
            }
            res.Pixel(bmp_ih.biHeight - 1 - i, j).B = bgr[0];
            res.Pixel(bmp_ih.biHeight - 1 - i, j).G = bgr[1];
            res.Pixel(bmp_ih.biHeight - 1 - i, j).R = bgr[2];
        }

        for (size_t p = 0; p < padding; p++) {
            if (buf_i == buf_len) {
                stream.read(buf, kBufferingBytes);
                buf_len = (size_t)stream.gcount();
                buf_i = 0;
                if (buf_len == 0) {
                    ok = false;
                    return ImageBMP();
                }
            }
            buf_i++;
        }
    }

    ok = true;
    return res;
}


void HandlerBMP::Write(const ImageBMP& image, const char* path, bool& ok) {

}
