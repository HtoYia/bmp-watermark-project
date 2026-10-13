#include <imageBMP.h>

class ReaderWriterBMP {
public:
    ImageBMP Read(const char* path, bool& out);
    void Wtite(const ImageBMP& image, const char* path, bool& out);
};
